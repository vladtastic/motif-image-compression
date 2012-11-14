#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Image.h"
#include "Motif.h"


int main( int argc, char* argv[] ){

	
	int err = 0;

	
	if( argc < 2 ){
		
		printf("Usage: executable /path/to/image \n" );
		return 0;

	}
	

	assert( argv[1] != NULL );



	//	Process the bitmap


	struct bitmap_core*  core_info;

	int* bitmap_ptr = LoadBmp( argv[1], core_info );

		
	
	
		
