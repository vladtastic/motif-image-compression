//Definitions for Image Class
#include <malloc.h>
#include "Image.h"


//Constructor
Image::Image(unsigned int m, unsigned int n)
{

	WIDTH = m;
	HEIGHT = n;

	p = (uint8_t **) malloc(m * sizeof(uint8_t*));
	for(int i = 0 ; i < HEIGHT ; i++)
	{
		//Allocate space for the row
		p[i] = (uint8_t *)memalign(16,n * sizeof(uint8_t));
	}
		
}

//Destructor
Image::~Image()
{}


//Takes in array of pixel values, outputs corresponding Image object
Image::Image(char* filename){

	uint8_t *raw = LoadBmp(filename, &image_info);
	
	WIDTH = image_info.biWidth;
	HEIGHT = image_info.biHeight;
	
	p = (uint8_t **) malloc(HEIGHT * sizeof(uint8_t*));
	
	//fill in the image
	for(int i = 0 ; i < HEIGHT ; i++)
	{
		//Allocate space for the row
		p[i] = (uint8_t *)memalign(16,WIDTH * sizeof(uint8_t));
		
		for(int j = 0 ; j < WIDTH ; j++)
		{
			p[i][j] = raw[i * WIDTH + j];
		}
	}
	
	free(raw);

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


