#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "ImageBlurStruct.h"

using namespace std;
using namespace MyImageBlurStruct;

template<typename T>
class IImageReaderWriter {
public:

	virtual void readFile(const string& filename, vector<Pixel>& pixels, T& header) = 0;

	virtual void writeFile(const string& filename, const vector<Pixel>& pixels,const T& header) = 0;

	virtual ~IImageReaderWriter() {}
};
