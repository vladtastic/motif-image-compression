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

//Takes in array of pixel values, outputs corresponding Image object
Image::Image( int* pix_val, struct bitmap_core* bcore ){

	assert( pix_val != NULL);
	assert( bcore != NULL);	

	int dimW = bcore.biWidth;
	int dimH = bcore.biHeight;

	assert( dimW > 0 );
	assert( dimH > 0 );

	vector<int> tmp;

	Image cur = Image();
	
	cur.WIDTH = dimW;
	cur.HEIGHT = dimH;

	
	for( int i=0; i<dimH; i++){
		for( int j=0; j<dimW; j++){

			tmp.push_back( pix_val[i][j] );
		}

		cur.intensity.push_back(tmp);

		tmp.clear();

	}
	
	~tmp;

	return cur;

}

	

//	Outputs bite of the image
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
