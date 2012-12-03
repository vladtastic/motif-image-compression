#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include "Image.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cout << "Usage : bin/exec filename\n";
		return 0;
	}
	char* filename = argv[1];
	Image I(filename); 
	
	uint8_t *pix = I.p;
	
	for(int i = 0; i < I.HEIGHT ; i++)
	{
		for(int j = 0 ; j < I.WIDTH ; j++)
		{
			cout << (unsigned int)pix[i * I.WIDTH + j] << "\t";
		}
		cout << "\n";
	}
	
	return 0 ;	
}
