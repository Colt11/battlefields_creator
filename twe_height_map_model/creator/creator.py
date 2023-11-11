import torch
from torch.utils.data import DataLoader
import numpy
from PIL import Image


class Creator:
	def __init__(self, model, dataset, weight_file_path, result_name):
		self.model = model
		self.dataset = dataset
		self.weight_file_path = weight_file_path
		self.result_name = result_name


	def create_prediction_image(self, prediction, name):
		prediction_tmp = prediction.copy()
		prediction_tmp *= 255

		prediction_tmp = numpy.reshape(prediction_tmp, (prediction_tmp.shape[0], prediction_tmp.shape[1], 1))

		prediction_image = prediction_tmp.copy()
		prediction_image = numpy.concatenate((prediction_image, prediction_tmp.copy()), axis=2)
		prediction_image = numpy.concatenate((prediction_image, prediction_tmp.copy()), axis=2)
		alpha = numpy.zeros((prediction_image.shape[0], prediction_image.shape[1], 1))
		alpha = numpy.full((alpha.shape[0], alpha.shape[1], 1), 255)
		prediction_image = numpy.concatenate((prediction_image, alpha.copy()), axis=2)

		img = Image.fromarray(numpy.uint8(prediction_image))

		img.save(name)


	def get_square_param(self, current_pos, result_pos, border_size, max_side_size):
		if current_pos < 0:
			square_pos = current_pos * -1 + border_size
			square_side_size = max_side_size - square_pos + border_size
		else:
			square_pos = border_size
			square_side_size = min(max_side_size, max_side_size - result_pos)
		return  square_pos, square_side_size


	def create_prediction(self, data_full_shape, training_data_loader, rotate, border_size):
		prediction = numpy.float32(numpy.zeros((data_full_shape[0], data_full_shape[1], 1)))
		for i, (data, pos) in enumerate(training_data_loader):
			output = self.model(data.cuda())[0]
			for j, prediction_image in enumerate(output.cpu()):
				current_prediction = prediction_image.detach().numpy()
				current_prediction = numpy.ascontiguousarray(current_prediction.T)
				current_prediction = numpy.rot90(current_prediction, 4 - rotate)

				current_pos_r = pos[0][j].data + border_size
				current_pos_c = pos[1][j].data + border_size

				result_pos_r = max(current_pos_r, 0)
				result_pos_c = max(current_pos_c, 0)

				square_pos_r, square_side_size_r = self.get_square_param(current_pos_r, result_pos_r, border_size, data_full_shape[0])
				square_pos_c, square_side_size_c = self.get_square_param(current_pos_c, result_pos_c, border_size, data_full_shape[1])

				current_prediction = current_prediction[square_pos_r: square_pos_r + square_side_size_r, square_pos_c: square_pos_c + square_side_size_c]
				prediction[result_pos_r: result_pos_r + square_side_size_r, result_pos_c: result_pos_c + square_side_size_c] = current_prediction.copy()

		prediction[prediction < 0] = 0
		prediction[prediction > 1] = 1

		return prediction


	def create(self, batch_size=1, with_shift=False):
		assert not self.model.get_is_training()

		print("Start creating of " + self.result_name + " map")

		self.model.load_state_dict(torch.load(self.weight_file_path))
		self.model.to(torch.cuda.current_device())
		self.model.eval()

		predictions = []
		data_full_shape = self.dataset.get_full_shape_images_map()

		if with_shift:
			shift_count = 4
		else:
			shift_count = 1

		for rotate in range(4):
			for shift_index in range(shift_count):
				self.dataset.set_shift_index(shift_index)
				training_data_loader = DataLoader(dataset=self.dataset, batch_size=batch_size, shuffle=False)
				prediction = self.create_prediction(data_full_shape, training_data_loader, rotate, self.dataset.get_border_size())
				predictions.append(prediction)
			self.dataset.rotate()

		result_prediction = numpy.float32(numpy.zeros((data_full_shape[0], data_full_shape[1], data_full_shape[2])))
		for prediction in predictions:
			result_prediction += prediction

		result_prediction /= len(predictions)
		predictions.clear()

		self.create_prediction_image(result_prediction, self.result_name)