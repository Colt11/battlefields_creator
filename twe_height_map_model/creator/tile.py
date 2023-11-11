import numpy
from PIL import Image
import cv2
import math


def tile_map(height_map, scale, rotate_direction):
    result = numpy.zeros((height_map.shape[0] * scale, height_map.shape[1] * scale, height_map.shape[2]))
    for i in range(scale):
        for j in range(scale):
            arr = height_map.copy()
            if (i % 2 == 0) == rotate_direction:
                arr = numpy.flipud(arr)
            if (j % 2 == 0) == rotate_direction:
                arr = numpy.fliplr(arr)
            result[i * height_map.shape[0]: (i + 1) * height_map.shape[0], j * height_map.shape[1]: (j + 1) * height_map.shape[1]] = arr
    offset_r = math.floor(height_map.shape[0] / 2)
    offset_c = math.floor(height_map.shape[1] / 2)
    result = result[offset_r: result.shape[0] - offset_r, offset_c: result.shape[1] - offset_c]
    result = cv2.resize(result, (height_map.shape[0], height_map.shape[1]))
    return result


def tile_height_map(name, height_map):
    scales = (3, 5, 9)
    for i, scale in enumerate(scales):
        result = tile_map(height_map, scale, rotate_direction=(i == 0))
        img = Image.fromarray(numpy.uint8(result))
        img.save(name + "_" + str(i+1) + ".dds")