#pragma once


#include <string>
#include <opencv2/core/mat.hpp>


class ColorMapGenerator
{
public:
	ColorMapGenerator() = default;

	bool Generate(const std::string& dstPath, const std::string& dstFileName, const cv::Mat& heigthMap);

private:
	cv::Mat CreateMap(const std::string& baseMapPath, const cv::Mat& heigthMap, bool inverted);
};