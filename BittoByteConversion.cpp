#include<iostream>
#include<stdlib.h>
#include<fstream>
#include <time.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
        ofstream myfile;
        myfile.open (argv[1]);
        myfile<<"15\n";
        myfile<<"20\n";

		srand(time(NULL));

		int** m;

		   m = new int*[15];
			for(int i = 0; i < 15; i++)
				m[i] = new int[20];

		//alfabeto
		vector<int>v;

		//for(int i=0;i<100;i++){
			//per immagini binarie l'alfabeto è di due caratteri
		for(int i=0;i<2;i++){
			int number=0;
			for(int k=8;k>=0;k--)
                        {
                          int l = rand()%10;
                          number = number*10 + l;
                        }
                        v.push_back(number);
		}

		//l'alfabeto è stato generato
		//ora riempio la matrice pescando in modo random nell'alfabeto

 		for(int i=0;i<15;i++)

                for(int j=0;j<20;j++)


			//m[i][j]=v[rand()%100];

			//per immagini binarie v ha due elementi
			m[i][j]=v[rand()%2];

        for(int i=0;i<15;i++)
        {
                for(int j=0;j<20;j++)
                {
                    myfile<<m[i][j]<<" ";

                     /* int number =0;
                        for(int k=8;k>=0;k--)
                        {
                                int l = rand()%10;
                                number = number*10 + l;
                        }
                        myfile<<number<<" ";*/
                }
                myfile<<"\n";
        }
        myfile.close();
        return 0;
}