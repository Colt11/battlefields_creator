#include "color_map_generator.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <filesystem>
#include <iostream>
#include "helpers.h"


bool ColorMapGenerator::Generate(const std::string& dstPath, const std::string& dstFileName, const cv::Mat& heigthMap)
{
	cv::Mat greenMap = CreateMap("data\\green_base.png", heigthMap, true);
	cv::Mat brownMap = CreateMap("data\\brown_base.png", heigthMap, false);

	if (greenMap.data == nullptr || brownMap.data == nullptr) {
		return false;
	}

	cv::Mat result{};
	cv::addWeighted(greenMap, 0.4f, brownMap, 0.6f, 0.0f, result);

	cv::resize(result, result, cv::Size(2048, 2048));
	std::string texturePath = (std::filesystem::path{ dstPath } / std::filesystem::path{ dstFileName }).string();
	return Tile(result, texturePath, false);
}


cv::Mat ColorMapGenerator::CreateMap(const std::string& baseMapPath, const cv::Mat& heigthMap, bool inverted)
{
	std::vector<cv::Mat> channels{3};
	cv::Mat colorMap = cv::imread(baseMapPath, cv::IMREAD_COLOR);
	if (colorMap.data == nullptr) {
		std::cout << "Can't load " << baseMapPath << std::endl;
		return {};
	}

	cv::cvtColor(colorMap, colorMap, cv::COLOR_RGB2HSV);
	cv::split(colorMap, channels);
	
	if (inverted) {
		channels[1] = uint8_t{255} - heigthMap;
	} else {
		channels[1] = heigthMap;
	}
	
	cv::merge(channels, colorMap);
	cv::cvtColor(colorMap, colorMap, cv::COLOR_HSV2RGB);
	
	return colorMap;
}