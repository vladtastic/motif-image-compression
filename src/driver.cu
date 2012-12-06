#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "Bitmap.h"
#include "Image.h"
#include "timer.h"
//#include "cuda_utils.h"

#define ALPHABET_SIZE 128
#define MAX_THREADS_PER_BLOCK 1024
#define min(a,b) (((a) < (b)) ? (a) : (b)) 

__global__ void init_precompute( uint8_t* precompute, uint8_t* pixel_row, int* prop );

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage : bin/exec filename\n");
		return 0;
	}
	char* filename = argv[1];
	Image I(filename);

	struct timeval t0, t1;
	double elapsed = 0.0;

	gettimeofday( &t0, NULL );	
	
	int image_width = I.WIDTH;
	int image_height = I.HEIGHT;
	int image_size = image_height * image_width;

	int NUM_THREADS = min( MAX_THREADS_PER_BLOCK , image_width );
	int BLOCK_CHUNK = image_width / MAX_THREADS_PER_BLOCK;
	int NUM_BLOCKS  = ALPHABET_SIZE * BLOCK_CHUNK;

	int prop[4];

	prop[0] = image_width / MAX_THREADS_PER_BLOCK;
	prop[1] = image_size;
	prop[2] = 0;
	prop[3] = MAX_THREADS_PER_BLOCK;

	// HOST POINTERS
	uint8_t* precompute = (uint8_t*) malloc( ALPHABET_SIZE * image_size * sizeof(uint8_t) );

	// DEVICE POINTERS
	uint8_t* dev_precompute;
	uint8_t* dev_pixel_row;

	
	
		// MAKE SPACE ON GPU
		
		cudaMalloc( &dev_precompute, sizeof(uint8_t) * ALPHABET_SIZE * image_size  );
		cudaMalloc( &dev_pixel_row, sizeof(uint8_t) * image_width );


		
		// INIT_PRECOMPUTE
		
		for( int i = 0; i < image_height; i++ ){

			prop[2] = i;

			uint8_t* pixel_row = (uint8_t*) malloc( sizeof(uint8_t) * image_width );

			for( int j = 0; j < image_width; j++ ){

				pixel_row[j] = I.p[ i * image_width + j ];

			}


			// COPY FROM CPU TO GPU

			cudaMemcpy( &dev_pixel_row, pixel_row, sizeof( uint8_t ) * image_size, cudaMemcpyHostToDevice );

			
			// KERNEL INVOCATION
			init_precompute<<<NUM_BLOCKS, NUM_THREADS>>>( precompute, pixel_row, prop );

			cudaThreadSynchronize();

	
			// COPY FROM GPU TO CPU

			cudaMemcpy( &precompute[ i * image_size ], &dev_precompute, sizeof( uint8_t ) * ALPHABET_SIZE * image_size, cudaMemcpyDeviceToHost );
	

		}


	
	gettimeofday( &t1, NULL );
	
	elapsed = ( t1.tv_sec - t0.tv_sec ) + ( ( t1.tv_usec - t0.tv_usec ) / 1e6 );

	printf("Timer: %lf \n", elapsed );

	
	return 0 ;	
}
