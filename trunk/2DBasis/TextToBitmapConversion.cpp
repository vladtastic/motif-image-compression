
#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "ImageFactory.h"
#include "BMPImage.h"
#include "stdio.h"
using namespace std;


int main(int argc, char *argv[]){

	int** image;
	int rows;
	int cols;
	ifstream file;
	 file.open(argv[1]);
	    if (!file) {
	        cout << "Unable to open file";
	        return 0; // terminate with error
	    }
	        file>>rows;
	        file>>cols;
	   image = new int*[rows];
		for(int i = 0; i < rows; i++)
			image[i] = new int[cols];

	        for(int i=0;i<rows;i++)
			   {
			      for(int j=0;j<cols;j++)
			          {
			           file>>image[i][j];

			                }
	        }

BMPImage consintimage = BMPImage(argv[2]);
	consintimage.set_int(image, rows, cols);
	consintimage.writeBmpFile();

	file.close();

return 0;
}