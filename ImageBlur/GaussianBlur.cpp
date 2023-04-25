#include "GaussianBlur.h"


GaussianBlur::GaussianBlur()
{
	lookup_table = make_unique<unique_ptr<double[]>[]>(MAX_RADIUS + 1);
	for (int i = 0; i <= MAX_RADIUS; i++) {
		lookup_table[i] = unique_ptr<double[]>(new double[LOOKUP_TABLE_SIZE]);
	}

	//prepare LUT for values of Gaussian function
	precomputeLUT(); 
}


GaussianBlur::~GaussianBlur()
{
}

void GaussianBlur::precomputeLUT()
{
	// Precomputing the lookup table for different radius values and corresponding sigma values.
	for (int r = 0; r <= MAX_RADIUS; r++) {
		double sigma = r / 3.0;
		for (int i = 0; i < LOOKUP_TABLE_SIZE; i++) {
			// calculate the x value for each index in the lookup table
			double x = (i - LOOKUP_TABLE_SIZE / 2.0) / LOOKUP_TABLE_SIZE;
			// calculate the Gaussian function value for given x, sigma, and radius
			lookup_table[r][i] = exp(-0.5 * x * x / (sigma * sigma));
		}
	}
}

void GaussianBlur::applyBlur(vector<Pixel> & pixels, TGAImageHeader & header, const double& sigma)
{
	// calculate the radius of the kernel for given sigma value
	int radius = 0;

	// calculate the Gaussian kernel values for given sigma and radius
	vector<double> kernel = calculateGaussianKernel(sigma * CONVERTIMAGEFACTOR, radius);

	// apply the Gaussian blur on the image using the computed kernel values and radius
	applyBlurMultiThread(pixels, header, kernel, radius);
}

vector<double> GaussianBlur::calculateGaussianKernel(const double& sigma, int& radius)
{
	// calculate the radius of the kernel for given sigma value
	radius = ceil(sigma);

	// calculate the size of the kernel using the computed radius value
	int kernel_size = 2 * radius + 1;

	// for small sigma values, reduce the size of the kernel to 3 and set radius to 1
	//this code to optimize the smaller value which also results in small blur effect for 0
	if (sigma < 1.0) {
		kernel_size = 3;
		radius = 1;
	}

	// create a vector to hold the kernel values
	vector<double> kernel(kernel_size);

	// calculate the sum of all kernel values
	double sum = 0;
	for (int i = 0; i < kernel_size; i++) {
		// calculate the x value for each index in the kernel
		double x = i - radius;
		// calculate the index for the corresponding x value in the lookup table
		int index = round(x * LOOKUP_TABLE_SIZE);
		if (index < 0) index = 0;
		if (index >= LOOKUP_TABLE_SIZE) index = LOOKUP_TABLE_SIZE - 1;
		// use the precomputed Gaussian function value from the lookup table
		kernel[i] = lookup_table[radius][index];
		// update the sum of all kernel values
		sum += kernel[i];
	}

	// normalize the kernel values so that they sum up to 1
	for (int i = 0; i < kernel_size; i++) {
		kernel[i] /= sum;
	}

	// return the computed kernel values
	return kernel;
}

void GaussianBlur::applyBlurMultiThread(vector<Pixel> & pixels, TGAImageHeader & header, const vector<double> & kernel, const int& radius)
{
	// Create a new vector to store the result of the blurring operation
	vector<Pixel> result(pixels.size());

	// Determine the number of threads to use based on the hardware concurrency
	int num_threads = thread::hardware_concurrency();

	// Calculate the height per thread (assuming equal distribution of work)
	int height_per_thread = header.height / num_threads;

	// Create a vector to hold the thread objects
	vector<thread> threads(num_threads);

	// For each thread, create a lambda function that performs the blurring operation on a subset of the image
	for (int t = 0; t < num_threads; t++) {
		threads[t] = thread([&pixels, &result, &header, &kernel, radius, height_per_thread, t, num_threads]() {
			// Calculate the starting and ending heights for this thread's work
			int width = header.width;
			int height_start = height_per_thread * t;
			int height_end = (t == num_threads - 1) ? header.height : height_start + height_per_thread;

			// Iterate over each pixel within this thread's work area
			for (int y = height_start; y < height_end; y++) {
				for (int x = 0; x < width; x++) {
					double r_acc = 0, g_acc = 0, b_acc = 0, a_acc = 0, w_acc = 0;

					// Iterate over each pixel in the kernel's radius around the current pixel
					for (int i = -radius; i <= radius; i++) {
						for (int j = -radius; j <= radius; j++) {
							int nx = x + j;
							int ny = y + i;

							// Ignore pixels that are outside the image bounds
							if (nx < 0 || nx >= width || ny < 0 || ny >= header.height) {
								continue;
							}

							// Calculate the weight for this pixel based on the kernel value
							double w = kernel[j + radius] * kernel[i + radius];

							// Retrieve the pixel data from the image
							Pixel pixel = pixels[ny * width + nx];

							// Update the accumulated color and weight values
							r_acc += w * pixel.r;
							g_acc += w * pixel.g;
							b_acc += w * pixel.b;
							a_acc += w * pixel.a;
							w_acc += w;
						}
					}

					// Calculate the blurred pixel values and store them in the result vector
					Pixel result_pixel;
					result_pixel.r = round(r_acc / w_acc);
					result_pixel.g = round(g_acc / w_acc);
					result_pixel.b = round(b_acc / w_acc);
					result_pixel.a = round(a_acc / w_acc);
					result[y * width + x] = result_pixel;
				}
			}
			});
	}

	// Wait for all threads to finish and join the results
	for (auto& thread : threads) {
		thread.join();
	}

	// Update the image header and pixel data with the result of the blurring operation
	pixels = result;
	header.bits_per_pixel = 32;
}