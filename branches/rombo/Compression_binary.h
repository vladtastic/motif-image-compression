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

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ImageFactory.h"
#include "BMPImage.h"
#include "MotifPatternCore_autoc.h"
#include "MyTrie.h"
#include "MyCore.h"
#include "CompactTrie.h"

#ifndef COMPRESSION_BINARY_H_
#define COMPRESSION_BINARY_H_


using namespace std;



class temp1{
        public:
        int total;
        int index;
};
class match{
        public:
        string rep;
        int index;
};

class patches{
        public:
        int freq;
        int rowsize;
        int colsize;
        int **array;
        vector<int> rowvec;
        vector<int> colvec;

 patches(int m, int n){
                rowsize=m;
                colsize=n;
                freq=1;
                array = new int*[rowsize];
                for(int i = 0; i < rowsize; i++)
                    array[i] = new int[colsize];
        }
};
//guardare il metodo!!!
void tempImageSet(int ** tempImage, int rows,int cols, int i, int j){
        for(int x=i;x<i+rows;x++){
                for(int y=j;y<j+cols;y++){
                        tempImage[x][y]=1;
                }
        }
}

int findexists(vector<int> dictindexes, int index){
        for(int i=0;i<dictindexes.size();i++)
        {
                if(dictindexes[i] == index)
                        return -1;
        }
        return 0;
}

/*
double calculateAverage(int** suppImage, int rows, int cols, int** tempImage_c){

	double sommaTot=0;
	double media=0;
	//calcolo la media
	for(int i=0;i<rows;i++)
	  for(int j=0; j<cols;j++)
		sommaTot=sommaTot+fabs(suppImage[i][j]-tempImage_c[i][j]);

	media=fabs(sommaTot/(double)(rows*cols));

	return media;

	}//*/

double calculateLoss(int** suppImage, int** tempImage_c, int rows, int cols){

	double sommaTot=0.0;
	double media=0.0;

	//calcolo la media
	for(int i=0;i<rows;i++){
	  for(int j=0; j<cols;j++){
		  //calcolo i codici RGB del colore
			//su supImage e tempImage_c
				unsigned char R1 = suppImage[i][j] & 0xFF;
				unsigned char G1 = (suppImage[i][j] >> 8) & 0xFF;
				unsigned char B1 = (suppImage[i][j] >> 16) & 0xFF;
				unsigned byte3 = (suppImage[i][j] >> 24) & 0xFF;

				unsigned char R2 = tempImage_c[i][j] & 0xFF;
				unsigned char G2 = (tempImage_c[i][j] >> 8) & 0xFF;
				unsigned char B2 = (tempImage_c[i][j] >> 16) & 0xFF;
				unsigned byte4 = (tempImage_c[i][j] >> 24) & 0xFF;

				double Ri=(double)R1;
				double Gi=(double)G1;
				double Bi=(double)B1;

				double Rj=(double)R2;
				double Gj=(double)G2;
				double Bj=(double)B2;

				double f1=(Ri-Rj)*(Ri-Rj);
				double f2=(Gi-Gj)*(Gi-Gj);
				double f3=(Bi-Bj)*(Bi-Bj);

		//calcolo distanza euclidea
		double distance=sqrt(f1+f2+f3);

		sommaTot=sommaTot+fabs(distance);

		}
	}

	double massimo=441.673;
	media=fabs(sommaTot/(double)(rows*cols));
	double loss=media/massimo*100;

	return loss;

}


/*
double calculateLoss(int** suppImage, int rows, int cols, double media){

	double maximo=0.0;
	double minimo=0.0;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(suppImage[i][j]>maximo) maximo=suppImage[i][j];
			if(suppImage[i][j]<minimo) minimo=suppImage[i][j];}
			}
	//ora min e max hanno il minimo
	double loss=media/(maximo-minimo)*100;
		return loss;
}//

/*
template<typename T>
double** calculateInterpolation(T** tempImage, int rows, int cols, T dontcare){

	double** tempImage_c;
	//ricopio la matrice in una matrice di double
	 tempImage_c = new double*[rows];
	for(int i = 0; i < rows; i++)
	     tempImage_c[i] = new double[cols];

	 for(int i=0;i<rows;i++)
	 	for(int j=0;j<cols;j++)
	 		tempImage_c[i][j]=tempImage[i][j];

	bool flag=true;
	while(flag){
	  for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
	     if(tempImage_c[i][j]==dontcare){
		  int c=0;
			double somma=0.0;

			if(i!=0 && i!=(rows-1) && j!=0 && j!=(cols-1)){//al centro
				if(tempImage_c[i-1][j]!=dontcare){
				     somma=somma+tempImage_c[i-1][j];c++;}
				if(tempImage_c[i+1][j]!=dontcare){
				     somma=somma+tempImage_c[i+1][j];c++;}
				if(tempImage_c[i][j-1]!=dontcare){
				     somma=somma+tempImage_c[i][j-1];c++;}
				if(tempImage_c[i][j+1]!=dontcare){
				    somma=somma+tempImage_c[i][j+1];c++;}
				if(tempImage_c[i-1][j-1]!=dontcare){
				     somma=somma+tempImage_c[i-1][j-1];c++;}
				if(tempImage_c[i-1][j+1]!=dontcare){
					somma=somma+tempImage_c[i-1][j+1];c++;}
				if(tempImage_c[i+1][j-1]!=dontcare){
					somma=somma+tempImage_c[i+1][j-1];c++;}
				if(tempImage_c[i+1][j+1]!=dontcare){
				    somma=somma+tempImage_c[i+1][j+1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}

			if(i==(rows-1) && j==0){//angolo basso sinistro
				if(tempImage_c[i-1][j]!=dontcare){
					somma=somma+tempImage_c[i-1][j];c++;}
				if(tempImage_c[i-1][j+1]!=dontcare){
					somma=somma+tempImage_c[i-1][j+1];c++;}
				if(tempImage_c[i][j+1]!=dontcare){
					somma=somma+tempImage_c[i][j+1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}

			if(i==(rows-1) && j==(cols-1)){//angolo basso destro
				if(tempImage_c[i-1][j]!=dontcare){
					somma=somma+tempImage_c[i-1][j];c++;}
				if(tempImage_c[i-1][j-1]!=dontcare){
					somma=somma+tempImage_c[i-1][j-1];c++;}
				if(tempImage_c[i][j-1]!=dontcare){
					somma=somma+tempImage_c[i][j-1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}

			if(i==0 && j==0){//angolo alto sinistro
				if(tempImage_c[i+1][j]!=dontcare){
					somma=somma+tempImage_c[i+1][j];c++;}
				if(tempImage_c[i][j+1]!=dontcare){
					somma=somma+tempImage_c[i][j+1];c++;}
				if(tempImage_c[i+1][j+1]!=dontcare){
					somma=somma+tempImage_c[i+1][j+1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}

			if(i==0 && j==(cols-1)){//angolo alto destro
				if(tempImage_c[i+1][j]!=dontcare){
					somma=somma+tempImage_c[i+1][j];c++;}
				if(tempImage_c[i][j-1]!=dontcare){
					somma=somma+tempImage_c[i][j-1];c++;}
				if(tempImage_c[i+1][j-1]!=dontcare){
					somma=somma+tempImage_c[i+1][j-1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}

			if(j==0 && i!=0 && i!=(rows-1)){//parete sinistra
				if(tempImage_c[i-1][j]!=dontcare){
					somma=somma+tempImage_c[i-1][j];c++;}
				if(tempImage_c[i-1][j+1]!=dontcare){
					somma=somma+tempImage_c[i-1][j+1];c++;}
				if(tempImage_c[i][j+1]!=dontcare){
					somma=somma+tempImage_c[i][j+1];c++;}
				if(tempImage_c[i+1][j]!=dontcare){
					somma=somma+tempImage_c[i+1][j];c++;}
				if(tempImage_c[i+1][j+1]!=dontcare){
				    somma=somma+tempImage_c[i+1][j+1];c++;}
			   	if(c==0){}
			    else tempImage_c[i][j]=somma/c;}

	   		if(j==(cols-1) && i!=0 && i!=(rows-1)){//parete destra
				if(tempImage_c[i-1][j]!=dontcare){
					somma=somma+tempImage_c[i-1][j];c++;}
				if(tempImage_c[i-1][j-1]!=dontcare){
					somma=somma+tempImage_c[i-1][j-1];c++;}
				if(tempImage_c[i][j-1]!=dontcare){
					somma=somma+tempImage_c[i][j-1];c++;}
				if(tempImage_c[i+1][j]!=dontcare){
					somma=somma+tempImage_c[i+1][j];c++;}
				if(tempImage_c[i+1][j-1]!=dontcare){
					somma=somma+tempImage_c[i+1][j-1];c++;}
			  	if(c==0){}
			  	else tempImage_c[i][j]=somma/c;}

		   if(i==(rows-1) && j!=0 && j!=(cols-1)){//parete in basso
				if(tempImage_c[i-1][j]!=dontcare){
					somma=somma+tempImage_c[i-1][j];c++;}
				if(tempImage_c[i-1][j-1]!=dontcare){
					somma=somma+tempImage_c[i-1][j-1];c++;}
				if(tempImage_c[i][j-1]!=dontcare){
					somma=somma+tempImage_c[i][j-1];c++;}
				if(tempImage_c[i][j+1]!=dontcare){
					somma=somma+tempImage_c[i][j+1];c++;}
				if(tempImage_c[i-1][j+1]!=dontcare){
					somma=somma+tempImage_c[i-1][j+1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}

			 if(i==0 && j!=0 && j!=(cols-1)){//parete in alto
				if(tempImage_c[i+1][j]!=dontcare){
					somma=somma+tempImage_c[i+1][j];c++;}
				if(tempImage_c[i+1][j+1]!=dontcare){
					somma=somma+tempImage_c[i+1][j+1];c++;}
				if(tempImage_c[i][j-1]!=dontcare){
					somma=somma+tempImage_c[i][j-1];c++;}
				if(tempImage_c[i][j+1]!=dontcare){
					somma=somma+tempImage_c[i][j+1];c++;}
				if(tempImage_c[i+1][j-1]!=dontcare){
					somma=somma+tempImage_c[i+1][j-1];c++;}
				if(c==0){}
				else tempImage_c[i][j]=somma/c;}
			}//if dontcare
		}//for
	}//for
	 int g=0;
	 for(int k=0; k<rows; k++)
		for(int h=0; h<cols; h++)
			if(tempImage_c[k][h]==dontcare) g++;
			if(g==0)flag=false;
   }//while

return tempImage_c;
}
*/

//il nuovo ok
int** calculateInterpolationBMP(int** tempImage_c, int rows, int cols, int dontcare){

	bool flag=true;
	while(flag){
	  for(int i=0; i<rows; i++){
		for(int j=0;j<cols;j++){
	     if(tempImage_c[i][j]==dontcare){
		  double c=0.0;
			int sommaB=0.0;
			int sommaG=0.0;
			int sommaR=0.0;
			unsigned char byte0=0;
			unsigned char byte1=0;
			unsigned char byte2=0;
			unsigned char byte3=0;

			if(i!=0 && i!=(rows-1) && j!=0 && j!=(cols-1)){//al centro
				if(tempImage_c[i-1][j]!=dontcare){

						byte0 = tempImage_c[i-1][j] & 0xFF;
						byte1 = (tempImage_c[i-1][j] >> 8) & 0xFF;
						byte2 = (tempImage_c[i-1][j] >> 16) & 0xFF;
						byte3 = (tempImage_c[i-1][j] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;
						c++;
				     }
				if(tempImage_c[i+1][j]!=dontcare){

				    	byte0 = tempImage_c[i+1][j] & 0xFF;
						byte1 = (tempImage_c[i+1][j] >> 8) & 0xFF;
						byte2 = (tempImage_c[i+1][j] >> 16) & 0xFF;
						byte3 = (tempImage_c[i+1][j] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;
						c++;}
				if(tempImage_c[i][j-1]!=dontcare){

				     	byte0 = tempImage_c[i][j-1] & 0xFF;
						byte1 = (tempImage_c[i][j-1] >> 8) & 0xFF;
						byte2 = (tempImage_c[i][j-1] >> 16) & 0xFF;
						byte3 = (tempImage_c[i][j-1] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;
						c++;}
				if(tempImage_c[i][j+1]!=dontcare){
					byte0 = tempImage_c[i][j+1] & 0xFF;
					byte1 = (tempImage_c[i][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
				    }
				if(tempImage_c[i-1][j-1]!=dontcare){
					byte0 = tempImage_c[i-1][j-1] & 0xFF;
					byte1 = (tempImage_c[i-1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
				     }
				if(tempImage_c[i-1][j+1]!=dontcare){
					byte0 = tempImage_c[i-1][j+1] & 0xFF;
					byte1 = (tempImage_c[i-1][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i+1][j-1]!=dontcare){
					byte0 = tempImage_c[i+1][j-1] & 0xFF;
					byte1 = (tempImage_c[i+1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i+1][j+1]!=dontcare){
				    byte0 = tempImage_c[i+1][j+1] & 0xFF;
						byte1 = (tempImage_c[i+1][j+1] >> 8) & 0xFF;
						byte2 = (tempImage_c[i+1][j+1] >> 16) & 0xFF;
						byte3 = (tempImage_c[i+1][j+1] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;
						c++;}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));
					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;
					tempImage_c[i][j]=bgr;}
				}

			if(i==(rows-1) && j==0){//angolo basso sinistro
				if(tempImage_c[i-1][j]!=dontcare){

					byte0 = tempImage_c[i-1][j] & 0xFF;
					byte1 = (tempImage_c[i-1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i-1][j+1]!=dontcare){

					byte0 = tempImage_c[i-1][j+1] & 0xFF;
					byte1 = (tempImage_c[i-1][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i][j+1]!=dontcare){
					byte0 = tempImage_c[i][j+1] & 0xFF;
					byte1 = (tempImage_c[i][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));
					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
					}
				}
			if(i==(rows-1) && j==(cols-1)){//angolo basso destro
				if(tempImage_c[i-1][j]!=dontcare){
					byte0 = tempImage_c[i-1][j] & 0xFF;
					byte1 = (tempImage_c[i-1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i-1][j-1]!=dontcare){
					byte0 = tempImage_c[i-1][j-1] & 0xFF;
					byte1 = (tempImage_c[i-1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i][j-1]!=dontcare){
					byte0 = tempImage_c[i][j-1] & 0xFF;
					byte1 = (tempImage_c[i][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));
					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
					}
				}

			if(i==0 && j==0){//angolo alto sinistro
				if(tempImage_c[i+1][j]!=dontcare){
					byte0 = tempImage_c[i+1][j] & 0xFF;
						byte1 = (tempImage_c[i+1][j] >> 8) & 0xFF;
						byte2 = (tempImage_c[i+1][j] >> 16) & 0xFF;
						byte3 = (tempImage_c[i+1][j] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i][j+1]!=dontcare){
					byte0 = tempImage_c[i][j+1] & 0xFF;
					byte1 = (tempImage_c[i][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j+1]!=dontcare){
						byte0 = tempImage_c[i+1][j+1] & 0xFF;
						byte1 = (tempImage_c[i+1][j+1] >> 8) & 0xFF;
						byte2 = (tempImage_c[i+1][j+1] >> 16) & 0xFF;
						byte3 = (tempImage_c[i+1][j+1] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;

						c++;
					}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));

					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
					}
				}
			if(i==0 && j==(cols-1)){//angolo alto destro
				if(tempImage_c[i+1][j]!=dontcare){
						byte0 = tempImage_c[i+1][j] & 0xFF;
						byte1 = (tempImage_c[i+1][j] >> 8) & 0xFF;
						byte2 = (tempImage_c[i+1][j] >> 16) & 0xFF;
						byte3 = (tempImage_c[i+1][j] >> 24) & 0xFF;

						sommaB=sommaB+(int)byte2;
						sommaG=sommaG+(int)byte1;
						sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i][j-1]!=dontcare){
					byte0 = tempImage_c[i][j-1] & 0xFF;
					byte1 = (tempImage_c[i][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;

					}
				if(tempImage_c[i+1][j-1]!=dontcare){

					byte0 = tempImage_c[i+1][j-1] & 0xFF;
					byte1 = (tempImage_c[i+1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));

					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
				}
			}

			if(j==0 && i!=0 && i!=(rows-1)){//parete sinistra
				if(tempImage_c[i-1][j]!=dontcare){

					byte0 = tempImage_c[i-1][j] & 0xFF;
					byte1 = (tempImage_c[i-1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;

					}
				if(tempImage_c[i-1][j+1]!=dontcare){

					byte0 = tempImage_c[i-1][j+1] & 0xFF;
					byte1 = (tempImage_c[i-1][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;
						c++;
					}
				if(tempImage_c[i][j+1]!=dontcare){

					byte0 = tempImage_c[i][j+1] & 0xFF;
					byte1 = (tempImage_c[i][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j]!=dontcare){
					byte0 = tempImage_c[i+1][j] & 0xFF;
					byte1 = (tempImage_c[i+1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j+1]!=dontcare){
				    byte0 = tempImage_c[i+1][j+1] & 0xFF;
					byte1 = (tempImage_c[i+1][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
				    }
			   	if(c==0){}
			    else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));
					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
			    }

			   }

	   		if(j==(cols-1) && i!=0 && i!=(rows-1)){//parete destra
				if(tempImage_c[i-1][j]!=dontcare){

					byte0 = tempImage_c[i-1][j] & 0xFF;
					byte1 = (tempImage_c[i-1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i-1][j-1]!=dontcare){

					byte0 = tempImage_c[i-1][j-1] & 0xFF;
					byte1 = (tempImage_c[i-1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i][j-1]!=dontcare){
					 byte0 = tempImage_c[i][j-1] & 0xFF;
					 byte1 = (tempImage_c[i][j-1] >> 8) & 0xFF;
					 byte2 = (tempImage_c[i][j-1] >> 16) & 0xFF;
					 byte3 = (tempImage_c[i][j-1] >> 24) & 0xFF;

					 sommaB=sommaB+(int)byte2;
					 sommaG=sommaG+(int)byte1;
					 sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j]!=dontcare){
					byte0 = tempImage_c[i+1][j] & 0xFF;
					byte1 = (tempImage_c[i+1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j-1]!=dontcare){
					byte0 = tempImage_c[i+1][j-1] & 0xFF;
					byte1 = (tempImage_c[i+1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
			  	if(c==0){}
			  	else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));

					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
			  	}
			  }

		   if(i==(rows-1) && j!=0 && j!=(cols-1)){//parete in basso
				if(tempImage_c[i-1][j]!=dontcare){
					byte0 = tempImage_c[i-1][j] & 0xFF;
					byte1 = (tempImage_c[i-1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i-1][j-1]!=dontcare){

					byte0 = tempImage_c[i-1][j-1] & 0xFF;
					byte1 = (tempImage_c[i-1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i][j-1]!=dontcare){

					byte0 = tempImage_c[i][j-1] & 0xFF;
					byte1 = (tempImage_c[i][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i][j+1]!=dontcare){
					byte0 = tempImage_c[i][j+1] & 0xFF;
					byte1 = (tempImage_c[i][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i-1][j+1]!=dontcare){

					byte0 = tempImage_c[i-1][j+1] & 0xFF;
					byte1 = (tempImage_c[i-1][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i-1][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i-1][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));

					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;

					}
				}

			 if(i==0 && j!=0 && j!=(cols-1)){//parete in alto
				if(tempImage_c[i+1][j]!=dontcare){
					byte0 = tempImage_c[i+1][j] & 0xFF;
					byte1 = (tempImage_c[i+1][j] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j+1]!=dontcare){

					byte0 = tempImage_c[i+1][j+1] & 0xFF;
					byte1 = (tempImage_c[i+1][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i][j-1]!=dontcare){

					byte0 = tempImage_c[i][j-1] & 0xFF;
					byte1 = (tempImage_c[i][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i][j+1]!=dontcare){
					byte0 = tempImage_c[i][j+1] & 0xFF;
					byte1 = (tempImage_c[i][j+1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i][j+1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i][j+1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(tempImage_c[i+1][j-1]!=dontcare){

					byte0 = tempImage_c[i+1][j-1] & 0xFF;
					byte1 = (tempImage_c[i+1][j-1] >> 8) & 0xFF;
					byte2 = (tempImage_c[i+1][j-1] >> 16) & 0xFF;
					byte3 = (tempImage_c[i+1][j-1] >> 24) & 0xFF;

					sommaB=sommaB+(int)byte2;
					sommaG=sommaG+(int)byte1;
					sommaR=sommaR+(int)byte0;

						c++;
					}
				if(c==0){}
				else {
					//calcolo vettore media
					unsigned char compB=(unsigned char)(round(sommaB/c));
					unsigned char compG=(unsigned char)(round(sommaG/c));
					unsigned char compR=(unsigned char)(round(sommaR/c));
					int bgr = 0x0000;

					bgr = (0 & 0xF) << 24;
					bgr |= (compB & 0xFF) << 16; // 0xF == 15 == 1111 binary
					bgr |= (compG & 0xFF) << 8;
					bgr |= (compR & 0xFF) << 0;

					tempImage_c[i][j]=bgr;
					}
				}
			}//if dontcare
		}//for
	}//for
	 int g=0;
	 for(int k=0; k<rows; k++)
		for(int h=0; h<cols; h++)
			if(tempImage_c[k][h]==dontcare) g++;
			if(g==0)flag=false;
   }//while

return tempImage_c;
}


template<typename T>
void compressBasis(T** image, int rows, int cols, T dontcare,
		int max_rows, int max_cols, int max_dontcare,
		int min_rows, int min_cols, unsigned char pixel, bool optimize, bool debug, const char* filename_img, const char* filename_loss, const char* filename_compressed, const char* filename_reconstructed, const char* filename_interpolated) {

 T **tempImage;
 vector<int> dictindexes;
 vector<match> matchvec;
 T ** suppImage;

 vector<patches> dict;
 vector<int> dict_count;

//set<int> basis;
 set<int> basis= set<int> ();
 MyTrie<T> maximal = MyTrie<T>();



suppImage = new T*[rows];
for(int i = 0; i < rows; i++)
	suppImage[i] = new T[cols];

for(int i=0;i<rows;i++)
	for(int j=0;j<cols;j++)
		suppImage[i][j]=image[i][j];

tempImage = new T*[rows];
for(int i = 0; i < rows; i++)
     tempImage[i] = new T[cols];

for(int i=0;i<rows;i++){
   for(int j=0;j<cols;j++){
      tempImage[i][j]=0;
   }
}

//begin patch discovery
//process(image, rows, cols, dontcare, max_rows, max_cols, max_dontcare,
		//min_rows, min_cols, optimize, false, &basis, maximal);

get_Base(image, rows, cols, dontcare, max_rows, max_cols, max_dontcare,
		min_rows, min_cols, maximal, basis);

ofstream myfile1(filename_img, ios::binary);
//stampa base


if(basis.size()==0){
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(pixel==4){//ottimizzazione per bitmap
				char* out=new char[3];
				unsigned char byte0 = image[i][j] & 0xFF;
				unsigned char byte1 = (image[i][j] >> 8) & 0xFF;
				unsigned char byte2 = (image[i][j] >> 16) & 0xFF;
				unsigned byte3 = (image[i][j] >> 24) & 0xFF;

				out[0] = byte2;
				out[1] = byte1;
				out[2] = byte0;

				myfile1.write(out, 3);
			}else myfile1.write((char *)&image[i][j], (int)pixel);
		}
	}
	myfile1.close();
	return;
}

//inserisce i motivi della base nella lista delle patch
for(set<int>::iterator it = basis.begin(); it != basis.end(); it++){

 	T** motif=maximal.getMotif(*it);

 	//stampa sui motivi
 	int col=maximal.getCols(*it);
 	int row=maximal.getRows(*it);

	patches *p = new patches(row,col);
	int occ1=0;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
		bool flag=true;
		if(((i+row)>rows) || ((j+col)>cols)){ }
		else{
			for(int k=0;k<row && flag;k++){
				for(int h=0;h<col && flag;h++){
						if(image[i+k][j+h]!=motif[k][h] && motif[k][h]!=-1000)
							flag=false;
						}//for
					}//for
				if(flag==true){
				occ1++;
				p->rowvec.push_back(i);
				p->colvec.push_back(j);
					}//if
				}//else
			}//for
		}//for

	p->freq=occ1;

	//patch
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			p->array[i][j]=motif[i][j];
			dict.push_back(*p);

	//qui cancello la patch corrente dealloco
	for(int x=0;x<row;x++)
		delete motif[x];
	delete motif;

 }
//finding patch with maximum gain

//verifichiamo dimensione puntatori usando il numero delle patch
int size_dict=dict.size();
//sempre 1 byte
//1) dimensione puntatori
unsigned char dim_punt;
//cout<<size_dict;
if(size_dict>0 && size_dict<=UCHAR_MAX)
	//codifica con un byte
	dim_punt=(unsigned char)1;

if(size_dict>UCHAR_MAX && size_dict<=USHRT_MAX)
//con due byte
	dim_punt=(unsigned char)2;

if(size_dict>USHRT_MAX && size_dict<=ULONG_MAX)
	//con 4 byte
	dim_punt=(unsigned char)4;

myfile1.write((char *)&dim_punt, sizeof(dim_punt));
cout<<"dimensione puntatori:"<<(int)dim_punt<<" size:"<<sizeof(dim_punt)<<endl;

//2) grandezza pixel
myfile1.write((char *)&pixel, sizeof(pixel));
cout<<"grandezza pixel:"<<(int)pixel<<" size:"<<sizeof(pixel)<<endl;

//3) dimensioni immagine originaria
short rows_imm=(short)rows;
short cols_imm=(short)cols;

myfile1.write((char *)&rows_imm, sizeof(rows_imm));
myfile1.write((char *)&cols_imm, sizeof(cols_imm));

cout<<"R:"<<(int)rows_imm<<" size:"<<sizeof(rows_imm)<<endl;
cout<<"C:"<<(int)cols_imm<<" size:"<<sizeof(cols_imm)<<endl;

//calcola guadagno per ogni patch e ordina item in base al guadagno
     vector<temp1> item;
     for(int i=0;i<dict.size();i++){
                temp1 *t1= new temp1;
                patches *p1 = &dict[i];
                int totalpatch = p1->rowsize * p1->colsize;
                totalpatch = totalpatch * p1->freq;
                int total = rows*cols;
                total = total - totalpatch + p1->rowsize * p1->colsize + (p1->freq-1)*1;
                t1->total=total;
                t1->index=i;
                item.push_back(*t1);
        }
        int min = rows*cols;
        int index;
        for(int i=0;i<item.size();i++){
                for(int j =i;j<item.size();j++){
                     if(item[i].total  >  item[j].total){
                                temp1 mem = item[i];
                                item[i]=item[j];
                                item[j]=mem;
                        }
                }
        }
        if(debug){
        	cout<<"The following patch produces the maximum compression \n";
		    cout<<"Now the size of the image is as follows \n";
		    cout<<item[0].index<<"\n";
        	cout<<item[0].total<<"\n";
        }


//compression phase
  for(int l=0;l<item.size();l++){
                index = item[l].index;

                for(int k=0;k<dict[index].rowvec.size();k++){
                        int i = dict[index].rowvec[k];
                        int j = dict[index].colvec[k];

                        int tempcount=0;
                        int rsize = dict[index].rowsize;
                        int csize = dict[index].colsize;
                        for(int x=0;x<rsize;x++){
                                for(int y=0;y<csize;y++){
                                        if(tempImage[x+i][y+j]!=0){
                                                tempcount=1;
                                        }
                                }
                        }
                        if(tempcount==0){

                                for(int x=i;x< i+rsize;x++){
                                        for(int y=j;y<j+csize;y++){

                                                image[x][y] = -1;
                                        }
                                }
                                        image[i][j]=(index+2)*-1;

                                tempImageSet(tempImage,rsize,csize,i,j);
                        }
     			}
      }

  if(debug){
	cout<<"Compressed image"<<endl;
      for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
           cout<<image[i][j]<<" ";
         }
         cout<<"\n";
      }
        cout<<"\n";
        cout<<"\n";
  }

  //qua facciamo stampare la bitmap compressa image
  BMPImage consintimage = BMPImage(filename_compressed);
  consintimage.set_int(image, rows, cols);
  consintimage.writeBmpFile();
/*ofstream myfile5;
  myfile5.open ("./bitmap_compressed01.txt");
  myfile5<<rows<<endl;
  myfile5<<cols<<endl;
  for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        myfile5<<image[i][j]<<" ";
                }
                myfile5<<"\n";
        }
myfile5.close();*/


//decompression phase----------------------
        for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
                        tempImage[i][j]=0;
                }
        }
        for(int i=0;i<rows;i++){
                for(int j=0;j<cols;j++){
					if(image[i][j]<0){
                        if(image[i][j]!=-1)
                        {
                                int index = -1*image[i][j]-2;
                                for(int k=0;k<dict[index].rowsize;k++)
                                {
                                        for(int l=0;l<dict[index].colsize;l++)
                                        {
                                                tempImage[i+k][j+l]=dict[index].array[k][l];
                                        }
                                }
                        }
                   } else tempImage[i][j]=image[i][j];
		 		}
        	}
     					for(int i=0;i<rows;i++){
	                 		for(int j=0;j<cols;j++){
	                         if(image[i][j]!=-1 && image[i][j]<0){
	                                 int index = -1*image[i][j]-2;
	                                 int k = findexists(dictindexes, index);
	                                 if(k==0){
                                         dictindexes.push_back(index);
                                      }
                                }
                    		}
     					}

		//dictindexes ha una size pari al numero di patch effettivamente sostituite
		//4) numero patch
		int area=rows*cols;

		if(area>0 && area<=UCHAR_MAX){
		//codifica con un byte
		unsigned char n_patch=(unsigned char)dictindexes.size();
		myfile1.write((char *)&n_patch, sizeof(unsigned char));
		cout<<"Area:"<<area<<" n_patch:"<<(int)n_patch<<" dim:"<<sizeof(n_patch)<<endl;
		}

		if(area>UCHAR_MAX && area<=USHRT_MAX){
		//con due byte
		short n_patch=(short)dictindexes.size();
		myfile1.write((char *)&n_patch, sizeof(short));
		cout<<"Area:"<<area<<" n_patch:"<<(int)n_patch<<" dim:"<<sizeof(n_patch)<<endl;

		}

		if(area>USHRT_MAX && area<=ULONG_MAX){
		//con 4 byte
		int n_patch=(int)dictindexes.size();
		myfile1.write((char *)&n_patch, sizeof(int));
		cout<<"Area:"<<area<<" n_patch:"<<(int)n_patch<<" dim:"<<sizeof(n_patch)<<endl;

		}

		dictindexes.clear();

		for(int i=0;i<rows;i++){
           for(int j=0;j<cols;j++){
              if(image[i][j]!=-1 && image[i][j]<0){
                int index = -1*image[i][j]-2;
                int k = findexists(dictindexes, index);
                if(k==0){
                  match *m1= new match();
                  m1->index=index;
                  dictindexes.push_back(index);
                  //myfile1<<index<<"\n";

				  //5) triple (R,C) patch
                  //ricaviamo un numero di bit in base al campo R e C
                  if(rows>0 && rows<=UCHAR_MAX){
					//codifica con un byte
					unsigned char rows_patch=(unsigned char)dict[index].rowsize;
					myfile1.write((char *)&rows_patch, sizeof(unsigned char));
					cout<<"rows:"<<(int)rows_patch<<" dim:"<<sizeof(rows_patch)<<endl;

					}
				 if(rows>UCHAR_MAX && rows<=USHRT_MAX){
					//codifica con due byte
					short rows_patch=(short)dict[index].rowsize;
					myfile1.write((char *)&rows_patch, sizeof(short));
					cout<<"rows:"<<(int)rows_patch<<" dim:"<<sizeof(rows_patch)<<endl;
					}

				 if(cols>0 && cols<=UCHAR_MAX){
					//codifica con un byte
					unsigned char cols_patch=(unsigned char)dict[index].colsize;
					myfile1.write((char *)&cols_patch, sizeof(unsigned char));
					cout<<"cols:"<<(int)cols_patch<<" dim:"<<sizeof(cols_patch)<<endl;
					}
				 if(cols>UCHAR_MAX && cols<=USHRT_MAX){
					//codifica con due byte
					short cols_patch=(short)dict[index].colsize;
					myfile1.write((char *)&cols_patch, sizeof(short));
					cout<<"cols:"<<(int)cols_patch<<" dim:"<<sizeof(cols_patch)<<endl;
					}
/*
						int conta_dc=0;
							//prima devo mettere la dimensione della lista pari al numero di dc
							for(int k=0;k<dict[index].rowsize;k++){
				              for(int l=0;l<dict[index].colsize;l++){
									if(dict[index].array[k][l]==dontcare){
										conta_dc++;
								  }//if
								}//for
							}//for


			//pongo la dimensione della lista
			//che dipende dal valore R X C
			int area_patch=dict[index].rowsize*dict[index].colsize;

			if(area_patch>0 && area_patch<=UCHAR_MAX){
					//codifica con un byte
						unsigned char cod=(unsigned char)conta_dc;
						myfile1.write((char *)&cod, sizeof(unsigned char));
						cout<<"dim lista dc:"<<(int)cod<<" size:"<<sizeof(unsigned char)<<endl;
						}
			if(area_patch>UCHAR_MAX && area_patch<=USHRT_MAX){
					//con due byte
						short cod=(short)conta_dc;
						myfile1.write((char *)&cod, sizeof(short));
						cout<<"dim lista dc:"<<cod<<" size:"<<sizeof(short)<<endl;
					}//if

			if(area_patch>USHRT_MAX && area_patch<=ULONG_MAX){
						//con quattro byte
						myfile1.write((char *)&conta_dc, sizeof(int));
						cout<<"dim lista dc:"<<conta_dc<<" size:"<<sizeof(int)<<endl;
					}//if


			conta_dc=0;
			//da qua in poi prima di mettere le patch devo inserire gli offset
			for(int k=0;k<dict[index].rowsize;k++){
				for(int l=0;l<dict[index].colsize;l++){
				 conta_dc++;
				if(dict[index].array[k][l]==dontcare){
					if(area_patch>0 && area_patch<=UCHAR_MAX){
					//codifica con un byte
						unsigned char cod=(unsigned char)conta_dc;
						myfile1.write((char *)&cod, sizeof(unsigned char));
						cout<<"offset dc:"<<(int)cod<<" size:"<<sizeof(unsigned char)<<endl;
						}
					if(area_patch>UCHAR_MAX && area_patch<=USHRT_MAX){
					//con due byte
						short cod=(short)conta_dc;
						myfile1.write((char *)&cod, sizeof(short));
						cout<<"offset dc:"<<cod<<" size:"<<sizeof(short)<<endl;
					}//if

					if(area_patch>USHRT_MAX && area_patch<=ULONG_MAX){
						//con quattro byte
						myfile1.write((char *)&conta_dc, sizeof(int));
						cout<<"offset dc:"<<conta_dc<<" size:"<<sizeof(int)<<endl;}
					}//if
				}//for
			}//for
*/

				for(int k=0;k<dict[index].rowsize;k++){
                   for(int l=0;l<dict[index].colsize;l++){
                      //myfile1<<dict[index].array[k][l]<<" ";
                      T pixel_curr=dict[index].array[k][l];

					  //metto il pixel a 24 bit e al posto del dc metto un simbolo byte
					 if(pixel_curr!=dontcare){
					    if(pixel==4){//ottimizzazione per bitmap
					  		char* out=new char[3];
					  		unsigned char byte0 = image[i][j] & 0xFF;
					  	    unsigned char byte1 = (image[i][j] >> 8) & 0xFF;
					  		unsigned char byte2 = (image[i][j] >> 16) & 0xFF;
					  		unsigned byte3 = (image[i][j] >> 24) & 0xFF;

					  		out[0] = byte2;
					  		out[1] = byte1;
					  		out[2] = byte0;

					  		myfile1.write(out, 3);
					  	    }else myfile1.write((char *)&pixel_curr, (int)pixel);
					  	  }else myfile1.write((char *)&pixel_curr, (int)pixel);
					  cout<<"pixel:"<<pixel_curr<<" dim:"<<(int)pixel<<endl;
                      tempImage[i+k][j+l]=dict[index].array[k][l];
                    }
                                            //myfile1<<"\n";
                  }
                    matchvec.push_back(*m1);
                 }
               }
		 	}

       }

//quanti sono gli offset?
//6) dimensione lista
int dim_offset=0;

      for(int i=0;i<rows;i++){
	         for(int j=0;j<cols;j++){
	               if(image[i][j]!=-1)
	                     if(image[i][j]<0)
								dim_offset++;
	                 }
        		}

		//int area=rows*cols;
		if(area>0 && area<=UCHAR_MAX){
		//codifica con un byte
		unsigned char dim_lista=(unsigned char)dim_offset;
		myfile1.write((char *)&dim_lista, sizeof(unsigned char));
		cout<<"dim lista:"<<(int)dim_lista<<" size:"<<sizeof(dim_lista)<<endl;
		}
		if(area>UCHAR_MAX && area<=USHRT_MAX){
		//con due byte
		short dim_lista=(short)dim_offset;
		myfile1.write((char *)&dim_lista, sizeof(short));
		cout<<"dim lista:"<<dim_lista<<" size:"<<sizeof(dim_lista)<<endl;
		}
		if(area>USHRT_MAX && area<=ULONG_MAX){
		//con 4 byte
		myfile1.write((char *)&dim_offset, sizeof(int));
		cout<<"dim lista:"<<dim_offset<<" size:"<<sizeof(int)<<endl;
		}

//7) offset puntatori
int offset=0;
for(int i=0;i<rows;i++){
	         for(int j=0;j<cols;j++){
	               if(image[i][j]!=-1){
					   offset++;
					   	if(image[i][j]<0){//è un puntatore
	                    	myfile1.write((char *)&offset, sizeof(int));
	                    	cout<<"offset:"<<offset<<" size:"<<sizeof(int)<<endl;
	                    	}

	                    }

	          }
       }
      for(int i=0;i<rows;i++){
	         for(int j=0;j<cols;j++){
	               if(image[i][j]!=-1)
	                     if(image[i][j]<0){
						//8) multiset pixel-puntatori
	                     //è un puntatore
	                     //quanti bit necessitano dipende da dim_punt
	                     if(dim_punt==1){
	                     		unsigned char car=(unsigned char)(-1*image[i][j]);
	                     		myfile1.write((char *)&car,(int)dim_punt);
	                     		cout<<"car:"<<(int)car<<" size:"<<(int)dim_punt<<endl;
	                     		}
	                     if(dim_punt==2){
		                		short car=(short)(-1*image[i][j]);
	                     		myfile1.write((char *)&car,(int)dim_punt);
	                     		cout<<"car:"<<car<<" size:"<<(int)dim_punt<<endl;

	                     		}
	                     if(dim_punt==4){
	                     		int car=(-1*image[i][j]);
	                     		myfile1.write((char *)&car,(int)dim_punt);
	                     		cout<<"car:"<<car<<" size:"<<dim_punt<<endl;

	                     		}
	                     }
	                    /* else{
	                       myfile1.write((char *)&image[i][j], (int)pixel);
							cout<<"car:"<<image[i][j]<<" size:"<<(int)pixel<<endl;}*/
						 else{
							if(pixel==4){//ottimizzazione per bitmap
			 				char* out=new char[3];
			 				unsigned char byte0 = image[i][j] & 0xFF;
			 				unsigned char byte1 = (image[i][j] >> 8) & 0xFF;
			 				unsigned char byte2 = (image[i][j] >> 16) & 0xFF;
			 				unsigned byte3 = (image[i][j] >> 24) & 0xFF;

			 				out[0] = byte2;
			 				out[1] = byte1;
			 				out[2] = byte0;

	       	 				myfile1.write(out, 3);
	       					}//if
	       					else myfile1.write((char *)&image[i][j], (int)pixel);
		   					cout<<"car:"<<image[i][j]<<" size:"<<(int)pixel<<endl;

	                       }
	                   }
        		}

if(debug){
	cout<<"Reconstructed image"<<endl;
 	for(int i=0; i<rows;i++){
 	 for(int j=0; j<cols;j++){
   		cout<<tempImage[i][j]<<" ";
 	 }
 	cout<<endl;
 	}
}
/*
ofstream myfile6;
  myfile6.open ("./bitmap_reconstructed01.txt");
  myfile6<<rows<<endl;
  myfile6<<cols<<endl;
  for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        myfile6<<tempImage[i][j]<<" ";
                }
                myfile6<<"\n";
        }
myfile6.close();*/

//qua dovrei farmi stampare in bitmap l'immagine ricostruita
BMPImage consint1 = BMPImage(filename_reconstructed);
consint1.set_int(tempImage, rows, cols);
consint1.writeBmpFile();


ofstream myfile2;
myfile2.open(filename_loss);

int** tempImage_c=calculateInterpolationBMP(tempImage, rows, cols, dontcare);

if(debug){
	cout<<"Interpolated image"<<endl;
 	for(int i=0; i<rows;i++){
 	 for(int j=0; j<cols;j++){
   		cout<<tempImage_c[i][j]<<" ";
 	 }
 	cout<<endl;
 	}
}
/*
ofstream myfile7;
  myfile7.open ("./bitmap_interpolated01.txt");
  myfile7<<rows<<endl;
  myfile7<<cols<<endl;
  for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        myfile7<<tempImage_c[i][j]<<" ";
                }
                myfile7<<"\n";
        }
myfile7.close();*/

//qua dovrei farmi stampare l'immagine interpolata
BMPImage consintimage2 = BMPImage(filename_interpolated);
consintimage2.set_int(tempImage_c, rows, cols);
consintimage2.writeBmpFile();

//double avg=calculateAverage(suppImage, rows, cols, tempImage_c);

//ora dovrei calcolare valore massimo e minimo
//BMPImage file_orig = BMPImage("./einstein/einstein4.bmp");

	//int** image_orig = file_orig.read_int();


double loss=calculateLoss(suppImage, tempImage_c, rows, cols);

//double loss=calculateLoss(image_orig, tempImage_c, rows, cols);
//double loss=calculateLoss(suppImage, tempImage_c, rows, cols);

if(debug){
	cout<<"Loss:"<<loss<<endl;
}

myfile2<<"Loss:"<<loss<<endl;
myfile2.close();
        myfile1.close();

}//compress



#endif /* COMPRESSION_BINARY_H_ */
