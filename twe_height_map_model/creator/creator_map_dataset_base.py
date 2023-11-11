import torch
import math
import numpy


class CreatorMapDatasetBase(torch.utils.data.Dataset):
    def __init__(self, side_size, map_size, border_size=0, need_fill_border=True):
        self.side_size = side_size
        self.half_size = math.floor(side_size / 2)
        self.map_size = map_size
        self.border_size = border_size
        self.need_fill_border = need_fill_border
        if need_fill_border:
            self.border_shift = 2
        else:
            self.border_shift = 0

        self.side_size_without_border = self.side_size - border_size * 2
        self.shift_r = 0
        self.shift_c = 0

        self.images_count = (math.floor((self.map_size[0] - max(self.shift_r, self.border_size) - self.border_size)
                                        / self.side_size_without_border) + self.border_shift) * \
                            (math.floor((self.map_size[1] - max(self.shift_c, self.border_size) - self.border_size)
                                        / self.side_size_without_border) + self.border_shift)

        self.rotation_count = 0
        self.shift_index = 0


    def rotate_images_map(self):
        pass


    def get_images_map(self):
        return numpy.zeros((self.side_size, self.side_size))


    def get_full_shape_images_map(self):
        return self.map_size


    def rotate(self):
        self.rotate_images_map()
        self.rotation_count += 1


    def get_border_size(self):
        return self.border_size


    def __len__(self):
        return self.images_count


    def set_shift_index(self, shift_index):
        self.shift_index = shift_index

        self.shift_r = 0
        self.shift_c = 0

        if shift_index == 1 or shift_index == 3:
            self.shift_r = self.side_size - self.border_size \
                           - math.floor(self.rotationCount * (self.side_size - self.border_size) / 4)

        if shift_index == 1 or shift_index == 2:
            self.shift_c = self.side_size - self.border_size \
                           - math.floor(self.rotationCount * (self.side_size - self.border_size) / 4)

        self.images_count = (math.floor((self.map_size[0] - max(self.shift_r, self.border_size) - self.border_size)
                                        / self.side_size_without_border) + self.border_shift) * \
                            (math.floor((self.map_size[1] - max(self.shift_c, self.border_size) - self.border_size)
                                        / self.side_size_without_border) + self.border_shift)


    def get_square_param(self, current_pos, result_pos):
        if current_pos < 0:
            square_pos = current_pos * -1
            side_size = self.side_size - square_pos
        else:
            square_pos = 0
            side_size = min(self.side_size, self.map_size[0] - result_pos)
        return square_pos, side_size


    def __getitem__(self, index):
        weight = math.floor((self.map_size[0] - max(self.shift_r, self.border_size) - self.border_size)
                            / self.side_size_without_border) + self.border_shift

        if self.need_fill_border:
            current_pos_r = math.floor(index % weight) * self.side_size_without_border + self.shift_r \
                            - self.side_size_without_border
            current_pos_c = math.floor(index / weight) * self.side_size_without_border + self.shift_c \
                            - self.side_size_without_border
        else:
            current_pos_r = math.floor(index % weight) * self.side_size_without_border + self.shift_r
            current_pos_c = math.floor(index / weight) * self.side_size_without_border + self.shift_c

        result_pos_r = max(current_pos_r, 0)
        result_pos_c = max(current_pos_c, 0)

        square_pos_r, side_size_r = self.get_square_param(current_pos_r, result_pos_r)
        square_pos_c, side_size_c = self.get_square_param(current_pos_c, result_pos_c)

        image_map = self.get_images_map()
        image = numpy.float32(numpy.zeros((self.side_size, self.side_size, image_map.shape[2])))

        image[square_pos_r: square_pos_r + side_size_r, square_pos_c: square_pos_c + side_size_c] \
            = image_map[result_pos_r: result_pos_r + side_size_r, result_pos_c: result_pos_c + side_size_c]

        image = image.T
        return image, (result_pos_r, result_pos_c)
