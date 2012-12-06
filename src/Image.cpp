//Definitions for Image Class
#include <malloc.h>
#include "Image.h"


//Constructor
Image::Image(unsigned int m, unsigned int n)
{

	WIDTH = m;
	HEIGHT = n;
	p = (uint8_t *)malloc(m * n * sizeof(uint8_t));	
}

//Destructor
Image::~Image()
{}


//Takes in array of pixel values, outputs corresponding Image object
Image::Image(char* filename){

	p = LoadBmp(filename, &image_info);
	
	WIDTH = image_info.biWidth;
	HEIGHT = image_info.biHeight;

}

	

//	Outputs bite of the image
Image Image::Bite( unsigned int x, unsigned int y )
{
	/*vector<int> tmp;

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


	return bite;*/
		
}

unsigned int Image::gHeight(){
	
	return this->HEIGHT;

}

unsigned int Image::gWidth(){

	return this->WIDTH;

}


