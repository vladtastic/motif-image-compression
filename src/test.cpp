#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <omp.h>
#include <sys/time.h>
#include <x86intrin.h>
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

void get_occurrence(Image &I)
{
  // Declare sizes
  int MW = 10; // Motif width
  int MH = 10; // Motif height
  int IW = I.WIDTH; // Image width
  int IH = I.HEIGHT; // Image height
  int I_Size = IW*IH; // Size of image
  int M_Size = IW*(MH-1) + MW; // Size of padded motif
  int P_Size = I_Size - M_Size + 1; // Size of product string

  // Calculate pattern matches for each possible alphabet characters ***
  uint8_t* Pattern_Match[128]; // One pattern match for each alphabet
  for(uint8_t i = 0; i < 128; i++)
  {
    Pattern_Match[i] = (uint8_t*)malloc(I_Size);
    for(int j = 0; j < I_Size; j++)
    {
      Pattern_Match[i][j] = ((uint8_t)I.p[j] == (uint8_t)i) ? (uint8_t)0 : (uint8_t)1;
    }
//    if(i == 64)
//      for(int j = 0; j < I_Size; j++)
//        cout << (int)Pattern_Match[i][j] << endl;
  }

  // Make Motif string
  uint8_t* Image = I.p; // Pointer to the Image
  uint8_t* Motif = (uint8_t *)malloc(M_Size);
  int x = IW - MW; // Starting location of bite
  int y = IH - MH;
  int k = 0;
  for (int j = y; j < IH-1; j++) // Loop through all but the last unpaddded row
  {
    for (int i = x; i < IW; i++)
    {
      Motif[k++] = Image[j*IW + i];
    }
    for (int i = 0; i < x; i++) // Padding
    {
      Motif[k++] = (uint8_t)255;
    }
  }
  for (int i = x; i < IW; i++) // Last row
  {
    Motif[k++] = Image[(IH-1)*IW + i];
  }

  // Calculate product string 
  uint8_t* Product = (uint8_t *)calloc(P_Size,1);
  for(int i = 0; i < M_Size; i++)
  {
    if(Motif[i] == (uint8_t)255)  // Skip if current pixel is a don't care
      continue;
    for(int k = 0; k < P_Size; k++) // ***
    {
      Product[k] = (Product[k] == (uint8_t)1) ? (uint8_t)1 : Pattern_Match[Motif[i]][k+i]; // Conditional statement to prevent overflow
    }
  }
  /*
  for(int k = 0; k < P_Size; k++)
  { 
    if(Product[k] == (uint8_t)0 && k%I.WIDTH <= x)
    {
      cout << k % I.WIDTH << "\t" << k / I.WIDTH << endl;
    }
  }*/
}

void get_occurrence_sse(Image &I)
{
  // Declare sizes
  int MW = 100; // Motif width
  int MH = 100; // Motif height
  int IW = I.WIDTH; // Image width
  int IH = I.HEIGHT; // Image height
  int I_Size = IW*IH; // Size of image
  int M_Size = IW*(MH-1) + MW; // Size of padded motif
  int P_Size = I_Size - M_Size + 1; // Size of product string

  // Calculate pattern matches for each possible alphabet characters ***
  uint8_t* Pattern_Match[128];// One pattern match for each alphabet
  for(uint8_t i = 0; i < 128; i++)
  {
    Pattern_Match[i] = (uint8_t*)malloc(I_Size);
    uint8_t* Image = I.p; // Pointer to the Image
    uint8_t* P_Match = Pattern_Match[i]; // Pointer to match phrase
    const __m128i character = _mm_set1_epi8(i);
    for(size_t j = 0; j < I_Size; j += 8, P_Match += 8, Image += 8)
    {
      const __m128i row = _mm_loadu_si128((const __m128i*)&Image);
      const __m128i mask = _mm_andnot_si128(_mm_cmpeq_epi8(character,row), _mm_set1_epi8(0xff));
      _mm_storeu_si128((__m128i*)P_Match, mask);
    }
//    if(i == 64)
//      for(int j = 0; j < I_Size; j++)
//        cout << (int)Pattern_Match[i][j] << endl;
  }

  // Make Motif string
  uint8_t* Motif = (uint8_t *)malloc(M_Size);
  uint8_t* Image = I.p;
  int x = IW - MW; // Starting location of bite
  int y = IH - MH;
  int k = 0;
  for (int j = y; j < IH-1; j++) // Loop through all but the last unpaddded row
  {
    for (int i = x; i < IW; i++)
    {
      Motif[k++] = Image[j*IW + i];
    }
    for (int i = 0; i < x; i++) // Padding
    {
      Motif[k++] = (uint8_t)255;
    }
  }
  for (int i = x; i < IW; i++) // Last row
  {
    Motif[k++] = Image[(IH-1)*IW + i];
  }

  // Calculate product string 
  uint8_t* Product = (uint8_t *)calloc(P_Size,1);
  for(int i = 0; i < M_Size; i++)
  {
    if(Motif[i] == (uint8_t)255)  // Skip if current pixel is a don't care
      continue;
    for(int k = 0; k < P_Size; k++) // ***
    {
      Product[k] = (Product[k] == (uint8_t)0xff) ? (uint8_t)0xff : Pattern_Match[Motif[i]][k+i]; // Conditional statement to prevent overflow
    }
  }
  /*
  for(int k = 0; k < P_Size; k++)
  { 
    if(Product[k] == (uint8_t)0 && k%I.WIDTH <= x)
    {
      cout << k % I.WIDTH << "\t" << k / I.WIDTH << endl;
    }
  }*/
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

	struct timeval t0, t1, t2;
	double elapsed1,elapsed2;

	gettimeofday(&t0, NULL);	
	
	//get_auto(I);

  get_occurrence(I);
	
	gettimeofday(&t1, NULL);

  get_occurrence_sse(I);
  
  gettimeofday(&t2, NULL);
	
	elapsed1 = (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1e6;
  elapsed2 = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;

	cout << "Serial Timer: " << elapsed1 << endl;
  cout << "SSE Timer: " << elapsed2 << endl;
	
	return 0 ;	
}
