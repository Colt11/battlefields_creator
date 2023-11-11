import math
import torch
import os
import cv2
import numpy
from typing import TypeVar, Generic, Type


class BaseMapData:
    def __init__(self, side_size, step, map_size):
        self.step = math.floor(step)
        self.side_size = side_size
        self.half_size = math.floor(side_size / 2)

        self.side_size_for_45_degree_rotate = (math.floor(math.sqrt(self.half_size * self.half_size * 2)) + 1) * 2
        self.offset_square_for_45_degree_rotate = math.floor(self.side_size_for_45_degree_rotate / 2) - self.half_size

        square = numpy.zeros((self.side_size_for_45_degree_rotate, self.side_size_for_45_degree_rotate))
        self.rotate_to_45_degree_matrix = cv2.getRotationMatrix2D(center=(numpy.array(square.shape[:2][::-1]) - 1) / 2.
                                                                  , angle=45, scale=1)

        self.count_of_90_degree_rotated_maps = (math.floor((map_size[0] - self.side_size) / self.step) + 1) \
                                          * (math.floor((map_size[1] - self.side_size) / self.step) + 1)
        self.count_of_45_degree_rotated_maps = ((math.floor((map_size[0] - self.side_size_for_45_degree_rotate) / self.step) + 1)
                                          * (math.floor((map_size[1] - self.side_size_for_45_degree_rotate) / self.step) + 1))
        self.images_count = (self.count_of_90_degree_rotated_maps + self.count_of_45_degree_rotated_maps) * 4


    def get_images_map(self):
        return numpy.zeros((self.side_size_for_45_degree_rotate, self.side_size_for_45_degree_rotate))


    def get_labels_map(self):
        return numpy.zeros((self.side_size_for_45_degree_rotate, self.side_size_for_45_degree_rotate))


    def get_count(self):
        return self.images_count


    def get_data(self, index):
        count = 0
        rotation = 0
        for i in range(8):
            if i % 2 == 0:
                current_count = self.count_of_90_degree_rotated_maps
            else:
                current_count = self.count_of_45_degree_rotated_maps

            if current_count + count > index:
                index -= count
                break
            else:
                count += current_count
                rotation += 1

        return self.get_square(self.get_images_map(), rotation, index), self.get_square(self.get_labels_map(), rotation, index)


    def get_square_for_90_degree_rotate(self, data, rotation, index):
        weight = math.floor((data.shape[0] - self.side_size) / self.step) + 1
        pos_r = math.floor(index % weight) * self.step
        pos_c = math.floor(index / weight) * self.step
        square = data[pos_r: pos_r + self.side_size, pos_c: pos_c + self.side_size].copy()
        square = numpy.rot90(square, math.floor((rotation / 2))).copy()
        return square.copy()


    def get_square_for_45_degree_rotate(self, data, rotation, index):
        weight = math.floor((data.shape[0] - self.side_size_for_45_degree_rotate) / self.step) + 1
        pos_r = math.floor(index % weight) * self.step
        pos_c = math.floor(index / weight) * self.step
        square = data[pos_r: pos_r + self.side_size_for_45_degree_rotate,
                      pos_c: pos_c + self.side_size_for_45_degree_rotate].copy()
        square = numpy.rot90(square, math.floor((rotation / 2))).copy()

        square = cv2.warpAffine(src=square, M=self.rotate_to_45_degree_matrix, dsize=square.shape[:2])
        square = square[self.offset_square_for_45_degree_rotate: self.offset_square_for_45_degree_rotate + self.side_size,
                      self.offset_square_for_45_degree_rotate: self.offset_square_for_45_degree_rotate + self.side_size]
        return square.copy()


    def get_square(self, data, rotation, index):
        if rotation % 2 == 0:
            square = self.get_square_for_90_degree_rotate(data, rotation, index)
        else:
            square = self.get_square_for_45_degree_rotate(data, rotation, index)

        square = numpy.reshape(square, (self.side_size, self.side_size, data.shape[2])).T
        return square


MapDataType = TypeVar("MapDataType")
class TrainingMapDataset(torch.utils.data.Dataset, Generic[MapDataType]):
    def __init__(self, map_data_type: Type[MapDataType], side_size, data_path, map_postfixes):
        super(TrainingMapDataset, self).__init__()
        maps_list = self.collect_data_path(data_path, map_postfixes)
        self.data_list = []
        for maps in maps_list:
            self.data_list.append(map_data_type(maps, side_size))


    def collect_data_path(self, data_path, map_postfixes):
        files = os.listdir(data_path)
        maps_list = []
        for file in files:
            if file.endswith(map_postfixes[0]):
                surrent_map_set = [os.path.join(data_path, file)]
                for i in range(len(map_postfixes) - 1):
                    current_map = (file[:]).replace(map_postfixes[0], map_postfixes[i + 1])
                    if files.count(current_map) == 1:
                        surrent_map_set.append(os.path.join(data_path, current_map))
                    else:
                        surrent_map_set.clear()
                        break

                if surrent_map_set:
                    maps_list.append(surrent_map_set)

        return maps_list


    def __len__(self):
        size = 0
        for data in self.data_list:
            size += data.get_count()
        return size


    def __getitem__(self, index):
        current_count = 0
        for data in self.data_list:
            if data.get_count() + current_count > index:
                current_data = data
                index -= current_count
                return current_data.get_data(index)
            else:
                current_count += data.get_count()

        assert False, "Training data out of range"
        return []
