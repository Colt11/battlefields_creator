#pragma once

#include <string>
#include <torch/script.h>
#include <opencv2/core/mat.hpp>



class TorchGenerator
{
public:
   TorchGenerator() = default;

   cv::Mat Generate(const std::string& srcTextureFilePath, const std::string& dstPath, const std::string& dstTextureName);
   bool Load(const std::string& weightsPath);

private:
   cv::Mat GenerateImage(at::Tensor);

   torch::jit::script::Module module{};
};