#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Image.h"
#include "Bitmap.h"


Image image_constructor(){

	// NEW IMAGE OBJ OF SIZE m*n

	unsigned int randH = ( rand() % 10) + 1 ;
	unsigned int randW = ( rand() % 10) + 1 ;

	Image test_image2 = Image( randW, randH );

	unsigned int height = test_image2.gHeight();
	unsigned int width = test_image2.gWidth();

	if( height != randH ){

		printf("Height is incorrect! \n");

		exit(1);

	}


	if( width != randW ){

		printf("Width is incorrect! \n");

		exit(1);

	}
	
	
	return test_image2;

}



Image pixel_array_to_image( int* pixel_array, struct bitmap_core bmpc ){

	Image test_image = Image( pixel_array, &bmpc );

	int width = bmpc.biWidth;
	int height = bmpc.biHeight;

	if( test_image.gHeight() != height ){
	
		printf("Error: Height mismatch!\n");
		exit(1);

	}

	
	if( test_image.gWidth() != width ){

		printf("Error: Width mismatch!\n");
		exit(1);

	}


	for( int i=0; i<height; i++){

		for( int j=0; j<width; j++){

			if( test_image.p[i][j] != pixel_array[ i*width + j ] ){

				printf("Unmatched Pixel Value!\n");
				exit(1);
			
			}

		}

	}


	return test_image;
				
}


int bite_test( Image test_image, unsigned int x, unsigned int y ){

	Image test_bite = test_image.Bite( x, y );

	int biteW, biteH, checkW, checkH;

	biteW = test_bite.WIDTH;
	biteH = test_bite.HEIGHT;

	checkW = test_image.WIDTH - x;
	checkH = test_image.HEIGHT - y;

	if( checkW != biteW ){

		printf(" Bite has incorrect width!\n");
		return 1;

	}


	if( checkH != biteH ){

		printf(" Bite has incorrect height!\n");
		return 1;

	}


	int temp1, temp2, err = 0;

	
	for( int i = (int) y; i < biteH; i++){

		for( int j = (int) x; j < biteW; j++){

			if( test_image.p[i][j] != test_bite.p[ i - (test_image.HEIGHT - test_bite.HEIGHT) ][ j - (test_image.WIDTH - test_bite.WIDTH) ] ){

				printf("Bite, Image value mismatch!\n");
				printf(" Error at %d %d \n", i, j );
				return 1;

			}

		}
	}

	
	return 0;

}

	
