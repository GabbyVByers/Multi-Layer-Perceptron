
#include "dataset.h"
#include <cassert>
#include <cstdint>
#include <fstream>

inline unsigned int returnReversedBytes(unsigned int value)
{
	return ((value & 0x000000FF) << 24) |
		   ((value & 0x0000FF00) << 8)  |
		   ((value & 0x00FF0000) >> 8)  |
		   ((value & 0xFF000000) >> 24);
}

struct ImageFileHeader
{
	unsigned int magicNumber = 0;
	unsigned int numImages = 0;
	unsigned int numRows = 0;
	unsigned int numColumns = 0;
};

struct LabelFileHeader
{
	unsigned int magicNumber = 0;
	unsigned int numLabels = 0;
};

DataSet::DataSet()
{
	std::ifstream digits_file("train-images.idx3-ubyte", std::ios::binary | std::ios::ate);
	size_t sizeDigits = digits_file.tellg(); digits_file.seekg(0, std::ios::beg);
	std::ifstream labels_file("train-labels.idx1-ubyte", std::ios::binary | std::ios::ate);
	size_t sizeLabels = labels_file.tellg(); labels_file.seekg(0, std::ios::beg);

	ImageFileHeader imageFileHeader;
	digits_file.read(reinterpret_cast<char*>(&imageFileHeader), sizeof(ImageFileHeader));
	LabelFileHeader labelFileHeader;
	labels_file.read(reinterpret_cast<char*>(&labelFileHeader), sizeof(LabelFileHeader));

	imageFileHeader.magicNumber = returnReversedBytes(imageFileHeader.magicNumber);
	imageFileHeader.numImages = returnReversedBytes(imageFileHeader.numImages);
	imageFileHeader.numRows = returnReversedBytes(imageFileHeader.numRows);
	imageFileHeader.numColumns = returnReversedBytes(imageFileHeader.numColumns);

	labelFileHeader.magicNumber = returnReversedBytes(labelFileHeader.magicNumber);
	labelFileHeader.numLabels = returnReversedBytes(labelFileHeader.numLabels);

	unsigned char* labels = new unsigned char[labelFileHeader.numLabels];
	labels_file.read(reinterpret_cast<char*>(labels), sizeof(unsigned char) * labelFileHeader.numLabels);

	assert(imageFileHeader.numImages == labelFileHeader.numLabels);

	handWrittenDigits.clear();
	handWrittenDigits.reserve(imageFileHeader.numImages);
	for (int i = 0; i < imageFileHeader.numImages; i++)
	{
		unsigned char* pixels = new unsigned char[784];
		digits_file.read(reinterpret_cast<char*>(pixels), sizeof(unsigned char) * 784);


		HandWrittenDigit handWrittenDigit;
		handWrittenDigit.pixels = new float[784];
		for (int i = 0; i < 784; i++)
		{
			float activation = (float)pixels[i] / 255.0f;
			handWrittenDigit.pixels[i] = activation;
		}
		unsigned char digit = labels[i];
		handWrittenDigit.trueValue = digit;
		handWrittenDigits.push_back(handWrittenDigit);
		delete[] pixels;
	}
	delete[] labels;
}

DataSet::~DataSet()
{
	for (auto& digit : handWrittenDigits)
	{
		delete[] digit.pixels;
	}
}

