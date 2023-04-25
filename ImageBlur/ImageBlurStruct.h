#pragma once

namespace MyImageBlurStruct 
{
// Disable struct padding
	
	struct Pixel
	{
		unsigned char b, g, r, a;
		// Constructor with optional alpha value
		Pixel() : b(0), g(0), r(0), a(255){}
		Pixel(unsigned char b_, unsigned char g_, unsigned char r_, unsigned char a_ = 255)
			: b(b_), g(g_), r(r_), a(a_) {}
	};

#pragma pack(push, 1) 
	struct TGAImageHeader  {
		char id_length;
		char color_map_type;
		char image_type;
		short color_map_start;
		short color_map_length;
		char color_map_depth;
		short x_origin;
		short y_origin;
		short width;
		short height;
		char bits_per_pixel;
		char image_descriptor;

		TGAImageHeader() :id_length(0),color_map_type(0), image_type(0),color_map_start(0),
			color_map_length(0), color_map_depth(0), x_origin(0), y_origin(0),
			width(0),height(0),bits_per_pixel(0),image_descriptor(0) {}
	};
#pragma pack(pop)
	// Re-enable struct padding
}
