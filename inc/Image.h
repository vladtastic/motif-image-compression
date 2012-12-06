#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <utility>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Bitmap.h"

using namespace std;

class Image
{
	private:

	public:
		uint8_t** p;
		unsigned int WIDTH;
		unsigned int HEIGHT;
		bitmap_core image_info;

		Image();
		Image( unsigned int, unsigned int );
		Image( char* filename);
		~Image();
		Image Bite(unsigned int, unsigned int);
		unsigned int gHeight();
		unsigned int gWidth();

};


#endif
