#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


//BITMAP FILE STRUCTURES
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


//Function to load the BITMAP file
unsigned char  *LoadBmp(char *filename,bitmap_core *image_core_info)
{
	FILE *fp; //our file pointer
	bitmap_file_header image_file_header; //our bitmap file header
	unsigned char *Image; //store the image data
	
	//Open the filename in read binary mode
	if((fp = fopen(filename,"rb")) == NULL)
        {
                printf("Image cannot be opened!\nExiting\n");
                exit(EXIT_FAILURE);
        }
	
	//Read the bbitmap file header
	fread(&image_file_header,sizeof(bitmap_file_header),1,fp);
	printf("Type %u \n",image_file_header.bfType);
	printf("Size %u \n",image_file_header.bfSize);
	printf("Res1 %u \n",image_file_header.bfReserved1);
	printf("Res2 %u \n",image_file_header.bfReserved2);
	printf("OffBits %u \n",image_file_header.bfOffBits);
	//verify that this is a bmp file
	if (image_file_header.bfType != 0x4D42)
	{
		printf("This is not a bitmap file!\n");
		fclose(fp);
		return NULL;
	}	
	
	//Read the bitmap info header
	fread(image_core_info,sizeof(bitmap_core),1,fp);
	
	//Move the file pointer to the beginning of the data 
	fseek(fp,image_file_header.bfOffBits,SEEK_SET);	
	
	//Allocate enough memory for the bitmap image data
	Image = (unsigned char*)malloc(image_core_info->biSizeImage);
	
	printf("Size of image is %u \n",image_core_info->biSizeImage);
	
	//Verify that memory has been allocated
	if (Image ==NULL)
	{
		printf("Could not allocate memory for the image array\n");
		fclose(fp);
		return NULL;
	}
	
	//Read in the bitmap image data
	fread(Image,image_core_info->biSizeImage,1,fp);
	
	//Make sure the bitmap was read
	if(Image==NULL)
	{
		fclose(fp);
		return NULL;
	}
	
	fclose(fp);
	
	//Write the image back just to make sure
	FILE *op;
	op = fopen("new.bmp","wb");
	fwrite(&image_file_header,sizeof(bitmap_file_header),1,op);
	fwrite(image_core_info,sizeof(bitmap_core),1,op);
	fseek(op,image_file_header.bfOffBits,SEEK_SET);	
	fwrite(Image,image_core_info->biSizeImage,1,op);
	
	fclose(op);
	
	return Image;
}



int main(int argc ,char *argv[])
{
	bitmap_core image_info_header;
	unsigned char *image;
	char name[] = "duke.bmp";
	image = LoadBmp(name,&image_info_header);
	int i = 0;
	/*while(i < 300)
	{
		printf("%u %u %u \n",image[i],image[i+1],image[i+2]);
		i = i+3;
	}*/
	return 0;
}
