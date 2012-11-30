
#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include "ImageFactory.h"
#include "BMPImage.h"
#include "stdio.h"
using namespace std;


int main(int argc, char *argv[]){

  BMPImage file_image = BMPImage(argv[1]);
  int** image = file_image.read_int();
  int rows=file_image.get_h();
  int cols=file_image.get_w();

//Reading the input file

  ofstream myfile;
  myfile.open ("C:/bitmap.txt");
  myfile<<rows<<endl;
  myfile<<cols<<endl;
  for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        myfile<<image[i][j]<<" ";
                }
                myfile<<"\n";
        }
myfile.close();
return 0;
}