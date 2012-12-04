#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include "Image.h"

using namespace std;


void get_auto(Image &I)
{
	//Get the bites of the image;
	int x = 100;
	int y = 100;
	
	uint8_t* pix = I.p;
	uint8_t* bite = (uint8_t *)malloc((I.HEIGHT-y)*(I.WIDTH-x));
	for(int i = y ; i < I.HEIGHT ; i++)
	{
		for(int j = x ; j < I.WIDTH ; j++)
		{
			bite[(i-y)*(I.WIDTH-x) + (j-x)] = pix[i * I.WIDTH + j];
		} 
	}
	
	/*for(int i = 0 ; i < I.HEIGHT - y ; i++)
	{
		for(int j = 0 ; j < I.WIDTH - x ; j++)
		{
			cout << (unsigned int)pix[i*(I.WIDTH) + j] << "\t";
		}
		cout << "\n";
	}*/
	
	/*for(int i = 0 ; i < I.HEIGHT - y ; i++)
	{
		for(int j = 0 ; j < I.WIDTH - x ; j++)
		{
			cout << (unsigned int)bite[i*(I.WIDTH-x) + j] << "\t";
		}
		cout << "\n";
	}*/
	
	//Calculate the consensus between the bite and the image
	uint8_t* consensus = (uint8_t *)malloc((I.HEIGHT-y)*(I.WIDTH-x));
	
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
			
			cout << (unsigned int)consensus[i*(I.WIDTH - x) + j] << "\t";			
			
		}
		cout << "\n";
	}
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
	
	get_auto(I);
	
	return 0 ;	
}
