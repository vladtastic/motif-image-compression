#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Bitmap.h"
#include "Image.h"
#include "Motif.h"
#include "Test.h"


int main( int argc, char* argv[] ){

	
	if( argc < 2 ){

		printf("Usage: executable filename.bmp \n");
		return 1;

	}


	int err = 0;
	struct bitmap_core bmpc;

	//	RETAIN LOADBMP TO GATHER IMAGE SIZE DIMENSIONS
	//	RETURNS INCORRECT PIXEL ARRAY

	int* dummy; 
	dummy = LoadBmp( argv[1], &bmpc );


	printf("Width: %d\tHeight: %d\n", bmpc.biWidth, bmpc.biHeight );


	// construct pixel array
	int* pixel_array = (int*) malloc( bmpc.biWidth * bmpc.biHeight * sizeof(int) );

	if( pixel_array == NULL ){

		printf("Error allocating pixel_array\n");
		exit(1);

	}


	for( int i=0; i< bmpc.biWidth * bmpc.biHeight; i++ ){
		
		pixel_array[i] = 0;

	}


	FILE *fp = fopen("thresholdpixelmatrix.txt", "r" );

	if( fp == NULL ){

		printf("Error opening pixel matrix file!\n");
		exit(1);

	}

	for( int i = 0; i<bmpc.biHeight; i++){
		for( int j=0; j<bmpc.biWidth; j++){

			fscanf(fp, "%d\t", &pixel_array[ i * bmpc.biWidth + j ] );

		}

	}

	for( int i=0; i<10; i++){
		for( int j=0; j<10; j++){
			
			printf("%d\t", pixel_array[i*bmpc.biWidth+j]);

		}

		printf("\n");

	}
		

	Image test_image = image_constructor();

	
	//	TESTING PIX ARRAY TO IMAGE

	Image converted_image = pixel_array_to_image( pixel_array, bmpc );


	//	TESTING BITE CONSTRUCTION

	err = bite_test( converted_image, 6, 6 );

	if( err != 0 ){

		printf("Error in bite test!\n");

		return 1;

	}

	
	return 0;

}

