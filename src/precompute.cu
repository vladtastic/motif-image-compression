#include <stdlib.h>
#include <stdint.h>
//#include "cuda_utils.h"

#define ALPHABET_SIZE 		128
#define MAX_THREADS_PER_BLOCK	1024
#define min(a,b) (((a) < (b)) ? (a) : (b))

__global__ void init_precompute( uint8_t* precompute, uint8_t* pixel_row, int* prop )
{

	// pack BLOCK_CHUNK, image_size, current_row_num and MAX_THREADS_PER_BLOCK in an int* array

	int BLOCK_CHUNK = prop[0];
	int image_size = prop[1];
	int current_row_num = prop[2];
	int mtpb = prop[3];

	uint8_t pix_val = pixel_row[ ( blockIdx.x % BLOCK_CHUNK ) * blockDim.x + threadIdx.x ];

	int my_letter = blockIdx.x / BLOCK_CHUNK;
	

		precompute[ ( ( blockIdx.x / BLOCK_CHUNK ) * image_size + current_row_num ) + threadIdx.x + ( ( blockIdx.x % BLOCK_CHUNK ) * mtpb ) ] = 1;


	if( pix_val == (uint8_t) my_letter ){

		precompute[ ( ( blockIdx.x / BLOCK_CHUNK ) * image_size + current_row_num ) + threadIdx.x + ( ( blockIdx.x % BLOCK_CHUNK ) * mtpb ) ] = 0;

	}
}
	
