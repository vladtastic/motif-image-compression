#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <omp.h>
#include <sys/time.h>
#include "Image.h"

using namespace std;


void get_auto(Image &I)
{
	//Get the bites of the image;
	uint8_t* pix = I.p;
	for(int x = I.WIDTH-1 ; x >= 0 ; x--)
	{
		//cout << "X = " << x << endl;
		for(int y = I.HEIGHT ; y >= 0 ; y--)
		{
			
			//cout << "Y = " << y << endl;
	
			//cout << "\n Generating bite" << endl;
			uint8_t* bite = (uint8_t *)malloc((I.HEIGHT-y)*(I.WIDTH-x));
			uint8_t* consensus = (uint8_t *)malloc((I.HEIGHT-y)*(I.WIDTH-x));
			

			for(int i = y ; i < I.HEIGHT ; i++)
			{
				for(int j = x ; j < I.WIDTH ; j++)
				{
					bite[(i-y)*(I.WIDTH-x) + (j-x)] = pix[i * I.WIDTH + j];
				} 
			}
			//Calculate the consensus between the bite and the image
			//cout << "\n Calculating Auto correlation\n\n";

			for(int i = 0 ; i < I.HEIGHT - y ; i++)
			{
				for(int j = 0; j < I.WIDTH - x ; j++)
				{
					if(bite[i*(I.WIDTH-x) + j] == pix[i*(I.WIDTH)+j])
					{
						consensus[i*(I.WIDTH-x)+j] = pix[i*(I.WIDTH)+j];
					}
					else
					{	
						consensus[i*(I.WIDTH-x)+j] = (uint8_t)255; // Don't care character;
					}			
			
				}

			}

			 //end parallel region
	
			free( consensus );
			free( bite );

		}
	}
	return;
}


int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cout << "Usage : bin/exec filename\n";
		return 0;
	}
	char* filename = argv[1];
	Image I(filename);

	struct timeval t0, t1;
	double elapsed = 0.0;

	gettimeofday( &t0, NULL );	
	
	get_auto(I);
	
	gettimeofday( &t1, NULL );
	
	elapsed = ( t1.tv_sec - t0.tv_sec ) + ( ( t1.tv_usec - t0.tv_usec ) / 1e6 );

	cout << "Timer: " << elapsed << endl;
	
	return 0 ;	
}
