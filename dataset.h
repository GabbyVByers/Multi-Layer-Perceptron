#pragma once

#include <vector>

struct HandWrittenDigit
{
	int trueValue = -1;
	float* pixels = nullptr;
};

class DataSet
{
public:

	std::vector<HandWrittenDigit> handWrittenDigits;

	// dataset_members.cpp
	DataSet();
	~DataSet();
};

