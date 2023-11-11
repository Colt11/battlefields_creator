from trainer.training_map_dataset import BaseMapData
from PIL import Image
import numpy


class VaeHeightMapData(BaseMapData):
	def __init__(self, paths, side_size):
		height_map_path = paths[0]
		height_map = numpy.array(Image.open(height_map_path))

		if len(height_map.shape) > 2:
			if height_map.shape[2] == 4:
				height_map = numpy.delete(height_map, [3], axis=2)
			height_map = numpy.delete(height_map, [1, 2], axis=2)
		else:
			height_map = numpy.reshape(height_map, (height_map.shape[0], height_map.shape[1], 1))

		height_map = numpy.float32(height_map)
		height_map /= 255

		super(VaeHeightMapData, self).__init__(side_size, side_size / 8, height_map.shape)
		self.height_map = height_map


	def get_images_map(self):
		return self.height_map


	def get_labels_map(self):
		return self.height_map