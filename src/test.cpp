#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include "Bitmap.h"

using namespace std;

int main()
{
	uint8_t * Image;
	char filename[] = "data/lena.bmp";
	
	bitmap_core image_info;
	
	Image = LoadBmp(filename,&image_info);
	
	int width = image_info.biWidth;
	int height = image_info.biHeight;
	
	for(int i = 0 ; i < height ; i++)
	{
		for(int j = 0 ; j < width ; j++)
		{
			cout << Image[i*width+j] << " ";
		}
		cout << "\n";
	}
	
	//Print out the image
	int j = 0;
	
	cout << "Height = " << height << endl;
	cout << "Width = " << width << endl;
	cout <<"BPP = " << image_info.biBitCount << endl;
	
	free(Image);
}
