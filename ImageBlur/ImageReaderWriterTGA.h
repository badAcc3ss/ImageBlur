#pragma once
#include "IImageReaderWriter.h"

//create a singleton
class ImageReaderWriterTGA : IImageReaderWriter<TGAImageHeader>
{
public:

	virtual void readFile(const string& filename,vector<Pixel>& pixels,TGAImageHeader& header) override;

	virtual void writeFile(const string& filename, const vector<Pixel>& pixels,const TGAImageHeader& header) override;

	static ImageReaderWriterTGA& getInstance() {
		static ImageReaderWriterTGA instance; // Only instantiated once
		return instance;
	}
	

private:
	ImageReaderWriterTGA() {} // Private constructor
	ImageReaderWriterTGA(const ImageReaderWriterTGA&) = delete; // No copy constructor
	ImageReaderWriterTGA& operator=(const ImageReaderWriterTGA&) = delete; // No copy assignment operator
	ImageReaderWriterTGA(ImageReaderWriterTGA&&) = delete; // No move constructor
	ImageReaderWriterTGA& operator=(ImageReaderWriterTGA&&) = delete; // No move assignment operator
};


