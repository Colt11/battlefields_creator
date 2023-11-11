#include "helpers.h"
#include <cstdio>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>


struct DDS_PIXELFORMAT
{
   uint32_t  size = 32;
   uint32_t  flags = 65;
   uint32_t  fourCC = 0;
   uint32_t  RGBBitCount = 32;
   uint32_t  RBitMask = 16711680;
   uint32_t  GBitMask = 65280;
   uint32_t  BBitMask = 255;
   uint32_t  ABitMask = 4278190080;
};


struct DDS_HEADER
{
   uint32_t				magic = 542327876;
   uint32_t          size = 124;
   uint32_t          flags = 4111;
   uint32_t          height = 1025;
   uint32_t          width = 1025;
   uint32_t          pitchOrLinearSize = 4100;
   uint32_t          depth = 0; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
   uint32_t          mipMapCount = 0;
   uint32_t          reserved1[11];
   DDS_PIXELFORMAT   ddspf;
   uint32_t          caps = 4096;
   uint32_t          caps2 = 0;
   uint32_t          caps3 = 0;
   uint32_t          caps4 = 0;
   uint32_t          reserved2;
};


bool SaveToDds(const cv::Mat& grayData, const std::string& fileName)
{
   std::vector<cv::Mat> channels{ grayData, grayData, grayData, cv::Mat{cv::Size(grayData.rows, grayData.cols), CV_8UC1, 255} };
   cv::Mat image;
   cv::merge(channels, image);

   std::FILE* fp = std::fopen(fileName.c_str(), "wb");
   if (!fp) {
      return false;
   }

   DDS_HEADER header{};
   std::fwrite(&header, sizeof(header), 1, fp);
   std::fwrite(image.data, image.rows * image.cols * 4, 1, fp);
   std::fclose(fp);

   return true;
}


bool Tile(const cv::Mat& image, const std::string& dstPathBaseTexture, bool saveDds)
{
   bool result = false;
   if (saveDds) {
      result = SaveToDds(image, dstPathBaseTexture + "_0.dds");
   } else  {
      result = cv::imwrite(dstPathBaseTexture + "_0.jpg", image);
   }
   if (!result){
      std::cout << "Can't save " << dstPathBaseTexture << std::endl;
      return false;
   }

   std::array<int, 3> scales{ 3, 5, 9 };
   for (int i = 0; i < 3; ++i) {
      int imageScale = scales[i] - 1;
      int newRows = image.rows / imageScale;
      int newCols = image.cols / imageScale;

      cv::Mat scaledImage;
      cv::resize(image, scaledImage, cv::Size(newRows, newCols));

      cv::Mat resultImage{ cv::Size(scaledImage.rows * scales[i], scaledImage.cols * scales[i]), image.type(), 255 };
      bool rotateDirection = i == 0;
      for (int r = 0; r < scales[i]; ++r) {
         for (int c = 0; c < scales[i]; ++c) {
            int posR = r * scaledImage.rows;
            int posC = c * scaledImage.cols;
            cv::Mat tmp = resultImage(cv::Rect_<int>(posR, posC, scaledImage.rows, scaledImage.cols));
            scaledImage.copyTo(tmp);

            if ((c % 2 == 0) == rotateDirection) {
               cv::flip(tmp, tmp, 0);
            }
            if ((r % 2 == 0) == rotateDirection) {
               cv::flip(tmp, tmp, 1);
            }
         }
      }

      int posR = scaledImage.rows / 2;
      int posC = scaledImage.cols / 2;
      if (saveDds) {
         std::string path = dstPathBaseTexture + "_" + std::to_string(i + 1) + ".dds";
         result = SaveToDds(resultImage(cv::Rect_<int>(posR, posC, image.rows, image.cols)), path);
      } else {
         std::string path = dstPathBaseTexture + "_" + std::to_string(i + 1) + ".jpg";
         result = cv::imwrite(path, resultImage(cv::Rect_<int>(posR, posC, image.rows, image.cols)));
      }

      if (!result) {
         std::cout << "Can't save " << dstPathBaseTexture + "_" + std::to_string(i + 1) << std::endl;
         return false;
      }
   }
   return true;
}