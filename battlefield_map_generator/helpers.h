#pragma once


#include <string>
#include <opencv2/core/mat.hpp>


bool SaveToDds(const cv::Mat& grayData, const std::string& fileName);
bool Tile(const cv::Mat& image, const std::string& dstPathBaseTexture, bool saveDds);