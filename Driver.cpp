#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include <queue>

#include "Bitmap.h"
#include "Image.h"
#include "Motif.h"


int main( int argc, char* argv[] ){

	
	int err = 0;

	
	if( argc < 2 ){
		
		printf("Usage: executable /path/to/image \n" );
		return 0;

	}
	

	assert( argv[1] != NULL );



	// bmp to Image object

	struct bitmap_core*  core_info;

	int* bitmap_ptr = LoadBmp( argv[1], core_info );

	//!!	core_info has height, are pixel array dims biWidth * biHeight?

	int dimR = core_info->biHeight;
	int dimC = core_info->biWidth;

	Image current_img = Image( bitmap_ptr, core_info );	
	
	int num_bites = dimR * dimC;

	


	//	BEGIN THREADED REGION

	#pragma omp parallel
	{

		int thread_id = omp_get_thread_num();
		int num_threads = omp_get_num_threads();
		int chunk_size = num_bites / num_threads;
		int remainder = num_bites % num_threads;

		queue<Image> bite_list;

		#pragma omp for
		
			/*
				An Image has mn bites
				Each thread takes a chunk (subset)
				Each thread gets coords of each bite in subset
				Adds bite to its personal queue			
			*/
					
			
			for( int i = thread_id * chunk_size; i < (thread_id + 1 ) * chunk_size; i++ ){

				int pos_row = i % dimR;
				int pos_col = i % dimC;

				bite_list.push( current_img.Bite( pos_row, pos_col ) );

			}


			
			// Last thread takes the leftovers
	
			if( thread_id == num_threads - 1 ){
				
				for( int i = (thread_id + 1 ) * chunk_size; i < ((thread_id + 1) * chunk_size) + remainder; i++ ){

					int pos_row = i % dimR;
					int pos_col = i % dimC;

					bite_list.push( current_img.Bite( pos_row, pos_col ) );
				
				}

			}




		// Each thread has a subset of autocorrelations
		queue<Motif> meet_list;

		if( thread_id == num_threads - 1 ){
			
			chunk_size = chunk_size + remainder;
		
		}

		#pragma omp for

			for( int i = chunk_size; i > 0; i-- ){
		
				Motif tmp = Motif();
				tmp.Meet( current_img, bite_list.front() );
				meet_list.push( tmp );
				bite_list.pop();

			}

	}				



	return 0;

}
