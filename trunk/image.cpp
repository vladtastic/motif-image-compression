#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cmath>


//BITMAP FILE STRUCTURES
#pragma pack(push,1)
typedef struct bitmap_file_header
{
	uint16_t bfType; //Specifies the file type
	uint32_t bfSize; //specifies the size in bytes of the bitmap file
	uint16_t bfReserved1; //reserved : must be 0
	uint16_t bfReserved2; //reserved : must be 0
	uint32_t bfOffBits; //specifies the offset in bytes from the bitmap file header to the bitmap data
}bitmap_file_header;


typedef struct bitmap_core
{

	uint32_t biSize; //specifies the number of bytes required by the struct
	int32_t biWidth; //specifies the width in pixels
	int32_t biHeight; //specifies the height in pixels
	uint16_t biPlanes; //specifies the number of colour planes; must be 1
	uint16_t biBitCount; //specifies the number of bits per pixel
	uint32_t biCompression;//specifies the type of compression
	uint32_t biSizeImage; //size of the image in bytes
	int32_t biXPelsPerMeter; //number of pixels per meter in X axis
	int32_t biYPelsPerMeter; //number of pixels per meter in Y axis
	uint32_t biClrUsed; //number of colours used by the bitmap
	uint32_t biClrImportant; //number of colours that are important
	

}bitmap_core;

#pragma pack(pop)


//Function to load the BITMAP file
int  *LoadBmp(char *filename,bitmap_core *image_info)
{
	FILE *fp; //our file pointer
	bitmap_file_header image_header; //our bitmap file header
	int *Image; //store the image data
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

