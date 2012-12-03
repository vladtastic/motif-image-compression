#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

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


uint8_t *LoadBmp(char *filename, bitmap_core *image_info );


#endif
