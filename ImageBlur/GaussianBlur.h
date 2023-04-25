#pragma once
#include "IBlur.h"

#define MAX_RADIUS 10
#define LOOKUP_TABLE_SIZE 256
#define CONVERTIMAGEFACTOR 5

class GaussianBlur : public IBlur
{
public:
	virtual void applyBlur(vector<Pixel>& pixels, TGAImageHeader& header, const double& sigma) override;
	GaussianBlur();
	~GaussianBlur();

private:
	void precomputeLUT();

	vector<double> calculateGaussianKernel(const double& sigma, int& radius);

	void applyBlurMultiThread(vector<Pixel>& pixels, TGAImageHeader& header, const vector<double>& kernel, const int& radius);

	unique_ptr<unique_ptr<double[]>[]> lookup_table;

};

