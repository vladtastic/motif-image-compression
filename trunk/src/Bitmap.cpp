#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include "Bitmap.h"


//Function to load the BITMAP file
uint8_t  *LoadBmp(char *filename,bitmap_core *image_info)
{
	FILE *fp; //our file pointer
	bitmap_file_header image_header; //our bitmap file header
	uint8_t *Image; //store the image data
	unsigned int *pal;
	
	//Open the filename in read binary mode
	if((fp = fopen(filename,"r")) == NULL)
        {
                printf("Image cannot be opened!\nExiting\n");
                exit(EXIT_FAILURE);
        }
	
	
	
	//Read the bbitmap file header
	fread(&image_header,sizeof(bitmap_file_header),1,fp);



	//verify that this is a bmp file
	if (image_header.bfType != 0x4d42)
	{
		printf("This is not a bitmap file!\n");
		fclose(fp);
		return NULL;
	}
	
	//Read in the bitmap core info
	fread(image_info,sizeof(bitmap_core),1,fp);

	//If it isn't an 8-bit bitmap image, then throw error and exit
	if(image_info -> biBitCount != 8)
	{
		std::cout << "Not an 8 Bit Bitmap image.\nPlease use 8 Bit images only.\n";
		exit(EXIT_FAILURE);
	}
	
	//Calculate image parameters
	uint32_t RowSize = ((image_info->biBitCount * image_info->biWidth + 31)/32)*4;
	
	uint32_t PixelArraySize = RowSize * abs(image_info->biHeight);
	 


	//Allocate memory for the array that will hold the pixel values
	int height = image_info->biHeight;
	int width = image_info->biWidth;
	Image = (uint8_t * )malloc(height * width * sizeof(uint8_t));
	
	if(Image ==NULL)
	{
		free (Image);
		printf("Could not allocate memory for the image array\n");
		fclose(fp);
		return NULL;
	}
	//Read the pixel values into the array

	fseek(fp , image_header.bfOffBits , SEEK_SET);	
	
	//First check how many padded bytes there are
	int numPadded = width % 4; //Each row is packed on a 4 byte boundary
	
	for(int i = 0 ; i < height ; i++)
	{
		for(int j = 0; j < width ; j++)
		{
			uint8_t p;
			fread(&p , 1 , 1 , fp);
			p = p >> 1; //Right shift by one bit to make it into a 7bit pixel depth. So that the 8th bit can be used for don't care
			int inverted = (height - 1) - i;
			Image[inverted * width + j] = p;
			
			//Skip over the padded bytes
			if(numPadded != 0)
			{
				uint8_t skip[4];
				fread(skip , 1 , 4-numPadded , fp);
			} 
		}
	}
	//Read the palette if it exists
	if(image_info->biBitCount <= 8)
	{
		pal = (unsigned int*)malloc(4 * pow((double)2,(double)image_info->biBitCount));
		fread(pal,4 * pow((double)2,(double)image_info->biBitCount),1,fp);
	}

	//Close the file	
	fclose(fp);

	return Image;
}

