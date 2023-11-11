#include "torch_generator.h"
#include <torch/script.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <filesystem>
#include "helpers.h"


cv::Mat TorchGenerator::Generate(const std::string& srcTextureFilePath, const std::string& dstPath, const std::string& dstTextureName)
{
   cv::Mat readedImage = cv::imread(srcTextureFilePath, cv::IMREAD_GRAYSCALE);
   if (readedImage.data == nullptr) {
      return {};
   }

   if (readedImage.size().height != 1025 || readedImage.size().width != 1025) {
      return {};
   }

   at::Tensor tensorImage = torch::from_blob(readedImage.data, { readedImage.rows, readedImage.cols, 1 }, at::kByte);
   tensorImage = tensorImage.toType(c10::kFloat).div(255.0f);
   cv::Mat result(static_cast<int>(tensorImage.size(0)), static_cast<int>(tensorImage.size(1)), CV_32FC1, 0.0f);

   for (int i = 0; i < 4; ++i) {
      cv::Mat matImage = GenerateImage(tensorImage);
      if (matImage.data == nullptr) {
         return {};
      }

      for (int j = i; j > 0; --j) {
         cv::rotate(matImage, matImage, cv::ROTATE_90_CLOCKWISE);
      }

      result += matImage;
      tensorImage = tensorImage.rot90();
   }

   result /= 4.0f;
   result *= 255.0f;
   cv::blur(result, result, { 5, 5 });

   cv::Mat resultGray(result.rows, result.cols, CV_8UC1);
   result.convertTo(resultGray, CV_8UC1);

   std::string dstTexturePath = (std::filesystem::path{dstPath} / std::filesystem::path{dstTextureName}).string();
   if (!Tile(resultGray, dstTexturePath, true)) {
      return {};
   }

   return resultGray;
}


cv::Mat TorchGenerator::GenerateImage(at::Tensor tensorImage)
{
   std::vector<torch::jit::IValue> inputs;
   at::Tensor tmpTensorImage = tensorImage.permute({ 2,0,1 });

   tmpTensorImage.unsqueeze_(0);
   tmpTensorImage.to(c10::DeviceType::CPU);
   inputs.push_back(tmpTensorImage);

   at::Tensor outputs;
   try {
      outputs = module.forward(inputs).toTuple()->elements()[0].toTensor()[0].permute({ 1, 2, 0 }).contiguous();
   } catch (const c10::Error& e) {
      std::cout << e.msg() << std::endl;
      return {};
   }

   cv::Mat result(static_cast<int>(outputs.size(0)), static_cast<int>(outputs.size(1)), CV_32FC1, outputs.data_ptr<float>());
   return result;
}


bool TorchGenerator::Load(const std::string& weightsPath)
{
   try {
      module = torch::jit::load(weightsPath);
      module.eval();
   } catch (const c10::Error& e) {
      std::cout << e.msg() << std::endl;
      return false;
   }
   return true;
}