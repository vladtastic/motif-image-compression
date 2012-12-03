#ifndef TEST_H
#define TEST_H

#include "Bitmap.h"
#include "Image.h"
#include "Motif.h"



//	TEST ROUTINES FOR IMAGE CLASS

	Image image_constructor();
	Image pixel_array_to_image( int* pixel_array, struct bitmap_core bmpc );
	int bite_test( Image test_image, unsigned int x, unsigned int y );

//	TEST ROUTINES FOR MOTIF CLASS


#endif
