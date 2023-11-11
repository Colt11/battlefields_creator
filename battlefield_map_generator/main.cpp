#include <iostream>
#include "torch_generator.h"
#include "tree_generator.h"
#include "color_map_generator.h"


int main(int argc, char* argv[])
{
	TorchGenerator tg{};
	if (!tg.Load("data\\battleterrain_model_scripted.pt")) {
		return 1;
	}

	cv::Mat heigthMap = tg.Generate(argv[1], argv[2], "height_map");
	if (heigthMap.data == nullptr) {
		return 1;
	}

	TreeGenerator treeGenerator{};
	if (!treeGenerator.Generate(argv[2], "bmd.tree_list", heigthMap, std::stoi(argv[3]), std::stof(argv[4]), std::stoi(argv[5]))) {
		return 1;
	}

	ColorMapGenerator colorMapGenerator{};
	if (!colorMapGenerator.Generate(argv[2], "colour_map", heigthMap)) {
		return 1;
	}

	return 0;
}
