#pragma once
#include "algorithm"
#include "thread"
#include <stdlib.h>
#include <chrono>
#include <vector>
#include "ImageBlurStruct.h"

using namespace MyImageBlurStruct;
using namespace std;

class IBlur {
public:
	virtual void applyBlur(vector<Pixel>& pixels, TGAImageHeader& header, const double& sigma) = 0;
	virtual ~IBlur() {}
};

