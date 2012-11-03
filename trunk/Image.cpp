//Definitions for Image Class
#include "Image.h"

//Constructor
Image::Image(unsigned int m, unsigned int n)
{}

//Destructor
Image::~Image()
{}

//Reads and initializes image with the given file name
void Image::Read(char* filename)
{}

//Outputs bite of the image
Image Image::Bite( unsigned int x, unsigned int y, unsigned image_nRow, unsigned image_nCol )
{

	for( int i = 0; i < image_nRow; i++ ){
		for( int j = 0; j < image_nCol; j++ ){
	
			bite_colSize = image_nCol - j;
			bite_rowSize = image_nRow - i;
	
	
			bite* current_bite  = (bite*) malloc( bite_rowSize * bite_colSize * sizeof(int) );

			for( int k = row_pos; k < nRow; k++ ){
				for( int m = col_pos; m < nCol; m++ ){

					current_bite[ k * bite_colSize + m ] = image[ k * bite_colSize + m ];

				}
			}

			

			//	Add this bite into the array of bites
			
			bite_list.push_back( current_bite );

		}
	}
}

