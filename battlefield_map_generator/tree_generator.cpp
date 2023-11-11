#include "tree_generator.h"
#include <vector>
#include <filesystem>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>


static const char* etwHead = "CEAB0000000000004C52445A";
static const char* tail = "02000D00545245455F4C4F445F4C4953540900545245455F4C495354";

static const char* voidRecord0 = "0800000000";
static const char* voidRecord3 = "0803000000";

static const uint16_t recordTagName0 = 0;
static const uint16_t recordTagName1 = 1;

static const uint8_t recordTagVersion1 = 1;
static const uint8_t recordTagVersion2 = 2;


static void HexToBytes(const std::string& hex, std::vector<unsigned char>& outBytes) {
   for (unsigned int i = 0; i < hex.length(); i += 2) {
      std::string byteString = hex.substr(i, 2);
      char byte = (char)strtol(byteString.c_str(), NULL, 16);
      outBytes.push_back(byte);
   }
}


TreeGenerator::TreeGenerator()
{
   typeBytes[INT_TYPE] = 0x04;
   typeBytes[UINT32_TYPE] = 0x08;
   typeBytes[PAIR_FLOAT_TYPE] = 0x0C;
   typeBytes[PAIR_RECORD_NODE] = 0x80;
   typeBytes[STRING_TYPE] = 0x0E;

   treeTypes.push_back(L"lc_eu_south_s_arid-Hazel_Humid_shrub");
   treeTypes.push_back(L"lc_eu_south_s_arid-Lowbrush_shrub");
   treeTypes.push_back(L"lc_eu_south_s_arid-Lombardy_Poplar_tree");
   treeTypes.push_back(L"lc_eu_south_s_arid-Stone_Pine_tree");
   treeTypes.push_back(L"lc_eu_south_s_arid-Common_Olive_tree");
}


void TreeGenerator::WriteHead(std::FILE* fp, uint32_t countOfTreeTypes, Offsets& offsets)
{
   std::vector<unsigned char> buff;
   HexToBytes(etwHead, buff);
   std::fwrite(buff.data(), sizeof(char), buff.size(), fp);

   offsets.firstOffsetOfLastTagOffset = std::ftell(fp);
   std::fwrite(&offsets.firstOffsetOfLastTagOffset, sizeof(offsets.firstOffsetOfLastTagOffset), 1, fp);

   std::fwrite(&typeBytes[PAIR_RECORD_NODE], 1, 1, fp);
   std::fwrite(&recordTagName0, sizeof(recordTagName0), 1, fp);
   std::fwrite(&recordTagVersion1, sizeof(recordTagVersion1), 1, fp);

   offsets.secondOffsetOfLastTagOffset = std::ftell(fp);
   std::fwrite(&offsets.secondOffsetOfLastTagOffset, sizeof(offsets.secondOffsetOfLastTagOffset), 1, fp);

   buff.clear();
   HexToBytes(voidRecord3, buff);
   std::fwrite(buff.data(), sizeof(char), buff.size(), fp);

   std::fwrite(&typeBytes[PAIR_RECORD_NODE], 1, 1, fp);
   std::fwrite(&recordTagName1, sizeof(recordTagName1), 1, fp);
   std::fwrite(&recordTagVersion2, sizeof(recordTagVersion2), 1, fp);

   offsets.offsetOfLastPosOffset = std::ftell(fp);
   std::fwrite(&offsets.offsetOfLastPosOffset, sizeof(offsets.offsetOfLastPosOffset), 1, fp);

   std::fwrite(&typeBytes[UINT32_TYPE], 1, 1, fp);
   std::fwrite(&countOfTreeTypes, sizeof(countOfTreeTypes), 1, fp);

}


void TreeGenerator::WriteTail(std::FILE* fp, const Offsets& offsets)
{
   std::vector<unsigned char> buff;
 
   uint32_t lastPosOffset = std::ftell(fp);
   
   std::fwrite(&typeBytes[PAIR_RECORD_NODE], 1, 1, fp);
   std::fwrite(&recordTagName1, sizeof(recordTagName1), 1, fp);
   std::fwrite(&recordTagVersion2, sizeof(recordTagVersion2), 1, fp);

   buff.clear();
   HexToBytes(voidRecord0, buff);
   uint32_t lastByteAfterRecord = std::ftell(fp) + sizeof(uint32_t) + static_cast<uint32_t>(buff.size());

   std::fwrite(&lastByteAfterRecord, sizeof(lastByteAfterRecord), 1, fp);
   std::fwrite(buff.data(), sizeof(char), buff.size(), fp);

   std::fwrite(&typeBytes[PAIR_RECORD_NODE], 1, 1, fp);
   std::fwrite(&recordTagName1, sizeof(recordTagName1), 1, fp);
   std::fwrite(&recordTagVersion2, sizeof(recordTagVersion2), 1, fp);

   buff.clear();
   HexToBytes(voidRecord0, buff);
   lastByteAfterRecord = std::ftell(fp) + sizeof(uint32_t) + static_cast<uint32_t>(buff.size());

   std::fwrite(&lastByteAfterRecord, sizeof(lastByteAfterRecord), 1, fp);
   std::fwrite(buff.data(), sizeof(char), buff.size(), fp);

   buff.clear();
   HexToBytes(tail, buff);
   std::fwrite(buff.data(), sizeof(char), buff.size(), fp);

   uint32_t currentOffset = std::ftell(fp);

   std::fseek(fp, offsets.offsetOfLastPosOffset, SEEK_SET);
   std::fwrite(&lastPosOffset, sizeof(lastPosOffset), 1, fp);
   std::fseek(fp, offsets.secondOffsetOfLastTagOffset, SEEK_SET);
   std::fwrite(&lastByteAfterRecord, sizeof(lastByteAfterRecord), 1, fp);
   std::fseek(fp, offsets.firstOffsetOfLastTagOffset, SEEK_SET);
   std::fwrite(&lastByteAfterRecord, sizeof(lastByteAfterRecord), 1, fp);

   std::fseek(fp, currentOffset, SEEK_SET);
}


void TreeGenerator::WriteTree(std::FILE* fp, const std::wstring& treeType, const std::vector<std::pair<float, float>>& points)
{
   std::fwrite(&typeBytes[STRING_TYPE], 1, 1, fp);
   uint16_t treeTypeLenght = static_cast<uint16_t>(treeType.size());
   std::fwrite(&treeTypeLenght, sizeof(treeTypeLenght), 1, fp);
   std::fwrite(treeType.data(), sizeof(wchar_t), treeType.size(), fp);

   std::fwrite(&typeBytes[UINT32_TYPE], 1, 1, fp);
   uint32_t treeCount = static_cast<uint32_t>(points.size());
   std::fwrite(&treeCount, sizeof(treeCount), 1, fp);

   for (const auto& point : points) {
      std::fwrite(&typeBytes[PAIR_FLOAT_TYPE], 1, 1, fp);
      std::fwrite(&point, sizeof(point), 1, fp);

      std::fwrite(&typeBytes[INT_TYPE], 1, 1, fp);
      uint32_t voidValue = 0;
      std::fwrite(&voidValue, sizeof(voidValue), 1, fp);
   }
}


void TreeGenerator::CreateForest(const cv::Mat& readedPattern, 
   RndDist& sizeDist, RndDist& rotateDist, RndDist& posRDist, RndDist& posCDist, std::mt19937& rnd,
   cv::Mat& resultImage)
{
   cv::Mat pattern;
   cv::resize(readedPattern, pattern, cv::Size(sizeDist(rnd), sizeDist(rnd)));

   int angle = rotateDist(rnd);
   cv::Point2f center((pattern.cols - 1) / 2.0f, (pattern.rows - 1) / 2.0f);
   cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);

   cv::Rect2f bbox = cv::RotatedRect(cv::Point2f{}, pattern.size(), static_cast<float>(angle)).boundingRect2f();
   rot.at<double>(0, 2) += bbox.width / 2.0 - pattern.cols / 2.0;
   rot.at<double>(1, 2) += bbox.height / 2.0 - pattern.rows / 2.0;
   cv::Mat dst;
   cv::warpAffine(pattern, pattern, rot, bbox.size());

   int posR = posRDist(rnd);
   int posC = posCDist(rnd);

   int rows = pattern.rows - std::max((posR + pattern.rows - resultImage.rows), 0);
   int cols = pattern.cols - std::max((posC + pattern.cols - resultImage.cols), 0);
   auto patternRect = pattern(cv::Rect_<int>(0, 0, cols, rows));
   auto resultRect = resultImage(cv::Rect_<int>(posC, posR, cols, rows));
   cv::addWeighted(patternRect, 0.5f, resultRect, 0.5f, 0.0f, resultRect);
}


cv::Mat TreeGenerator::CreateDistributionMap(int teamsSize)
{
   std::srand(static_cast<unsigned int>(time(0)));
   std::random_device dev;
   std::mt19937 rnd(dev());
   RndDist sizeDist(MIN_FOREST_SIZE, teamsSize != 1 ? MAX_FOREST_SIZE : MAX_FOREST_SIZE);
   RndDist rotateDist(0, 360);

   RndDist posRDist(150, 750);
   RndDist posCDist(150, 750);
   if (teamsSize == 2) {
      posRDist = RndDist(0, 800);
      posCDist = RndDist(0, 750);
   } else if (teamsSize == 3) {
      posRDist = RndDist(0, 800);
      posCDist = RndDist(0, 950);
   }

   RndDist forestDist(teamsSize != 1 ? MIN_FOREST_COUNT : MIN_FOREST_COUNT_1x1,
      teamsSize != 1 ? MAX_FOREST_COUNT : MAX_FOREST_COUNT_1x1);

   cv::Mat readedPattern = cv::imread("data\\tree_pattern.bmp", cv::IMREAD_GRAYSCALE);
   if (readedPattern.data == nullptr) {
      std::cout << "Can't load " << "data\\tree_pattern.bmp" << std::endl;
      return {};
   }

   cv::Mat resultImage = cv::Mat::zeros(1025, 1025, CV_8UC1);

   int forestCount = forestDist(rnd);
   for (int i = 0; i < forestCount; ++i) {
      CreateForest(readedPattern, sizeDist, rotateDist, posRDist, posCDist, rnd, resultImage);
   }

   forestDist = RndDist(teamsSize != 1 ? MIN_SMALL_FOREST_COUNT : MIN_SMALL_FOREST_COUNT_1x1,
      teamsSize != 1 ? MAX_SMALL_FOREST_COUNT : MAX_SMALL_FOREST_COUNT_1x1);
   sizeDist = RndDist(MIN_SMALL_FOREST_SIZE, MAX_SMALL_FOREST_SIZE);
   forestCount = forestDist(rnd);
   for (int i = 0; i < 7; ++i) {
      CreateForest(readedPattern, sizeDist, rotateDist, posRDist, posCDist, rnd, resultImage);
   }

   cv::resize(resultImage, resultImage, cv::Size(1025 * 2, 1025 * 2));
   return resultImage;
}


std::vector<std::vector<std::pair<float, float>>> TreeGenerator::CreateTreeList(const cv::Mat& heigthMap, int teamsSize, float waterHeight, int scale)
{
   std::vector<std::vector<std::pair<float, float>>> treeList{ treeTypes.size() };

   cv::Mat distributionMap = CreateDistributionMap(teamsSize);   
   if (distributionMap.data == nullptr) {
      return treeList;
   }

   std::random_device dev;
   std::mt19937 rnd(dev());
   RndDist treeType(0, static_cast<std::mt19937::result_type>(treeTypes.size()) - 1);

   float scaledWaterHeight = waterHeight / scale * 255.0f;

   cv::Mat generatedHeigthMap;
   cv::resize(heigthMap, generatedHeigthMap, cv::Size(1025 * 2, 1025 * 2));

   for (int r = 0; r < distributionMap.rows; ++r) {
      for (int c = 0; c < distributionMap.cols; ++c) {
         if (distributionMap.at<uint8_t>(r, c) > 0) {
            if (static_cast<float>(generatedHeigthMap.at<uint8_t>(generatedHeigthMap.rows - r, c)) < scaledWaterHeight) {
               continue;
            }

            treeList[treeType(rnd)].push_back(std::pair<float, float>{static_cast<float>(c - 1025), static_cast<float>(r - 1025)});

            for (int i = -5; i < 5; ++i) {
               for (int j = -5; j < 5; ++j) {
                  distributionMap.at<uint8_t>(std::clamp(r + i, 0, distributionMap.rows - 1), std::clamp(c + j, 0, distributionMap.cols - 1)) = 0;
               }
            }
         }
      }
   }
   return treeList;
}


bool TreeGenerator::Generate(const std::string& dstPath, const std::string& dstFileName, const cv::Mat& heigthMap, int teamsSize, float waterHeight, int scale)
{
   std::FILE* fp = std::fopen((std::filesystem::path{ dstPath } / std::filesystem::path{ dstFileName }).string().c_str(), "wb");
   if (!fp) {
      return false;
   }

   Offsets offsets{};
   WriteHead(fp, static_cast<uint32_t>(treeTypes.size()), offsets);
   
   std::vector<std::vector<std::pair<float, float>>> points = CreateTreeList(heigthMap, teamsSize, waterHeight, scale);
   for (int i = 0; i < points.size(); ++i) {
      WriteTree(fp, treeTypes[i], points[i]);
   }

   WriteTail(fp, offsets);

   std::fclose(fp);

   return true;
}