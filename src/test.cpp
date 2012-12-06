#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <omp.h>
#include <sys/time.h>
#include <x86intrin.h>
#include "Image.h"
#include "hpcdefs.hpp"

using namespace std;

void get_occurrence(Image &I)
{
  // Declare sizes
  int MW = 16; // Motif width
  int MH = 16; // Motif height
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
    uint8_t* Image = I.p;
    for(int j = 0; j < I_Size; j++, Image++)
    {
      Pattern_Match[i][j] = (*Image == (uint8_t)i) ? (uint8_t)0 : (uint8_t)1;
    }
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

  for(int k = 0; k < P_Size; k++)
  { 
    if(Product[k] == (uint8_t)0 && k%I.WIDTH <= x)
    {
      cout << k % I.WIDTH << "\t" << k / I.WIDTH << endl;
    }
  }
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
  uint8_t* Pattern_Match[128]; // One pattern match for each alphabet
  for(uint8_t i = 0; i < 128; i++)
  {
    Pattern_Match[i] = (uint8_t*)aligned_alloc(I_Size*sizeof(uint8_t),32);
    uint8_t* Image = I.p;
    uint8_t* Pattern = Pattern_Match[i];
    const __m128i mask = _mm_set1_epi8(i);
    for(int j = 0; j < I_Size; j+=16, Image+=16, Pattern+=16)
    {
      __m128i row = _mm_load_si128((__m128i*)Image);
      _mm_store_si128((__m128i*)Pattern,_mm_cmpeq_epi8(row,mask));
    }
  }

  // Make Motif string
  uint8_t* Image = I.p; // Pointer to the Image
  uint8_t* Motif = (uint8_t *)aligned_alloc(M_Size,32);
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
  uint8_t* Product = (uint8_t *)aligned_alloc(P_Size,32);
  int Pl = P_Size-(P_Size%16);
  int i;
  uint8_t* P = Product;
  for(int i = 0; i< Pl; i += 16, P += 16)
    _mm_store_si128((__m128i*)P,_mm_set1_epi8(0xff));
  for(int i = Pl; i < P_Size; i++, P++)
    *P = 255;

  for(int i = 0; i < M_Size; i++)
  {
    if(Motif[i] == (uint8_t)255)  // Skip if current pixel is a don't care
      continue;
    uint8_t* P = Product;
    uint8_t* M = Pattern_Match[Motif[i]] + i;
    int k;
    for(k = 0; k<Pl; k += 16, P += 16, M += 16)
    {
      _mm_store_si128((__m128i*)P, _mm_and_si128(_mm_load_si128((__m128i*)P),_mm_loadu_si128((__m128i*)M)));
    }
    for(k = Pl; k < P_Size; k++, P++, M++)
    {
      (*P) = (*P) & (*M);
    }
  }
  
  for(int k = 0; k < P_Size; k++)
  { 
    if(Product[k] == (uint8_t)255 && k%I.WIDTH <= x)
    {
      cout << k % I.WIDTH << "\t" << k / I.WIDTH << endl;
    }
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
