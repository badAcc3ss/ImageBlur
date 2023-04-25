# ImageBlur Filter
This is an implementation of the Gaussian blur image filter algorithm in C++.

## Description
The Gaussian blur filter is a commonly used image processing technique to reduce noise and smooth out images.
The algorithm convolves the image with a 2D Gaussian kernel, which is a matrix of weights that determine the intensity of each pixel in the resulting blurred image.
This implementation uses precomputed lookup tables for values of the Gaussian function, which helps to speed up the blurring operation.
The implementation also uses multi-threading to speed up the blurring operation by dividing the work into multiple threads that operate on different parts of the image simultaneously.

## Dependencies
This implementation requires the following dependencies:

## C++ 11 or higher
Visual Studio 2019

## Usage
To use this implementation of the Gaussian blur filter, you can follow these steps:

Clone the repository or download the source code files.
Make sure you are running VS2019.(Not tested with any other versions)
You can build the .sln project file directly through VS2019.
To run the code make sure to provide the command line arguments from Debugging Options in Project properties
The project file contains an Images folder with the Image to be blurred , provide the entire path of the image , output path/filename.tga and sigma value for standard deviation.

Format :
path\ImageBlur\Images\Sharmob.tga path\ImageBlur\Images\SharmobBlur.tga 0-1 (sigma value).
Alternatively can run the RunImageBlurExe.bat with similar args this runs the executable file, providing the input and output image file paths, and the sigma value for the Gaussian kernel.


### Files
main.cpp: The main file that reads the input image, applies the Gaussian blur filter, and writes the output image.

ImageReaderWriterTGA.h and ImageReaderWriterTGA.cpp:A class that provides functionality for reading and writing TGA image files.

GaussianBlur.h: A class that implement the bluring algorithm and applies to the image to be written

Classes and Functions

## GaussianBlur
This class provides the implementation of the Gaussian blur filter. It has the following member functions:

precomputeLUT(): A private member function that precomputes a lookup table for values of the Gaussian function.

applyBlur(vector<Pixel> & pixels, TGAImageHeader & header, const double& sigma): A public member function that applies a Gaussian blur to an image represented as a vector of pixels.
 
calculateGaussianKernel(const double& sigma, int& radius): A private member function that calculates the Gaussian kernel for a given sigma value.
 
applyBlurMultiThread(vector<Pixel> & pixels, TGAImageHeader & header, const vector<double> & kernel, const int& radius): A private member function that applies the Gaussian blur to an image using multi-threading.
 
### ImageReaderWriterTGA
This class provides functionality for reading and writing TGA image files. It has the following member functions:
 
readFile(string filename, vector<Pixel> & pixels, TGAImageHeader & header): A public member function that reads a TGA image file.
 
writeFile(string filename, vector<Pixel> & pixels, TGAImageHeader & header): A public member function that writes a TGA image file.

### License
This implementation is licensed under the MIT License. See the LICENSE file for more information.
