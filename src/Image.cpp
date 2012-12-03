//Definitions for Image Class
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Bitmap.h"
#include "Image.h"

//Constructor
Image::Image(unsigned int m, unsigned int n)
{

	WIDTH = m;
	HEIGHT = n;

	vector< vector<int> > p;
		
}

//Destructor
Image::~Image()
{}

//Reads and initializes image with the given file name
void Image::Read(char* filename)
{}

//Takes in array of pixel values, outputs corresponding Image object
Image::Image( int* pix_val, struct bitmap_core* bcore ){

	assert( pix_val != 0);
	assert( bcore != 0);	

	int dimW = bcore->biWidth;
	int dimH = bcore->biHeight;

	assert( dimW > 0 );
	assert( dimH > 0 );

	vector<int> tmp;

	this->WIDTH = dimW;
	this->HEIGHT = dimH;

	for( int i=0; i<dimH; i++){
		for( int j=0; j<dimW; j++){

			tmp.push_back( pix_val[i*dimW + j] );
		}

		this->p.push_back(tmp);

		tmp.clear();

	}

}

	

//	Outputs bite of the image
Image Image::Bite( unsigned int x, unsigned int y )
{
	vector<int> tmp;

	unsigned int bite_width = this->WIDTH -  x;
	unsigned int bite_height = this->HEIGHT - y;

	Image bite = Image(bite_width, bite_height);

	for( unsigned int k = y; k < this->HEIGHT; k++ ){

		for( unsigned int m = x; m < this->WIDTH; m++ ){

			tmp.push_back( this->p[k][m] );

		}
	
		bite.p.push_back(tmp);
		tmp.clear();

	}


	return bite;
		
}

unsigned int Image::gHeight(){
	
	return this->HEIGHT;

}

unsigned int Image::gWidth(){

	return this->WIDTH;

}


