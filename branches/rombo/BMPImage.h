/*==========================================================================
                2D-IME (2D Irredundant Motif Extraction)
                   http://siloe.deis.unical.it/2D-IME
 ============================================================================
  Copyright (C) 2009


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.


  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
*/

#ifndef BMPIMAGE_H_
#define BMPIMAGE_H_

#include <iostream>
#include <cstdlib>

using namespace std;

//TODO not used
///Stores the definition of the colors being used for indexed color bitmaps.
typedef unsigned char PALETTE[256][4];

///Memory structure clone of BMP header image file.
typedef struct {
	char id[2]; ///<Used to identify the BMP file: 0x42 0x4D (Hex code points for B and M).
	unsigned int fileSize;///<The size of the BMP file in bytes.
	short int reserved[2];///<Reserved; actual value depends on the application that creates the image.
	unsigned int headerSize;///<The offset, i.e. starting address, of the byte where the bitmap data can be found.
	unsigned int infoSize;///<The size of this header (40 bytes).
	unsigned int width;///<The bitmap width in pixels (signed integer).
	unsigned int heigth;///<The bitmap height in pixels (signed integer).
	short int biPlanes;///<The number of color planes being used. Must be set to 1.
	short int colorDepth;///<The number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
	unsigned int compression;///<The compression method being used. See the next table for a list of possible values.
	unsigned int imageSize;///<The image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.

	int horRes;///<The horizontal resolution of the image. (pixel per meter, signed integer).
	int verRes;///<The vertical resolution of the image. (pixel per meter, signed integer).

	unsigned int colorsNumber; ///<The number of colors in the color palette, or 0 to default to 2n.
	unsigned int importantColor; ///<The number of important colors used, or 0 when every color is important; generally ignored.
}__attribute__((__packed__)) BMPHEADERINFO;

///Memory structure clone of BMP image file.
typedef struct {
	BMPHEADERINFO bmpHeader;///<Stores general information about the BMP file.
	//PALETTE pal;Stores the definition of the colors being used for indexed color bitmaps.
	unsigned int palSize;///<Stores the size of Palette.
	void *imgData;///<Stores the actual image, pixel by pixel.
	unsigned int bpp; ///<Bytes per pixel.
} FBMP;

/*! \class BMPImage
 *  \brief Reading/writing class for reading, writing and modifying BMP image file
 *  \author Michele Mastroianni
 *  \mail michele.mastroianni@gmail.com
 *  \version 0.1
 *  \date    25-mar-2009
 *  \warning The class can be used only for 24 bit BMP Windows bitmap without Palette
 */
class BMPImage {
private:
	///Pointer to the memory structure clone of BMP image file.
	FBMP* fBmpStr;
	///Pointer to the raw pixel map of image.
	void* imm;
	///Pointer to the image file on disk.
	FILE *fBmp;
	///Image file name.
	const char *fBmpName;
	///Computes image size in byte using height and width.
	/*!
	 * \param h height of image.
	 * \param w width of image.
	 * \return size in byte of BMP image.
	 */
	int compute_img_size(int h, int w) {
		int dim_row = w * 3;
		int resto = dim_row % 4;
		int pad = 4 - resto;
		if (resto == 0)
			pad = 0;
		int size = (dim_row + pad) * h;
		return size;
	}
	///Used to set image dimension in BMP header.
	/*!
	 * \param h height of image.
	 * \param w width of image.
	 */
	void set_img_dimension(unsigned long h, unsigned long w) {
		fBmpStr->bmpHeader.heigth = h;
		fBmpStr->bmpHeader.width = w;
		fBmpStr->bmpHeader.imageSize = compute_img_size(h, w);
		fBmpStr->bmpHeader.fileSize = fBmpStr->bmpHeader.imageSize + 54;
	}
	void* get_BMP_imageData(int h, int w, int** A);
	void set_imageData(void* data, int h, int w);

public:
	const static int WHITE_INT = 0xFFFFFF; ///< RGB code for white pixel.
	const static int BLACK_INT = 0x000000; ///< RGB code for black pixel.
	BMPImage(const char *fBmpName);
	~BMPImage();
	///Reads BMP header heigth value.
	/*!
	 * \return image heigth.
	 */
	int get_h() {
		return fBmpStr->bmpHeader.heigth;
	}
	///Reads BMP header width value.
	/*!
	 * \return image width.
	 */
	int get_w() {
		return fBmpStr->bmpHeader.width;
	}
	int** read_int();
	void set_int(int** A, int h, int w);
	void printBMPHEADERINFO();
	void writeBmpFile();

};

///Destructor.
BMPImage::~BMPImage() {
	delete &(fBmpStr->bmpHeader.id);
	free(fBmpStr->imgData);
}

///Constructor.
/*!
 * Creates the specified image file if it doesn't exist. \n
 * Reads in memory the image if the file exists.
 * \param fBmpName image file name.
 */
BMPImage::BMPImage(const char *fBmpName) {
	BMPImage::fBmpName = fBmpName;
	fBmpStr = new FBMP();

	fBmp = fopen(fBmpName, "rb+");
	if (fBmp == NULL) {
		fBmp = fopen(fBmpName, "wb+");

		if (fBmp == NULL) {
			printf("BMPImage::BMPImage::Error in creation\n");
		} else {
			fBmpStr->bmpHeader.id[0] = 0x42;
			fBmpStr->bmpHeader.id[1] = 0x4D;
			fBmpStr->bmpHeader.fileSize = 58;
			fBmpStr->bmpHeader.reserved[0] = 0;
			fBmpStr->bmpHeader.reserved[1] = 0;
			fBmpStr->bmpHeader.headerSize = 0x36;
			fBmpStr->bmpHeader.infoSize = 0x28;
			fBmpStr->bmpHeader.colorDepth = 24;
			fBmpStr->bmpHeader.compression = 0;
			fBmpStr->bmpHeader.biPlanes = 1;
			fBmpStr->bmpHeader.colorsNumber = 0;
			fBmpStr->bmpHeader.importantColor = 0;

			fwrite(&fBmpStr->bmpHeader, sizeof(BMPHEADERINFO), 1, fBmp);
			fclose(fBmp);

		}
	} else {

		fread(&fBmpStr->bmpHeader, sizeof(BMPHEADERINFO), 1, fBmp);
		imm = new void*[fBmpStr->bmpHeader.imageSize];
		fBmpStr->palSize = fBmpStr->bmpHeader.headerSize
				- fBmpStr->bmpHeader.infoSize - 14;

		fseek(fBmp, fBmpStr->bmpHeader.headerSize, SEEK_SET);
		fread(imm, fBmpStr->bmpHeader.imageSize, 1, fBmp);

		fBmpStr->imgData = imm;
		fclose(fBmp);
		switch (fBmpStr->bmpHeader.colorDepth) {
		case 8:
			fBmpStr->bpp = 1;
			break;
		case 24:
			fBmpStr->bpp = 3;
			break;
		}
	}
}

///Prints on standard output the BMP image file header.
void BMPImage::printBMPHEADERINFO() {
	BMPHEADERINFO *bmpHeader = &fBmpStr->bmpHeader;
	printf("%10s: %u\n", "fileSize", bmpHeader->fileSize);
	printf("%10s: %d\n", "reserved", bmpHeader->reserved[0]);
	printf("%10s: %d\n", "reserved", bmpHeader->reserved[1]);
	printf("%10s: %u\n", "HeaderSize", bmpHeader->headerSize);
	printf("%10s: %u\n", "width", bmpHeader->width);
	printf("%10s: %u\n", "heigth", bmpHeader->heigth);
	printf("%10s: %d\n", "biPlanes", bmpHeader->biPlanes);
	printf("%10s: %d\n", "colorDepth", bmpHeader->colorDepth);
	printf("%10s: %u\n", "compression", bmpHeader->compression);
	printf("%10s: %u\n", "imageSize", bmpHeader->imageSize);
	printf("%10s: %i\n", "horRes", bmpHeader->horRes);
	printf("%10s: %i\n", "verRes", bmpHeader->verRes);
	printf("%10s: %u\n", "colorsNumber", bmpHeader->colorsNumber);
	printf("%10s: %u\n", "importantColor", bmpHeader->importantColor);
}

///Translates the related BMP image file into an array of int.
int** BMPImage::read_int() {
	int h = fBmpStr->bmpHeader.heigth;
	int w = fBmpStr->bmpHeader.width;

	unsigned char *imm = static_cast<unsigned char *> (fBmpStr->imgData);
	int i = 0;
	int ii = 0;
	int jj = 0;

	int** A = new int *[h];

	while (ii < h) {
		jj = 0;
		A[ii] = new int[w];
		while (jj < w) {
			{
				int blue = i++;
				int green = i++;
				int red = i++;

				int bgr = 0x0000;

				bgr = (0 & 0xF) << 24;
				bgr |= (imm[blue] & 0xFF) << 16; // 0xF == 15 == 1111 binary
				bgr |= (imm[green] & 0xFF) << 8;
				bgr |= (imm[red] & 0xFF) << 0;
				A[ii][jj] = bgr;

			}
			jj++;
		}
		int pad = i;
		int resto = pad % 4;
		if(resto != 0)
			resto = 4-resto;
		while (resto > 0) {
			pad = i++; // pad to the end line
			resto--;
		}

		ii++;
	}

	//reverse
int** image_rev=new int*[h];
	for (int i = 0; i < h; i++) {
	   			image_rev[i] = new int[w];

   			}

for(int i=h-1;i>=0;i--)
	for(int j=0;j<w;j++)
		image_rev[h-1-i][j]=A[i][j];


	return image_rev;
}

///Setting of related BMP image by 2D raw pixel map.
/*!
 * \param A 2D int array of raw pixel map.
 * \param h height of raw pixel map.
 * \param w width of raw pixel map.
 */
void BMPImage::set_int(int** A, int h, int w) {

	//reverse
	int** image_rev=new int*[h];
		for (int i = 0; i < h; i++) {
		   			image_rev[i] = new int[w];
	   			}

	for(int i=h-1;i>=0;i--)
		for(int j=0;j<w;j++)
			image_rev[h-1-i][j]=A[i][j];

	void * tmp = fBmpStr->imgData;
	fBmpStr->imgData = get_BMP_imageData(h, w, image_rev);
	set_img_dimension(h, w);
	free(tmp);
}

///Setting of related BMP image byte.
/*!
 * \param data pointer to image byte.
 * \param h height of image.
 * \param w width of image.
 */
void BMPImage::set_imageData(void* data, int h, int w) {
	void * tmp = fBmpStr->imgData;
	fBmpStr->imgData = data;
	set_img_dimension(h, w);
	free(tmp);
}

///Translates a raw pixel map into the related BMP image format.
/*!
 * \param h height of image.
 * \param w width of image.
 * \param A 2D int array of raw pixel map.
 */
void* BMPImage::get_BMP_imageData(int h, int w, int** A) {

	unsigned long size = compute_img_size(h, w);
	void* outData = new void*[size];
	unsigned char* out = static_cast<unsigned char *> (outData);

	int i = 0;
	int ii = 0;
	int jj = 0;

	while (ii < h) {
		jj = 0;
		int dw = 0;
		unsigned char byte3, byte2, byte1, byte0;
		while (jj < w) {

			int red = i++;
			int green = i++;
			int blue = i++;

			dw = A[ii][jj];
			byte0 = dw & 0xFF;
			byte1 = (dw >> 8) & 0xFF;
			byte2 = (dw >> 16) & 0xFF;
			byte3 = (dw >> 24) & 0xFF;

			out[red] = byte2;
			out[green] = byte1;
			out[blue] = byte0;

			jj++;
		}
		int pad = i;
		int resto = pad % 4;
		if(resto != 0)
			resto = 4-resto;
		while (resto > 0) {
			pad = i++;
			out[pad] = 0 & 0xFF;
			resto--;
		}

		ii++;
	}
	return outData;
}

///Writes on disk the clone of the relater image file.
void BMPImage::writeBmpFile() {
	fBmp = fopen(fBmpName, "wb+");
	if (fBmp == NULL) {
		printf("BMPImage::writeBmpFile::Error in creation\n");
	} else {
		fwrite(&fBmpStr->bmpHeader, sizeof(BMPHEADERINFO), 1, fBmp);
		fseek(fBmp, fBmpStr->bmpHeader.headerSize, SEEK_SET);
		fwrite(fBmpStr->imgData, fBmpStr->bmpHeader.imageSize, 1, fBmp);
		fclose(fBmp);
	}
}


#endif /* BMPIMAGE_H_ */
