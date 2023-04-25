// ImageBlur.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <future>
#include <algorithm>
#include "ImageReaderWriterTGA.h"
#include "GaussianBlur.h"

using namespace std;

float clamp(float value, float minimum, float maximum) {
	return min(max(value, minimum), maximum);
}

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cerr << "Error: Invalid arguments. Please provide input and output filenames and standard deviation." << endl;
		cerr << "Usage: " << argv[0] << " input_filename output_filename sigma" << endl;
		return 1;
	}

	const string& filename = argv[1];
	const string& outputFilename = argv[2];
	const float sigma = clamp(stof(argv[3]), 0.0f, 1.0f);

	vector<Pixel> pixels;
	ImageReaderWriterTGA& imageProcessor = ImageReaderWriterTGA::getInstance();
	TGAImageHeader header;
	imageProcessor.readFile(filename, pixels, header);

	if (sigma != 0)
	{
		unique_ptr<IBlur> blurLogic = make_unique<GaussianBlur>();

		cout << "Starting image blur operation...\n";

		future<void> futureBlur = async(launch::async, [&] {
			blurLogic->applyBlur(pixels, header, sigma);
			});

		cout << "Image blur operation in progress...\n";

		float lastProgress = -1.0f;
		while (futureBlur.wait_for(chrono::milliseconds(100)) != future_status::ready) {
			const float progress = 100.0f * static_cast<float>(pixels.size()) / (header.width * header.height);
			if (progress != lastProgress) {
				lastProgress = progress;
				cout << "Loading: " << fixed << setprecision(2) << progress << "%\r" << flush;
			}
		}

		futureBlur.get(); // Wait for the async task to complete
	}
		cout << "Finished Blurring Process" << endl;
		imageProcessor.writeFile(outputFilename, pixels, header);

		return 0;
}
