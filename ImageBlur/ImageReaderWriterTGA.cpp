#include "ImageReaderWriterTGA.h"
#include <mutex>
#include <WinSock2.h>


void ImageReaderWriterTGA::readFile(const string& filename, vector<Pixel>& pixels, TGAImageHeader& header)
{
	ifstream file(filename, ios::binary);
	if (!file.is_open()) {
		throw runtime_error("Failed to open file.");
	}

	// Read the header
	file.read(reinterpret_cast<char*>(&header), sizeof(TGAImageHeader));

	// Skip the image ID field (if present)
	file.seekg(header.id_length, ios::cur);

	// Check the image type (we only support uncompressed RGB or RGBA images)
	if (header.image_type != 2 && header.image_type != 3) {
		throw runtime_error("Unsupported TGA image type.");
	}

	// Check the pixel depth (we only support 24 or 32 bits per pixel)
	if (header.bits_per_pixel != 24 && header.bits_per_pixel != 32) {
		throw runtime_error("Unsupported pixel depth.");
	}

	// Allocate memory for the image data
	const int pixel_size = header.bits_per_pixel / 8;
	pixels.resize(header.width * header.height);

	// Read the image data
	mutex pixels_mutex; // Create a mutex to synchronize access to pixels
	for (int i = 0; i < header.width * header.height; i++) {
		Pixel pixel;
		file.read(reinterpret_cast<char*>(&pixel), pixel_size);
		{
			lock_guard<mutex> lock(pixels_mutex); // Use a lock guard to synchronize access to pixels
			pixels[i] = pixel;
		}
	}

	// Close the file
	file.close();
}


void ImageReaderWriterTGA::writeFile(const string& filename, const vector<Pixel>& pixels, const TGAImageHeader& header)
{
	ofstream file(filename, ios::binary);
	if (!file.is_open()) {
		throw runtime_error("Failed to open file.");
	}

	// Write the header
	file.write(reinterpret_cast<const char*>(&header), sizeof(TGAImageHeader));

	// Write the image data
	const int pixel_size = header.bits_per_pixel / 8;
	vector<char> buffer(pixels.size() * pixel_size);
	{
		size_t i = 0;
		mutex buffer_mutex; // Create a mutex to synchronize access to buffer
		for (const auto& pixel : pixels) {
			memcpy(&buffer[i * pixel_size], &pixel, pixel_size);
			i++;
		}
	}
	file.write(buffer.data(), buffer.size());

	// Close the file
	file.close();
}
