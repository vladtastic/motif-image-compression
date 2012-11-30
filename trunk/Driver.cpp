#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <queue>
#include <map>

#include "Bitmap.h"
#include "Image.h"
#include "Motif.h"

int* load_pixel_matrix( char*, struct bitmap_core* );

int main( int argc, char* argv[] ){

	
	int err = 0;

	
	if( argc < 2 ){
		
		printf("Usage: executable /path/to/image \n" );
		return 0;

	}
	

	assert( argv[1] != NULL );



	// bmp to Image object

	struct bitmap_core*  core_info = (struct bitmap_core*) malloc( sizeof(struct bitmap_core) );;

		
		
	int* bitmap_ptr = LoadBmp( argv[1], core_info );
	free(bitmap_ptr);

	//	LOADBMP RETURNS INCORRECT PIXEL MATRIX
	//	LOAD_PIXEL_MATRIX USED AS SUBSTITUTION
	//	LOADBMP USED TO FIND IMAGE DIMENSIONS

	int dimR = core_info->biHeight;
	int dimC = core_info->biWidth;

	int* pixel_array  = load_pixel_matrix( "thresholdpixelmatrix.txt" , core_info );
	
	if( pixel_array == NULL ){

		printf("Error loading pixel array!\n");
		return 1;

	}

	Image current_image = Image( pixel_array, core_info );	
	
	int num_bites = dimR * dimC;

	map< string, vector<struct coords> > motif_hash_list; 

	for( int i = 0; i < dimR; i++ ){

		for( int j = 0; j < dimC; j++ ){

			Image current_bite = current_image.Bite( (unsigned int) j, (unsigned int) i );	// how does this function work?
													// Also, on next loop iter, does it clear mem from prev var?

			Motif current_autoc = Motif();

			current_autoc.Meet( current_image, current_bite ); // this is autocor													//	This should return the AUTOCORRELATION


			//	Hash current_autoc
			//	Add autoc to a list for writing/saving
			
			motif_hash_list[ current_autoc.string_representation() ] = current_autoc.occurrences;


			if( ( i * dimC + j ) % 100 == 0 ){

				// Write data to disk

			}

		}

	}


	// Decompose any maximal motifs...



	return 0;

}


int* load_pixel_matrix( char* filename, struct bitmap_core* bmpc ){

	FILE *fp;

	fp  = fopen(filename, "r" );

	if( fp == NULL ){

		printf("Error opening pixel matrix file!\n");
		return (int*) NULL;

	}

	int* pixel_array = (int*) malloc( bmpc->biHeight * bmpc->biWidth * sizeof( int) );

	if( pixel_array == NULL ){

		printf("Error Allocating pixel value matrix!\n");
		return (int*) NULL;

	}

	for( int i = 0; i<bmpc->biHeight; i++){
		for( int j=0; j<bmpc->biWidth; j++){

			fscanf(fp, "%d\t", &(pixel_array[ i * bmpc->biWidth + j ]) );

		}

	}

	
	fclose(fp);


	return pixel_array;

}
