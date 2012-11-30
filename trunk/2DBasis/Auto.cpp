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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "BMPImage.h"
#include "ImageFactory.h"
#include "MotifPatternCore.h"
#include "MyTrie.h"
#include "MyCore.h"
#include "CompactTrie.h"

using namespace std;

///String to integer function conversion.
/*!
 * Converts each digit char and adds into result.
 *\param digit string rappresenting integer to be converted.
 *\param result output integer conversion.
 *\return returns true if conversion works fine.
*/
set<int>basis;

bool string2int(char* digit, int& result) {
   result = 0;

   //--- Convert each digit char and add into result.
   while (*digit >= '0' && *digit <='9') {
      result = (result * 10) + (*digit - '0');
      digit++;
   }

   //--- Check that there were no non-digits at end.
   if (*digit != 0) {
      return false;
   }

   return true;
}

///Prints on standard output the correct usage of console application.
/*!
Usage 1: for use on random char array\n
Usage 1: 2dIme -height <h> -width <w> [-simbols s] [-seed r] [-optimize] \n\n
Usage 2: for use on bitmap file image\n
Usage 2: 2dIme <file_name> [options] <-base|-autocorrelation>\n
*/
void printUse(char* prog){
	cout << endl;
	cout << " Usage 1: for use on random char array\n Usage 1: "<<prog<<" -height <h> -width <w> [-simbols s] [-seed r] [-optimize]" << endl << endl;
	cout << " Usage 2: for use on bitmap file image\n Usage 2: "<<prog<<" <file_name> [options] <-base|-autocorrelation>" << endl;
	cout << " Usage 2: options " << endl;
	cout << " ----------------------------------------------------------------------- " << endl;
	cout << " -up U (int 0<U<image_height)" << endl;
	cout << "       row index of the bite on which computing the autocorrelations" << endl << endl;
	cout << " -sx S (int 0<S<image_width)" << endl;
	cout << "       column index of the bite on which computing the autocorrelations" << endl << endl;
	cout << " -limit L (int 0<S<MIN[image_height,image_width])" << endl;
	cout << "        offset to limit the autocorrelation number" << endl << endl;
	cout << " -maxrows R (int 0<R<image_height)" << endl;
	cout << "        maximum size of rows for the motifs in the basis" << endl << endl;
	cout << " -maxcols R (int 0<R<image_width)" << endl;
	cout << "        maximum size of columns for the motifs in the basis" << endl << endl;
	cout << " -minrows R (int 0<R<image_height)" << endl;
	cout << "        minimum size of rows for the motifs in the basis" << endl << endl;
	cout << " -mincols R (int 0<R<image_width)" << endl;
	cout << "        minimum size of columns for the motifs in the basis" << endl << endl;
	cout << " -maxdontcare D (int 0<D<image_heightximage_width)" << endl;
	cout << "        maximum don't care number for the motifs in the basis" << endl << endl;
	cout << " -maxdontcare D (int 0<D<image_heightximage_width)" << endl;
	cout << "        maximum don't care number for the motifs in the basis" << endl << endl;
	cout << " -output D " << endl;
	cout << "        path of the folder where the images of the basis will be stored" << endl << endl;

}

///Main program console application.
int main(int argc, char *argv[]) {

//	testTrie();
//	cout << "fine testTrie" << endl;
//	return 0;
	//set<int> basis;

	//////////////////////////////////////
	// no parameter on command line, run an example.
	if(argc <= 2){

		bool optimize = true;

		if(argc == 2 && strcmp(argv[1], "-optimize") == 0)
			optimize = true;

		int size = 4;
		int size_2 = 3;
		ImageFactory<char> char_factory(2, 'a', 27);
		char** ranchar = char_factory.get_image(size, size_2);

 		string r1="aab\n";
  		string r2="bbb\n";
  		string r3="aab\n";
  		string r4="bab\n";

		ranchar[0] = (char*)r1.c_str();
		ranchar[1] = (char*)r2.c_str();
		ranchar[2] = (char*)r3.c_str();
		ranchar[3] = (char*)r4.c_str();

		print_image(ranchar, size, size_2);
		MyTrie<char> maximal = MyTrie<char>();
		//Extract all bases an print on standard output the last one.
		process(ranchar, size, size_2, '.', -1, -1, -1, -1, -1, optimize, true, &basis, maximal);
		cout << optimize << endl;

		//Extract base at (0,2)
		//CompactTrie<char> b = getBase(ranchar, size, size_2, '.', 0, 2, -1, -1, -1, -1, -1, optimize, true);
		//Print on standard output the base.
		//b.getValues();

		return 0;
	}
	//////////////////////////////////////////////////////////////////
	//read parameter on command line

	int up = 0;
	int sx = 0;
	int limit = -1;
	bool optimize = true;

	if(argc < 2){
		printUse(argv[0]);
		return 0;
	}

	int height = -1;
	int width = -1;
	int simbols = 3;
	int seed = 7;
	for (int argp = 1; argp < argc; argp++) {
		if (strcmp(argv[argp], "-height") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], height);
			if(!done){
				break;
			}
		}else if (strcmp(argv[argp], "-width") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], width);
			if(!done){
				break;
			}
		}else if (strcmp(argv[argp], "-simbols") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], simbols);
			if(!done){
				break;
			}
		}else if (strcmp(argv[argp], "-seed") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], seed);
			if(!done){
				break;
			}
		}else if(strcmp(argv[argp], "-optimize") == 0) {
			argp++;
			optimize = true;
		}else{
			break;
		}
	}

	if(height != -1 && width != -1 && simbols != -1 && seed != -1){
		ImageFactory<char> _factory(simbols, 'a', seed);
		char** ran = _factory.get_image(height, width);
		print_image(ran, height, width);
		MyTrie<char> maximal = MyTrie<char>();
		process(ran, height, width, '.', -1, -1, -1, -1, -1, optimize, true, &basis, maximal);
		cout << optimize << endl;
		return 0;
	}

	BMPImage file_image = BMPImage(argv[1]);
	int** image = file_image.read_int();

	bool write_autoc = false;
	bool write_base = false;
	bool print_base = false;
	int max_row_size = -1;
	int max_col_size = -1;
	int min_row_size = -1;
	int min_col_size = -1;
	int max_n_dontcare = -1;
	bool on_file = false;
	char* filename;


	for (int argp = 2; argp < argc; argp++) {
		if (strcmp(argv[argp], "-up") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], up);
			if(!done || up >file_image.get_h()){
				up = 0;
				cout << "warning: " << argv[argp] << " use: " << up << endl;
			}
		}else if(strcmp(argv[argp], "-sx") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], sx);
			if(!done || sx >file_image.get_w()){
				sx = 0;
				cout << "warning: " << argv[argp] << " use: " << sx << endl;
			}
		}else if(strcmp(argv[argp], "-limit") == 0) {
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], limit);
			if(!done || limit >file_image.get_w() || limit > file_image.get_h()){
				limit = -1;
				cout << "warning: " << argv[argp] << " use: " << limit << endl;
			}
		}else if(strcmp(argv[argp], "-maxrows") == 0) {
			optimize = true;
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], max_row_size);
			if(!done || max_row_size >file_image.get_w()){
				max_row_size = -1;
				cout << "warning: " << argv[argp] << " use: " << max_row_size << endl;
			}
		}else if(strcmp(argv[argp], "-minrows") == 0) {
			optimize = true;
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], min_row_size);
			if(!done || min_row_size < 0){
				min_row_size = -1;
				cout << "warning: " << argv[argp] << " use: " << min_row_size << endl;
			}
		}else if(strcmp(argv[argp], "-maxcols") == 0) {
			optimize = true;
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], max_col_size);
			if(!done || max_col_size >file_image.get_h()){
				max_col_size = -1;
				cout << "warning: " << argv[argp] << " use: " << max_col_size << endl;
			}
		}else if(strcmp(argv[argp], "-mincols") == 0) {
			optimize = true;
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], min_col_size);
			if(!done || min_col_size < 0){
				min_col_size = -1;
				cout << "warning: " << argv[argp] << " use: " << min_col_size << endl;
			}
		}else if(strcmp(argv[argp], "-maxdontcare") == 0) {
			optimize = true;
			argp++;
			bool done = false;
			if (argp < argc)
				done = string2int(argv[argp], max_n_dontcare);
			if(!done || max_n_dontcare >file_image.get_h()*file_image.get_w()){
				max_n_dontcare = -1;
				cout << "warning: " << argv[argp] << " use: " << max_n_dontcare << endl;
			}
		}else if(strcmp(argv[argp], "-output") == 0) {
			argp++;
			if (argp < argc){
				filename = argv[argp];
				on_file = true;
				cout << "output: " << argv[argp] << endl;
			}else{
				cout << "warning: " << endl;
			}
		}else if(strcmp(argv[argp], "-base") == 0) {
			argp++;
			write_base = true;
			if(on_file)
				print_base = false;
			else{
				write_base = false;
				print_base = true;
			}

		}else if(strcmp(argv[argp], "-autocorrelation") == 0) {
			argp++;
			write_autoc = true;
			print_base = false;
			write_base = false;
		}else{
			printUse(argv[0]);
			return 0;
		}
	}
	MyTrie<int> maximal=MyTrie<int>();
	cout << optimize << endl;
	if(write_autoc)
		write_autocorrelation(filename, image, up, sx, file_image.get_h(), file_image.get_w(), limit);
	if(write_base)
		process_and_save(filename, image, file_image.get_h(), file_image.get_w(), 0xFFFFFF,
				max_row_size, max_col_size, max_n_dontcare, min_row_size, min_col_size, optimize, false);
	if(print_base)
		process(image, file_image.get_h(), file_image.get_w(), 0xFFFFFF,
				max_row_size, max_col_size, max_n_dontcare, min_row_size, min_col_size, optimize, false, &basis, maximal);
}
