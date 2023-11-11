#pragma once
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <random>


class TreeGenerator
{
	enum
	{
		INT_TYPE = 0,
		UINT32_TYPE,
		PAIR_FLOAT_TYPE,
		PAIR_RECORD_NODE,
		STRING_TYPE,
		USED_TYPE_COUNT
	};

	struct Offsets
	{
		uint32_t firstOffsetOfLastTagOffset = 0;
		uint32_t secondOffsetOfLastTagOffset = 0;
		uint32_t offsetOfLastPosOffset = 0;
	};

	static constexpr int MIN_FOREST_SIZE = 150;
	static constexpr int MAX_FOREST_SIZE = 300;

	static constexpr int MIN_FOREST_COUNT_1x1 = 1;
	static constexpr int MAX_FOREST_COUNT_1x1 = 3;
	static constexpr int MIN_FOREST_COUNT = 3;
	static constexpr int MAX_FOREST_COUNT = 7;

	static constexpr int MIN_SMALL_FOREST_SIZE = 10;
	static constexpr int MAX_SMALL_FOREST_SIZE = 100;

	static constexpr int MIN_SMALL_FOREST_COUNT_1x1 = 7;
	static constexpr int MAX_SMALL_FOREST_COUNT_1x1 = 15;
	static constexpr int MIN_SMALL_FOREST_COUNT = 10;
	static constexpr int MAX_SMALL_FOREST_COUNT = 25;

	using RndDist = std::uniform_int_distribution<std::mt19937::result_type>;

public:
	TreeGenerator();

	bool Generate(const std::string& dstPath, const std::string& dstFileName, const cv::Mat& heigthMap, int teamsSize, float waterHeight, int scale);

private:
	void WriteHead(std::FILE* fp, uint32_t countOfTreeTypes, Offsets& offsets);
	void WriteTail(std::FILE* fp, const Offsets& offsets);
	void WriteTree(std::FILE* fp, const std::wstring& treeType, const std::vector<std::pair<float, float>>& points);

	cv::Mat CreateDistributionMap(int teamsSize);
	void CreateForest(const cv::Mat& readedPattern,
		RndDist& sizeDist, RndDist& rotateDist, RndDist& posRDist, RndDist& posCDist, std::mt19937& rnd,
		cv::Mat& resultImage);
	std::vector<std::vector<std::pair<float, float>>> CreateTreeList(const cv::Mat& heigthMap, int teamsSize, float waterHeight, int scale);

	std::array<unsigned char, USED_TYPE_COUNT> typeBytes{};
	std::vector<std::wstring> treeTypes;
};