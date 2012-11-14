#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include "Bitmap.h"


//Function to load the BITMAP file
int  *LoadBmp(char *filename,bitmap_core *image_info)
{
	FILE *fp; //our file pointer
	bitmap_file_header image_header; //our bitmap file header
	int *Image; //store the image data
	unsigned int *pal;
	
	//Open the filename in read binary mode
	if((fp = fopen(filename,"rb")) == NULL)
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
	
	//Calculate image parameters
	uint32_t RowSize = ((image_info->biBitCount * image_info->biWidth + 31)/32)*4;
	uint32_t PixelArraySize = RowSize * abs(image_info->biHeight);


	//Allocate memory for the array that will hold the pixel values
	Image = (int * )malloc(PixelArraySize);
	
	if(Image ==NULL)
	{
		free (Image);
		printf("Could not allocate memory for the image array\n");
		fclose(fp);
		return NULL;
	}
	//Read the pixel values into the array
	fread(Image,PixelArraySize,1,fp);
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

