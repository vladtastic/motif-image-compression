//Definitions for Image Class
#include "Image.h"
#include <assert.h>

//Constructor
Image::Image(unsigned int m, unsigned int n)
{

	unsigned int WIDTH = m;
	unsigned int HEIGHT = n;

	vector<vector<int>> intensity;
		
}

//Destructor
Image::~Image()
{}

//Reads and initializes image with the given file name
void Image::Read(char* filename)
{}

//Outputs bite of the image
Image Image::Bite( unsigned int x, unsigned int y )
{
	assert( x > -1 );
	assert ( y > -1 );
	
	Image bite = Image();

	vector<int> tmp;

	for( int k = x; k < this.HEIGHT; k++ ){

		for( int m = y; m < this.WIDTH; m++ ){

			tmp.push_back( this.intensity[k][m] );

		}
	
		bite.push_back(tmp);

	}


	return bite;
		
}


//	Outputs meet of the image
Image Image::Meet(Image bite)
{
		
	Image meet = Image();

	vector<int> tmp;

	for( int i=0; i < bite.HEIGHT; i++ ){
		for( int j=0; bite.WIDTH; j++ ){
			
			if( this.intensity[i][j] == bite.intensity[i][j] ){
				
				tmp.push_back( bite.intensity[i][j] );
			
			}

			else{

				tmp.push_back( -1 );

			}

		}

	
		meet.push_back(tmp);

	}

	
	return meet;

}
