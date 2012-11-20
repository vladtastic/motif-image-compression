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

#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <math.h>
#include <string.h>
#include <set>
#include "MyTrie.h"
#include "MyCore.h"
#include "CompactTrie.h"

#ifndef MOTIFPATTERNCORE_H_
#define MOTIFPATTERNCORE_H_

using namespace std;

///Print on standard output an autocorrelation.
/*!
 * Print on standard output autocorrelation between bite at (sup, sx) and image img with size height*width.
 * \n\b Note: it's possible to simulate printing on standard output for testing algorithm performance
 *\param sup first row index occurrence of the autocorrelation.
 *\param sx first column index occurrence of the autocorrelation.
 *\param height image height.
 *\param width image width.
 *\param dontcare don't care autocorrelation symbol.
 *\param simulate testing algorithm performance flag.
 *
 */
template<typename T>
void get_autocorrelation(T** img, int sup, int sx, int height, int width,
		T dontcare, int max_rows, int max_cols, int max_dontcare, int min_rows, int min_cols, MyTrie<T>& autocorrelations, set<int>& indexes, bool simulate = false) {
	int rows;
	int cols;
	int i = 0;
	int j = 0;
	int x = 0;
	int n_dontcare = 0;
	for (i = sup; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (i == sup && j < sx)
				j = sx;
		if(!(i==0 && j==0)){
			T** autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
					cols, n_dontcare, true, max_rows, max_cols, max_dontcare,
						min_rows, min_cols);
			if (rows > 0 && cols > 0) {
				if (simulate) {

				} else {

					int positions=autocorrelations.find(autoc, rows, cols, -1);

				   if(positions==-1){

					int* c;
					int* d;
					int size_c=0;
					int size_d=0;

					containMotif(img, autoc, height, width, rows, cols, &c, &d, size_c, size_d, dontcare);

					int now_size=size_c*2;

					int* up_occ= new int[now_size];
					int pos=0;
					for(int k=0;k<size_c;k++){
							up_occ[pos]=c[k];
							pos++;
							up_occ[pos]=d[k];
							pos++;
					}

					int index;

					autocorrelations.insElements(autoc, rows, cols, up_occ, now_size, index);
					indexes.insert(index);}
				}

				for (x = 0; x < rows; x++)
					delete autoc[x];
				delete autoc;
			}
			// second type of autocorrelation (j = -j)
			if(j != 0){
				autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
					cols, n_dontcare, true, max_rows, max_cols, max_dontcare,
						min_rows, min_cols);
					if (rows > 0 && cols > 0) {
						if (simulate) {

						} else {

						int positions=autocorrelations.find(autoc, rows, cols, -1);

						if(positions==-1){
							int* c;
							int* d;
							int size_c=0;
							int size_d=0;

							containMotif(img, autoc, height, width, rows, cols, &c, &d, size_c, size_d, dontcare);

							int now_size=size_c*2;

							int* up_occ= new int[now_size];
							int pos=0;
							for(int k=0;k<size_c;k++){
								up_occ[pos]=c[k];
								pos++;
								up_occ[pos]=d[k];
								pos++;
								}

							int index;
							autocorrelations.insElements(autoc, rows, cols, up_occ, now_size, index);
							indexes.insert(index);}
						}

						for (x = 0; x < rows; x++)
							delete autoc[x];
						delete autoc;
					}
			}
		}
		if (simulate)
			cout << "    DEBUG: " << i << " " << j << endl;
	}
	}
}

///Print on standard output all autocorrelations.
/*!
 * Print on standard output all autocorrelations of image img with size height*width.
 * \n\b Note: it's possible to simulate printing on standard output for testing algorithm performance
 *\param height image height.
 *\param width image width.
 *\param dontcare don't care autocorrelation symbol.
 *\param simulate testing algorithm performance flag.
 *
 */
template<typename T>
void get_Base(T** img, int height, int width,
		T dontcare, int max_rows, int max_cols, int max_dontcare, int min_rows, int min_cols, MyTrie<T>& autocorrelations, set<int>& irredundant, bool simulate = false) {

	set<int> indexes= set<int> ();
	get_autocorrelation(img, 0, 0, height, width, dontcare, max_rows, max_cols, max_dontcare, min_rows, min_cols, autocorrelations, indexes, simulate);
  	autocorrelations.printPaths();

	for(set<int>::iterator it = indexes.begin(); it != indexes.end(); it++){

		  T** mi=autocorrelations.getMotif(*it);
		  int rows1=autocorrelations.getRows(*it);
		  int cols1=autocorrelations.getCols(*it);
		  int n_occ_mi=autocorrelations.getNOccurence(*it);

		  int* occ_mi=autocorrelations.getOccurence(*it);
		  int* occ_meta;
		  int n_occ_meta=0;

		  for(set<int>::iterator it1 = indexes.begin(); it1 != indexes.end(); it1++){
			if((*it1)!= (*it)){

				T** mj=autocorrelations.getMotif(*it1);
				int rows2=autocorrelations.getRows(*it1);
				int cols2=autocorrelations.getCols(*it1);

				int* c;
				int* d;
				int size_c=0;
				int size_d=0;

				if(containMotif(mj, mi, rows2, cols2, rows1, cols1, &c, &d, size_c, size_d, dontcare)){
					int n_occ_mj=autocorrelations.getNOccurence(*it1);
					int* occ_mj=autocorrelations.getOccurence(*it1);

				    for(int index_c=0; index_c<size_c; index_c++){
						int offset_row=c[index_c];
						int offset_col=d[index_c];

						int* occ_temp=new int[n_occ_mj];

						for(int p=0;p<n_occ_mj-1;p+=2){
							occ_temp[p]=occ_mj[p]+offset_row;
							occ_temp[p+1]=occ_mj[p+1]+offset_col;
						}

						if(n_occ_meta==0){
							n_occ_meta=n_occ_mj;
							occ_meta=new int[n_occ_meta];

							for(int i=0;i<n_occ_meta;i++)
								occ_meta[i]=occ_temp[i];
						}
						else{
							int n_occ_meta_merged=n_occ_meta+n_occ_mj;
							int* occ_meta_merged=new int[n_occ_meta_merged];

							occ_meta_merged=MyTrie<T>::mergeOccurence(occ_meta, n_occ_meta, occ_temp, n_occ_mj);

							delete[] occ_meta;
							occ_meta=occ_meta_merged;
						}//else


					}//for

					delete[] occ_mj;
					occ_mj=NULL;
				}//if



			for (int x = 0; x < rows2; x++)
				delete mj[x];
				delete mj;


			}//if



		}//for

		bool redundant=true;

		for(int i=0; (i<n_occ_mi-1) && redundant; i+=2){
			bool found=false;
			for(int j=0; (j<n_occ_meta-1) && !found; j+=2){
				if((occ_mi[i]==occ_meta[j]) && (occ_mi[i+1]==occ_meta[j+1])) found=true;
			}
		    if(!found) redundant=false;
		}

		if(!redundant)
		irredundant.insert(*it);

for (int x = 0; x < rows1; x++)
				delete mi[x];
				delete mi;
delete[] occ_mi;
occ_mi=NULL;
	}//for

	for (set<int>::iterator it = irredundant.begin(); it != irredundant.end(); it++)
	  			autocorrelations.navigate(*it);

}//



///Release memory allocated by get_L.
/*!
 * Used to release memory allocated by get_L. \n\b Note: unused with listNaive approach.
 * \param L vector that will be deallocated.
 * \param height image height.
 * \param width image width.
 * \see get_L.
 */
void free_L(int**** L, int height, int width) {
	for(int i = 0; i< height; i++){
		for(int j = 0; j< height; j++){
			for(int k = 0; k< width; k++){
				delete L[i][j][k];
			}
			delete L[i][j];
		}
		delete L[i];
	}
	delete L;
}

///Allocate a size-dimesional vector.
/*!
 * Inizialize a size-dimensional vector l<i, i', h, q>. \n\b Note: unused with listNaive approach.
 * \param height image height.
 * \param width image width.
 * \param intValue init value.
 */
int**** get_L(int height, int width, int intValue) {

	int**** L = new int***[height];

	for(int i = 0; i< height; i++){
		L[i] = new int**[height];
	}

	for(int i = 0; i< height; i++){
		for(int j = 0; j< height; j++){
			L[i][j] = new int*[width];
		}
	}

	for(int i = 0; i< height; i++){
		for(int j = 0; j< height; j++){
			for(int k = 0; k<width; k++){
				L[i][j][k] = new int[height];
			}
		}
	}

	for(int i = 0; i< height; i++){
		for(int j = 0; j< height; j++){
			for(int k = 0; k<width; k++){
				for(int t = 0; t< height; t++){
					L[i][j][k][t] = intValue;
				}
			}
		}
	}

	return L;
}

///Print on standard output the bite (i,j) of image img with size height x width.
/*!
 * The method prints on standard output a bite.
 * \param img input image.
 * \param i first bite coordinate.
 * \param j second bite coordinate.
 * \param height image height.
 * \param width image width.
 */
template<typename T>
void printBite(T** img, int i, int j, int height, int width) {
	for(int k = 0; k<j; k++)
		cout << "  ";
	for(; i < height; i++){
		for(; j<width; j++){
			cout << img[i][j] << " ";
		}
		j = 0;
		cout << endl;
	}
}

///Print on standard output the core (i,j) of image img with size height x width.
/*!
 * The method prints on standard output the core of an image.
 * \param img input image.
 * \param i first core coordinate.
 * \param j second core coordinate.
 * \param height image height.
 * \param width image width.
 */
template<typename T>
void printCore(T** img, int i, int j, int height, int width) {
	int jj = j;
	for(; i < height; i++){
		for(; j<width; j++){
			cout << img[i][j] << " ";
		}
		j = jj;
		cout << endl;
	}
}

///Print on standard output the first row of bite (i,j) of image img with size height x width.
/*!
 * The method prints on standard output a row.
 * \param img input image.
 * \param i first core coordinate.
 * \param j second core coordinate.
 * \param height image height.
 * \param width image width.
 */
template<typename T>
void printRow(T** img, int i, int j, int height, int width) {
	for(int k = 0; k<j; k++)
		cout << ". ";
	for(; j<width; j++){
		cout << img[i][j] << " ";
	}
	cout << endl;
}

///Print on standard output the row j of image img with size width.
/*!
 * The method prints on standard output a row.
 * \param img input image.
 * \param j the row to print on standard output.
 * \param width image width.
 */
template<typename T>
void printRow(T* img, int j, int width) {
	for(; j<width; j++){
		cout << img[j] << " ";
	}
	cout << endl;
}

///To be completed. \n\b Note: unused with listNaive approach.
//  per ora verifica solo le righe, bisogna fare lo stesso per le colonne.
//  TODO: cambiare "aggiorna_L" in "aggiornaLRows" ed aggiungere "aggiornaLCols"
//	non è utilizzato con l'approccio listNaive
template<typename T>
void aggiorna_L(T** img, int**** L, int sup, int sx, int height, int width, T dontcare, bool debug = false) {
	T* curr = new T[width];

	for(int x = 0; x< sx; x++){
		curr[x] = dontcare;
	}
	for(int x = sx; x< width; x++){
		curr[x] = img[sup][x];
	}

	for(int x = sup; x<height; x++){
		for(int y = x; y<height; y++){
			if(debug){
				cout << endl;
				cout << "f-c: " << sup << " g-d: " << sx << endl;
				printBite(img, sup, sx, height, width);
				cout << endl;
			}
			for(int h = 0; h<width; h++){
				if(y!=sup && y!=x){
					if(x == sup)
						L[sup][x][h][y] = calcola_L_rows(width, dontcare, curr, curr, h, img[y]);
					else
						L[sup][x][h][y] = calcola_L_rows(width, dontcare, curr, img[x], h, img[y]);
					if(debug){
						cout << "L<" << sup << ", " << x<< ", " << h << ", "
						<< y << "> = " << L[sup][x][h][y] << endl;
					}
				}
				if(y!=x && y!=sup){
					if(x == sup){
							//gia' calcolato
						}
					else{
						L[x][sup][h][y] = calcola_L_rows(width, dontcare, img[x], curr, h, img[y]);
						if(debug){
							cout << "L<" << x << ", " << sup<< ", " << h << ", "
							<< y << "> = " << L[x][sup][h][y] << endl;
						}
					}
				}
				if(sup!=x && sup!=y){
					L[x][y][h][sup] = calcola_L_rows(width, dontcare, img[x], img[y], h, curr);
					if(debug){
						cout << "L<" << x << ", " << y<< ", " << h << ", "
						<< sup << "> = " << L[x][y][h][sup] << endl;
					}
				}
			}
		}
	}
	delete curr;
}

///Used by aggiorna_L to update the single row.
/*!
 * To be completed. \n
 * \b Note: unused with listNaive approach.
 */

template<typename T>
int calcola_L_rows(int size, T dontcare, T* i, T* i_, int h, T* q, bool debug = false) {
	if(debug){
		cout << endl;
		cout << "row (i): ";
		printRow(i, 0, size);
		cout << "row(i'): ";
		printRow(i_, 0, size);
		cout << "row (q): ";
		printRow(q, 0, size);
		cout << " (i@i'): ";
	}

	//consensus between rows i and suffix i' must be in size the same of suffix.
	T* consenso = new T[size - h];
	//compute the consensus between row i and row i'
	for(int x = 0; x < size - h; x++){
		if(i[x] == i_[x + h]){
			consenso[x] = i[x];
		}else{
			consenso[x] = dontcare;
		}
		if(debug)
			cout << consenso[x] << " " ;
	}
	if(debug){
		cout << endl;
		cout << "!(i@i'): ";
		for(int x = 0; x < size - h; x++){
			//DEBUG printing of rotated consensus
			cout << consenso[size-h-x-1] << " ";
		}
		cout << endl;
		cout << "!( q  ): ";
	}
	//mismatch= -1 means that no mismatch are founded between consensus and row q
	int mismatch = -1;
	//compare consensus with row q in reverse order and remember the last mismatch
	for(int y = 0; y < size - h; y++){
		if(consenso[size - h - y -1] == dontcare || q[size - y -1] == dontcare ){
			//no mismatch.
		}else if(consenso[size - h - y -1] != q[size - y -1]){
			mismatch = y;
		}
		if(debug)
			cout << q[size - y -1] << " " ;

	}
	if(debug)
		cout << endl;
	delete consenso;
	return mismatch;
}

///Get a specified base of motifs.
/*!Generate the specified base of image img with size height x width using incremental approach and
 * return it as . \n
 * DEBUG prints can be enabled by debug flag and don't care symbol can be specified by dontcare generics attribute.
 * \param img input image.
 * \param height image height.
 * \param width image width.
 * \param x_i row index of base set to extract.
 * \param y_j colum index of base set to extract.
 * \param dontcare don't care symbol.
 * \param max_rows maximal rows limit for motif that could be extracted.
 * \param max_cols maximal columns limit for motif that could be extracted.
 * \param max_dontcare maximal dontcare limit for motif that could be extracted.
 * \param min_rows minimal rows limit for motif that could be extracted.
 * \param min_cols minimal columns limit for motif that could be extracted.
 * \param optimize flag for enabling optimization in motif extraction.
 * \param debug flag used to enable debug.
 * \return returns a couple of two pointers. The former is a pointer to a identifiers base set,
 *  the latter is a pointer to a MyTrie structure.
 */
template<typename T>
CompactTrie<T> getBase(T** img, int height, int width, T dontcare, int x_i, int y_j,
		int max_rows, int max_cols, int max_dontcare,
		int min_rows, int min_cols, bool optimize, bool debug) {

	if(debug)
		cout << endl << "start process" << endl << endl;

	unsigned int sizeOfL = (height*height*height*width)* sizeof(int);

	//TODO bool useL = true  always Naive approach
	bool useL = false;
	int**** L = NULL;
	if(sizeOfL > 1000000000){
		cout << "max size of L: 1GB" << endl;
		useL = false;
	}
	if(debug && useL)
		cout << "creating " << sizeOfL << " bytes for L..." << endl;

	if(useL){
		L = get_L(height, width, NULL);
		if(debug)
			cout << "created L of: "<< sizeOfL << " bytes!" << endl;
	}

	MyCore<T> core = MyCore<T>();
	MyTrie<T> maximal = MyTrie<T>();
	set<int> maximalxy;
	set<int> novel;
	set<int> base;

	for(int x = height - 1; x >= x_i; x--){
		if(debug)
			cout << "x." << x << " " << endl;
		for(int y = width - 1; y >= y_j; y--){

			//maximal.printPaths();

			//contains only novel motifs.
			novel = set<int>();

			//contains maximal motifs at (x,y)
			maximalxy = set<int>();

			if(useL)
				aggiorna_L(img, L, x, y, height, width, dontcare);

			compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
					max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

			//contains new motif but not novels.
			set<int> newMotif = set<int>();

			//contains maximal motif already present in base.
			set<int> oldMotif = set<int>();

			separate(maximal, base, maximalxy, newMotif, oldMotif, dontcare, x, y);

			extract(maximal, base, novel, newMotif, oldMotif, dontcare);

			/**/
		}
	}

	if(useL)
		free_L(L, height, width);

	if(debug)
		cout << endl << "end process" << endl;

	return CompactTrie<T>(&maximal, base, dontcare);

}

///Main method for 2D irredundant motif extraction.
/*!Generate all bases of image img with size height x width using incremental approach and
 * print on standard output the list of motifs for each base. \n
 * DEBUG prints can be enabled by debug flag and don't care symbol can be specified by dontcare generics attribute.
 * \param img input image.
 * \param height image height.
 * \param width image width.
 * \param dontcare don't care symbol.
 * \param max_rows maximal rows limit for motif that could be extracted.
 * \param max_cols maximal columns limit for motif that could be extracted.
 * \param max_dontcare maximal dontcare limit for motif that could be extracted.
 * \param min_rows minimal rows limit for motif that could be extracted.
 * \param min_cols minimal columns limit for motif that could be extracted.
 * \param optimize flag for enabling optimization in motif extraction.
 * \param debug flag used to enable debug.
 */
template<typename T>
void process(T** img, int height, int width, T dontcare,
		int max_rows, int max_cols, int max_dontcare,
		int min_rows, int min_cols, bool optimize, bool debug) {

	if(debug)
		cout << endl << "start process" << endl << endl;

	unsigned int sizeOfL = (height*height*height*width)* sizeof(int);

	//TODO bool useL = true  always Naive approach
	bool useL = false;
	int**** L = NULL;
	if(sizeOfL > 1000000000){
		cout << "max size of L: 1GB" << endl;
		useL = false;
	}
	if(debug && useL)
		cout << "creating " << sizeOfL << " bytes for L..." << endl;

	if(useL){
		L = get_L(height, width, NULL);
		if(debug)
			cout << "created L of: "<< sizeOfL << " bytes!" << endl;
	}

	MyCore<T> core = MyCore<T>();
	MyTrie<T> maximal = MyTrie<T>();
	set<int> maximalxy;
	set<int> novel;
	set<int> base;

	for(int x = height - 1; x >= 0; x--){

		cout << "x." << x << " " << endl;
		for(int y = width - 1; y >= 0; y--){

			//maximal.printPaths();

			//contains only novel motifs.
			novel = set<int>();

			//contains maximal motifs at (x,y)
			maximalxy = set<int>();

			if(useL)
				aggiorna_L(img, L, x, y, height, width, dontcare);

			compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
					max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

			if(debug && false){
				cout << "Novelli{ " <<endl;
				for(set<int>::iterator it = novel.begin(); it != novel.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			//contains new motif but not novels.
			set<int> newMotif = set<int>();

			//contains maximal motif already present in base.
			set<int> oldMotif = set<int>();

			separate(maximal, base, maximalxy, newMotif, oldMotif, dontcare, x, y);

			extract(maximal, base, novel, newMotif, oldMotif, dontcare);

			if(debug && false){
				cout << "Base{ " <<endl;
				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			//write_base(base, maximal);

			/**/
			if(debug){
				cout << "Base " << x << ", " << y << " { " <<endl;
				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					cout << *it << ": " ;
					maximal.navigate(*it);
				}
				cout << endl << "}" << endl;
			}
			/**/
		}
	}
	if(debug){
		//cout << endl << endl << "TRIE STATUS" << endl << endl;

		cout << "Maximal elems: {" << endl;
		maximal.printPaths();
		cout << "}" << endl;

		//core.print();
	}
	cout << "Last Base{ " <<endl;
	for(set<int>::iterator it = base.begin(); it != base.end(); it++){
		cout << *it << ": " ;
		maximal.navigate(*it);
	}
	cout << endl << "}" << endl;

	if(useL)
		free_L(L, height, width);

	if(debug)
		cout << endl << "end process" << endl;

}


template<typename T>
void process_Modified(T** img, int height, int width, T dontcare,
		int max_rows, int max_cols, int max_dontcare,
		int min_rows, int min_cols, bool optimize, bool debug, set<int>* basis, MyTrie<T>& maximal){

	if(debug)
		cout << endl << "start process" << endl << endl;

	unsigned int sizeOfL = (height*height*height*width)* sizeof(int);

	//TODO bool useL = true  always Naive approach
	bool useL = false;
	int**** L = NULL;
	if(sizeOfL > 1000000000){
		cout << "max size of L: 1GB" << endl;
		useL = false;
	}
	if(debug && useL)
		cout << "creating " << sizeOfL << " bytes for L..." << endl;

	if(useL){
		L = get_L(height, width, NULL);
		if(debug)
			cout << "created L of: "<< sizeOfL << " bytes!" << endl;
	}

	MyCore<T> core = MyCore<T>();
	set<int> maximalxy;
	set<int> novel;
	set<int> base;

	for(int x = height - 1; x >= 0; x--){

		cout << "x." << x << " " << endl;
		for(int y = width - 1; y >= 0; y--){

			//contains only novel motifs.
			novel = set<int>();

			//contains maximal motifs at (x,y)
			maximalxy = set<int>();

			if(useL)
				aggiorna_L(img, L, x, y, height, width, dontcare);

			compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
					max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

			if(debug && false){
				cout << "Novelli{ " <<endl;
				for(set<int>::iterator it = novel.begin(); it != novel.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			//contains new motif but not novels.
			set<int> newMotif = set<int>();

			//contains maximal motif already present in base.
			set<int> oldMotif = set<int>();

			separate(maximal, base, maximalxy, newMotif, oldMotif, dontcare, x, y);

			extract(maximal, base, novel, newMotif, oldMotif, dontcare);

			if(debug && false){
				cout << "Base{ " <<endl;
				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			/**/
			if(debug){
				cout << "Base " << x << ", " << y << " { " <<endl;
				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					cout << *it << ": " ;
					maximal.navigate(*it);
					//maximal.printPaths();
				}
				cout << endl << "}" << endl;
			}
			/**/
		}
	}
	if(debug){
		//cout << endl << endl << "TRIE STATUS" << endl << endl;

		cout << "Maximal elems: {" << endl;
		maximal.printPaths();
		cout << "}" << endl;

		//core.print();
	}
	*basis=base;
	if(useL)
		free_L(L, height, width);
	if(debug)
		cout << endl << "end process" << endl;
}


///Extract procedure.
/*!
 * Generate the new base removing redundant motif.
 * \param maximal trie of all maximal motif.
 * \param base set of bese of irredundant motif.
 * \param novel set of novel motif.
 * \param newMotif set of new motif.
 * \param old motif set of old motif.
 * \param dontcare dontcare symbol.
 */
template<typename T>
void extract(MyTrie<T>& maximal, set<int>& base, set<int>& novel,
		set<int>& newMotif, set<int>& oldMotif, T dontcare) {

	CompactTrie<T> cc = CompactTrie<T>(&maximal, novel, dontcare);

	set<int>::iterator iter_old = oldMotif.begin();
	for(; iter_old != oldMotif.end(); iter_old++){
		T* motif = maximal.getLinearMotif(*iter_old);
		int size = maximal.getLinearSize(*iter_old);
		int* occ = maximal.getOccurence(*iter_old);
		int n_occ = maximal.getNOccurence(*iter_old);
		int id = cc.tryToInsValue(motif, size, occ, n_occ, dontcare);
		//cout << "id: " << id << " *it: " << *it << endl;
		if(id != -1)
			cc.my_trie_compact_trie[*iter_old] = id;
	}

	set<int>::iterator iter_new = newMotif.begin();
	for(; iter_new != newMotif.end(); iter_new++){
		T* motif = maximal.getLinearMotif(*iter_new);
		int size = maximal.getLinearSize(*iter_new);
		int* occ = maximal.getOccurence(*iter_new);
		int n_occ = maximal.getNOccurence(*iter_new);
		int id = cc.tryToInsValue(motif, size, occ, n_occ, dontcare);
		//cout << "id: " << id << " *it: " << *it << endl;
		if(id != -1)
			cc.my_trie_compact_trie[*iter_new] = id;
	}


	//inizialize the reference of the new base.
	base = set<int>();

	map<int, int>::iterator it = cc.my_trie_compact_trie.begin();
	for(; it != cc.my_trie_compact_trie.end(); it++ ){
		base.insert((*it).first);
	}

	//cc->getValues();
}


///Separate between old motif and new motif (but not novel).
/*!
 *From old motif eliminate motif without occourence at (i,j) that are submotif of old motif or new motif of old base.
 *From old motif without occurrences at (i,j) eliminate submotif of motif extracted at (i,j).
 * \param maximal trie of all maximal motif.
 * \param base set of bese of irredundant motif.
 * \param maximalxy set of motif extracted at (i,j).
 * \param newMotif set of new motif.
 * \param old motif set of old motif.
 * \param dontcare dontcare symbol.
 * \param i first coordinate of just extracted motif.
 * \param j second coordinate of just extracted motif.
*/
template<typename T>
void separate(MyTrie<T>& maximal, set<int>& base, set<int>& maximalxy,
		set<int>& newMotif, set<int>& oldMotif, T dontcare, int i, int j) {

	set<int> o_primo;
	set<int> o_secondo;

	for (set<int>::iterator it = base.begin(); it != base.end(); it++) {

		bool chk = 	(maximalxy.find(*it) != maximalxy.end());

		if(chk){
			//il motivo era già presente nella base
			//ed occorre ad (i,j) quindi lo inserisco tra gli oldMotif.
			o_primo.insert(*it);
			oldMotif.insert(*it);
		}else{
			//bisogna verificare se è un sottomotivo
			//non lo posso mettere subito tra gli oldMotif
			o_secondo.insert(*it);
		}
	}

	for (set<int>::iterator it = maximalxy.begin(); it != maximalxy.end(); it++) {
		bool chk = 	(o_primo.find(*it) != o_primo.end());
			if(chk){
				//è un vecchio motivo che occorre ad (i, j)
			}else{
				//è un nuovo motivo.
				newMotif.insert(*it);
			}
	}

	/*
	 * Note that the only case in which an old motif M not occurring at [i, j] can be
	 * obliterated is when M is a submotif of some other motifs in Class I or II
	 */
	for (set<int>::iterator o2 = o_secondo.begin(); o2 != o_secondo.end(); o2++) {
		for (set<int>::iterator o1 = o_primo.begin(); o1 != o_primo.end(); o1++) {

			//occorrenze di O'
			int* OPrimo = maximal.getOccurence(*o1);

			//occorrenze di O''
			int* OSecondo = maximal.getOccurence(*o2);
			//n_occurence di O''
			int n_occ = maximal.getNOccurence(*o2);

			bool a_condition = (OPrimo[0]-OSecondo[0] == i-OSecondo[n_occ -2])
							&& (OPrimo[1]-OSecondo[1] == j-OSecondo[n_occ -1]);

			bool b_condition = (OPrimo[2]-OPrimo[0] == OSecondo[2]-OSecondo[0])
							&& (OPrimo[3]-OPrimo[1] == OSecondo[3]-OSecondo[1]);

			if(a_condition && b_condition){

			}else{
				oldMotif.insert(*o2);
				break;
			}
		}
	}
}


///Optimized list procedure.
/*!
 * Controllo se righe e colonne a distanza "c" e "d" del core corrispondono effettivamente
 * alla prima riga e prima colonna del motivo massimale al passo corrente; se è così estendo
 * la lista di occorrenze del motivo massimale.
 * Si risparmia tempo di calcolo, ma serve una grande quantità di memoria (applicabile per array piccoli)
 * \n\b nota: segue fedelmente le indicazioni in "Motif Patterns in 2D"
 * è alternativo all'approccio listNaive
 */
template<typename T>
void listL(int****L, MyCore<T>& core, MyTrie<T>& maximal, set<int>& novel, T** autoc, int sup,
		int sx, int i, int j, int rows, int cols, T dontcare){

	//cacolo gli indici del core
	int c = -1;
	int d = -1;
	int n_cups;
	int* cups = get_cups(autoc, rows, cols, dontcare, n_cups);
	if(cups[n_cups-2] == -1){
		// se il core è vuoto ... ???
		int* o2 = new int[4];
		o2[0] = sup;
		o2[1] = sx;
		o2[2] = i;
		o2[3] = j;

		int index;
		int occ = maximal.insElements(autoc, rows, cols, o2, 4, index)[0][0];

		if(occ == 4){
			//l'occorrenza massimale è un motivo novello!!!
			//printBite(autoc,0,0,rows, cols);
			//genero una copia perchè devo avere un puntatore
			//diverso (appunto una copia)
			int* o2 = new int[4];
			o2[0] = sup;
			o2[1] = sx;
			o2[2] = i;
			o2[3] = j;

			novel.insert(index);
		}
		return;
	}else{
		c = cups[n_cups-2];
		d = cups[n_cups-1];
	}
	//quì arrivo solo se il core nn è vuoto!

	// verifico se è possibile estendere le occorrenze del
	// core per il motivo massimale
	int** ref_occ;

	bool can_extend = true;
	//n_occorrenze del core;
	int arg_size = (*ref_occ[0]) -2;
	int* ext_occ = new int[arg_size];
	for(int occ = 0 ; occ < arg_size; occ++){
		int f = *(ref_occ[1] + occ);
		ext_occ[occ++] = f-c;
		int g = *(ref_occ[1] + occ);
		ext_occ[occ] = g-d;
		if(j>=sx && L[sup][i][j-sx][f-c]<g-d){

		}
		else{
			can_extend = false;
			//cout << "CAN't EXTEND" << endl;
			//deve essere valido per tutte le coccorrenze del core
			break;
		}
	}
	if(can_extend){
		int* o2 = new int[4];

		o2[0] = sup;
		o2[1] = sx;
		o2[2] = i;
		o2[3] = j;

		int* merged_occ = MyTrie<T>::mergeOccurence(ext_occ, arg_size, o2, 4);
		int index;
		int occ = maximal.insElements(autoc, rows, cols, merged_occ, arg_size, index)[0][0];
		delete ext_occ;

		if(occ == 4){
			//l'occorrenza massimale è un motivo novello!!!
			//printBite(autoc,0,0,rows, cols);
			novel.insert(index);
		}
	}
}

///List naive procedure.
/*!
 * Per ciascun
  genera la lista di occorrenze presenti nella regione R(i,j)
 * identifica i motivi novelli. Alla fine aggiorna le liste di occorrenza per i motivi esistenti
 */
template<typename T>
void listNaive(int****L, set<int>& maximalxy, MyCore<T>& core, MyTrie<T>& maximal, set<int>& novel, T** autoc, int sup,
		int sx, int i, int j, int rows, int cols, T** img, int img_r, int img_c, T dontcare) {

	bool debug = false;

	//cacolo gli indici del core
	int c = -1;
	int d = -1;
	int n_cups;
	int* cups = get_cups(autoc, rows, cols, dontcare, n_cups);
	if(cups[n_cups-2] == -1 || maximal.getRoot() == NULL){
		// se il core è vuoto ...

		int index;
		int n_ret_occ = 0;

		n_ret_occ = *maximal.insElements(autoc, rows, cols, sup, sx, i, j, index)[0];
		if(index != -1){
			if(debug){
				cout << "-core vuoto, motif: " << endl;
				printBite(autoc, 0, 0, rows, cols);
			}
			core.insElement(sup, sx, i, j, c, d, index);
		}

		if(n_ret_occ == 4){
		// inserisco il motivo novello nell'insieme dei novelli
			novel.insert(index);
		}else{
			maximalxy.insert(index);
		}
	}else{

		c = cups[n_cups-2];
		d = cups[n_cups-1];
		if(debug){
			cout << "-core pieno, motif: " << endl;
			printBite(autoc, 0, 0, rows, cols);
		}
		int core_id = -1;

		if(debug){
			cout << "autoc: sup:" << sup << " sx:" << sx << " i:" << i << " j:" << j << endl;
			cout << "GET core: " << endl;
			printCore(autoc, c, d, rows, cols);
		}
		core_id = core.getCore(sup, sx, i, j, c, d);


		// il core è già stato inserito a qualche passo precedente.
		/**/
		int n_occurence = maximal.getNOccurence(core_id);
		int* occurence = maximal.getOccurence(core_id);
		int** ret_occ;

		int now_size = 4;
		int* now_occ = new int[now_size];
		now_occ[0] = sup;
		now_occ[1] = sx;
		now_occ[2] = i;
		now_occ[3] = j;

		ret_occ = tryToExtend(maximal, occurence, n_occurence, autoc, rows,	cols, sup, sx, i, j, c, d, img, img_r, img_c);

		int arg_size = *(ret_occ[0]);
		int* args = ret_occ[1];
		int* up_occ = MyTrie<T>::mergeOccurence(now_occ, now_size, args, arg_size);

		delete args;

		int index;
		/**/
		int novel_occ = *maximal.insElements(autoc, rows, cols, up_occ, now_size, index)[0];

		if(novel_occ == 4){
		// inserisco il motivo novello nell'insieme dei novelli
			novel.insert(index);
		}else{
			maximalxy.insert(index);
		}

		//inserisco il puntatore al core del motivo corrente
		if(index != -1){
			if(debug)
				cout << "EXT motif: " << endl;
			core.insElement(sup, sx, i, j, c, d, index);
		}
		/**/

		//chiamata al "Passo3"
		maximal.updateTrie(sup, sx, i, j, autoc, rows, cols, dontcare);
	}
}


///List procedure.
/*!
 * In base alle dimemnsione dell'immagine, si sceglie quale metodo utilizzare, listL oppure listNaive
 */
template<typename T>
void list(int****L, set<int>& maximalxy, MyCore<T>& core, MyTrie<T>& maximal, set<int>& novel, T** autoc, int sup,
		int sx, int i, int j, int rows, int cols, T** img, int img_r, int img_c, T dontcare) {

	if(L != NULL)
		listL(L, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, dontcare);
	else
		listNaive(L, maximalxy, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, img, img_r, img_c, dontcare);

}

///Compilation of all the motifs with a maximal occurrence at [i, j].
/*!
 * Genera tutti i meet tra ogni bite e tutti i bite della regione corrente.
 */
template<typename T>
void compile(set<int>& maximalxy, MyCore<T>& core, MyTrie<T>& maximal, set<int>& novel,
		T** img, int**** L, int sup, int sx, int height, int width,	T dontcare,
		int max_rows, int max_cols, int max_dontcare, int min_rows, int min_cols, bool optimize) {

	bool useL = true;
	if(L == NULL)
		useL = false;

	int rows;
	int cols;
	int i = 0;
	int j = 0;
	int x = 0;
	int n_dontcare = 0;

	for (i = sup; i < height; i++) {
		for (j = 0; j < width; j++) {
			//TODO: verificare!!!
			//così forse la regione cresce con la granularità delle righe
			//if (i == sup && j < sx)
			//	j = sx;

			if(sup== i && sx==j){

			}
			//sono interessato ai 2-motif non devo prendere il meet del bite(i,j) con se stesso
			else{
				//COMPILE: autoc è un motivo massimale con occorrenza a (i,j)
				T** autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
						cols, n_dontcare, optimize, max_rows, max_cols, max_dontcare,
						min_rows, min_cols);

				if (rows > 0 && cols > 0) {
					//if(rows == 2 && cols == 2){
					//	cout << autoc[0][0] << autoc[0][1] << "+++++++ sup:" << sup << " sx:" << sx << " i:" << i << " j:" << j << endl;
					//	cout << autoc[1][0] << autoc[1][1] << endl;
					//}
					//LIST
					list(L, maximalxy, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, img, height, width, dontcare);
					for (x = 0; x < rows; x++)
						delete autoc[x];
					delete autoc;
				}

				// seconda tipologia di autocorrelazione (j = -j)
				if(j != 0){
					//COMPILE: autoc è un motivo massimale con occorrenza a (i,-j)
					//sup, sx +j , i, sx
					//sup, sx, i, -j
					autoc = meet(img, sup, sx + j , i, sx, height, width, dontcare, rows,
							cols, n_dontcare, optimize, max_rows, max_cols, max_dontcare,
							min_rows, min_cols);

					if (rows > 0 && cols > 0) {
						//if(rows == 2 && cols == 2){
						//	cout << autoc[0][0] << autoc[0][1] << "------- sup:" << sup << " sx:" << sx + j << " i:" << i << " j:" << sx << endl;
						//	cout << autoc[1][0] << autoc[1][1] << endl;
						//}
						//LIST
						list(L, maximalxy, core, maximal, novel, autoc, sup, sx +j , i, sx, rows, cols, img, height, width, dontcare);
						for (x = 0; x < rows; x++)
							delete autoc[x];
						delete autoc;
					}
				}
			}
		}
		//cout << "   DEBUG: " << i << " " << j << endl;
	}
	//return trie;
}

///Returns array of integer that are in sequence rows index and column index for cusps of a specific motif.
/*!
 * ritorna come output un array di interi che rappresentano (in successione)
 * gli indici di riga e colonna delle cuspidi.
 * Nelle 2 ultime posizioni del vettore sono contenuti gli indici della cuspide
 * che identifica in core dell'meet passato come input al metodo stesso.
 * \n\b nota. se il core nn esiste gli indici valgono entrambi -1.
 */
template<typename T>
int* get_cups(T** autoc, int height, int width, T dontcare, int& n_cups) {

	//casi particolari in una dimensione
	if(height == 1){
		//ho solo una riga
		n_cups = 0;
		//al più posso avere width-1 cuspidi
		//ogni cuspide ha 2 indici che le individuano
		int* cups = new int[(width-1)*2];
		int cd = 0;
		for(int j = width-1; j > 0; j--){
			if(autoc[0][j] != dontcare){
				n_cups = n_cups +2;
				cups[cd++] = 0;
				cups[cd++] = j;
			}
		}
		if(n_cups == 0){
			//il core nn esiste
			int* new_cups = new int[2];
			new_cups[0] = -1;
			new_cups[1] = -1;
			n_cups = 2;
			delete cups;
			return new_cups;
		}else{
			if(n_cups == (width-1)*2){
				//erano tutti caratteri solidi ho width-1 cuspidi
				return cups;
			}else{
				int* new_cups = new int[n_cups];
				int* to_del = cups;
				for(int cd = 0; cd < n_cups; cd++){
					new_cups[cd] = cups[cd];
				}
				cups = new_cups;
				//TODO problema memoria??? xchè??? sotto funziona
				delete to_del;
				return cups;
			}
		}

	}
	if(width == 1){
		//ho solo una colonna
		n_cups = 0;
		//al più posso avere height-1 cuspidi
		//ogni cuspide ha 2 indici che le individuano
		int* cups = new int[(height-1)*2];
		int cd = 0;
		for(int i = height-1; i > 0; i--){
			if(autoc[i][0] != dontcare){
				n_cups = n_cups +2;
				cups[cd++] = i;
				cups[cd++] = 0;
			}
		}
		if(n_cups == 0){
			//il core nn esiste
			int* new_cups = new int[2];
			new_cups[0] = -1;
			new_cups[1] = -1;
			n_cups = 2;
			delete cups;
			return new_cups;
		}else{
			if(n_cups == (height-1)*2){
				//erano tutti caratteri solidi ho width-1 cuspidi
				return cups;
			}else{
				int* new_cups = new int[n_cups];
				int* to_del = cups;
				for(int cd = 0; cd < n_cups; cd++){
					new_cups[cd] = cups[cd];
				}
				cups = new_cups;
				//TODO problema memoria??? xchè??? sotto funziona
				delete to_del;
				return cups;
			}
		}
	}

	n_cups = 0;
	int last_i = 0;
	int array_size = 2*width;
	int* cups = new int[array_size];
	bool* binary_vector = new bool[width];
	//inizializzo il vettore binary_vector con true in posizione j
	//se l'ultima riga di autoc ha un carattere solido alla colonna j
	for (int j = width - 1; j > 0; j--) {
		if (autoc[height - 1][j] != dontcare) {
			binary_vector[j] = true;
			cups[last_i++] = height - 1;
			cups[last_i++] = j;
			n_cups = last_i;
			//cout << "cups: " << height - 1 << ", " << j << endl;
		} else {
			binary_vector[j] = false;
		}
	}
	if (height > 1) {
		for (int i = height - 2; i > 0; i--) {
			bool found_on_row = false;
			for (int j = width - 1; j > 0; j--) {
				if(last_i == array_size){
					//ridimensiona
					int old_size = array_size;
					array_size = array_size + (2*width);
					int* to_del = cups;
					int* new_cups = new int[array_size];
					for(int s = 0; s< old_size; s++)
						new_cups[s] = cups[s];
					cups = new_cups;
					delete to_del;
				}

				if (found_on_row && binary_vector[j]) {
					cups[last_i++] = i;
					cups[last_i++] = j;
					n_cups = last_i;
					//cout << "cups: " << i << ", " << j << endl;
				} else if (autoc[i][j] != dontcare) {
					found_on_row = true;
					binary_vector[j] = true;
					cups[last_i++] = i;
					cups[last_i++] = j;
					n_cups = last_i;
					//cout << "cups: " << i << ", " << j << endl;
				}
			}
		}
	}
	int* to_del = cups;
	int* new_cups;
	if(n_cups == 0 || height == 1){
		//il core non esiste
		new_cups = new int[2];
		new_cups[0] = -1;
		new_cups[1] = -1;
		n_cups = 2;
	}else{
		//trimmo l'array a n_cups elementi
		new_cups = new int[n_cups];
		for(int k = 0; k<n_cups; k++){
			new_cups[k] = cups[k];
		}
	}
	cups = new_cups;
	delete to_del;
	delete binary_vector;
	return cups;
}

///Compute the meet between bite at (sup, sx) and bite at (i, j) of an image img.
/*!
 * \param img input image.
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param i first coordinate of second bite.
 * \param j second coordinate of second bite.
 * \param rows image rows number.
 * \param cols image column number.
 * \param dontcare don't care symbol.
 * \param rows_meet output rows number.
 * \param cols_meet output columns number.
 * \param n_dontcare output don't care number.
 * \param compile_opt flag for optimization.
 * \param opt_row_max_size maximal rows limit for motif that could be extracted.
 * \param opt_col_max_size maximal columns limit for motif that could be extracted.
 * \param opt_max_n_dontcare maximal dontcare limit for motif that could be extracted.
 * \param opt_row_min_size minimal rows limit for motif that could be extracted.
 * \param opt_col_min_size minimal columns limit for motif that could be extracted.
 */
template<typename T>
T** meet(T** img, int sup, int sx, int i, int j, int rows, int cols,
		T dontcare, int& rows_meet, int& cols_meet, int& n_dontcare,
		bool compile_opt = false, int opt_row_max_size = -1,
		int opt_col_max_size = -1, int opt_max_n_dontcare = -1,
		int opt_row_min_size = -1, int opt_col_min_size = -1) {
	return consensus(img, sup, sx, i, j, rows, cols, dontcare, rows_meet,
			cols_meet, n_dontcare, compile_opt, true,
			opt_row_max_size, opt_col_max_size, opt_max_n_dontcare,
			opt_row_min_size, opt_col_min_size);
}

///Compute the consensus between bite at (sup, sx) and bite at (i, j) of an image img.
/*!
 * \param img input image.
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param i first coordinate of second bite.
 * \param j second coordinate of second bite.
 * \param rows image rows number.
 * \param cols image column number.
 * \param dontcare don't care symbol.
 * \param rows_meet output rows number.
 * \param cols_meet output columns number.
 * \param n_dontcare output don't care number.
 * \param compile_opt flag for optimization.
 * \param is_meet flag used for computing meet instead of consensus.
 * \param opt_row_max_size maximal rows limit for motif that could be extracted.
 * \param opt_col_max_size maximal columns limit for motif that could be extracted.
 * \param opt_max_n_dontcare maximal dontcare limit for motif that could be extracted.
 * \param opt_row_min_size minimal rows limit for motif that could be extracted.
 * \param opt_col_min_size minimal columns limit for motif that could be extracted.
 */
template<typename T>
T** consensus(T** img, int sup, int sx, int i, int j, int rows, int cols,
		T dontcare, int& rows_meet, int& cols_meet, int& n_dontcare,
		bool compile_opt = false, bool is_meet = false,
		int opt_row_max_size = -1, int opt_col_max_size = -1, int opt_max_n_dontcare = -1,
		int opt_row_min_size = -1, int opt_col_min_size = -1) {

	if (i < sup) {
		// nn si deve verificare perche' significa che il bite nn appartiene all'immagine
		rows_meet = 0;
		cols_meet = 0;
		return NULL;
	}

	int off_W = j - sx;
	int h_size = rows - i;
	int w_size = cols - j;

	if (off_W < 0)
		w_size = cols - sx;

	rows = h_size;
	cols = w_size;

	//Estrazione parametrica su size e numero di don't care/////////////////////////////
	//dei motivi rispetto a size e rispetto al numero di caratteri solidi
	//come default uso le dimensioni del consenso per nn limitare la dimensione del motivo
	int opt_max_rows_meet = opt_row_max_size;
	int opt_max_cols_meet = opt_col_max_size;
	int opt_max_dontcare = opt_max_n_dontcare;

	int opt_min_rows_meet = opt_row_min_size;
	int opt_min_cols_meet = opt_col_min_size;
	if(opt_min_rows_meet == -1)
		opt_min_rows_meet = 1; //valore di default. nessuna dimensione minima
	if(opt_min_cols_meet == -1)
		opt_min_cols_meet = 1; //valore di default. nessuna dimensione minima
	if(opt_row_max_size == -1)
		opt_max_rows_meet = (rows); // valore di default se non si vuole nessuna ottimizzazione
	if(opt_col_max_size == -1)
		opt_max_cols_meet = (cols); // valore di default se non si vuole nessuna ottimizzazione
	if(opt_max_n_dontcare == -1)
		opt_max_dontcare = (rows*cols);// rows * cols; // valore di default se non si vuole nessuna ottimizzazione
	//float
	///////////////////////////////////////////////////////////////////////////////////////////////

	if(cols < opt_min_cols_meet || rows < opt_min_rows_meet
			|| cols > opt_max_cols_meet || rows > opt_max_rows_meet){
		rows_meet = 0;
		cols_meet = 0;
		return NULL;
	}

	int min_row = rows;
	int min_col = cols;
	int max_row = 0;
	int max_col = 0;
	int x = 0;
	int y = 0;
	int i_del = 0;
	n_dontcare = 0;
	int first_col_n_dontcare = 0;
	int first_row_n_dontcare = 0;

	// verifico che prima riga o prima colonna non siano fatte tutte di dont'care
	if (is_meet && compile_opt) {
		for (x = 0; x < rows; x++) {
			// y = 0
			if (img[x + i][y + j ] == img[x + sup][y + sx ]) {
				break;
			} else {
				first_col_n_dontcare++;
			}
		}
		if (first_col_n_dontcare == rows && cols > 1) {
			// prima colonna tutti don't care alla procedura compile questo nn serve
			rows_meet = 0;
			cols_meet = 0;
			//return NULL;
		}

		//azzero x
		x = 0;
		for (y = 0; y < cols; y++) {
			// x = 0
			if (img[x + i][y + j ] == img[x + sup][y + sx ]) {
				break;
			}else {
				first_row_n_dontcare++;
			}
		}
		if (first_row_n_dontcare == cols && rows > 1) {
			// prima riga tutti don't care alla procedura compile questo nn serve
			rows_meet = 0;
			cols_meet = 0;
			//return NULL;
		}
	}

	T** immagine = new T*[rows];
	T * *meet;

	//devo allocare prima il vettore per intero
	//perchè altrimenti o problemi a deallocare.
	for(int g = 0; g < rows; g++)
		immagine[g] = new T[cols];

	for (x = rows-1; x >= 0; x--) {
		for (y = cols-1; y >= 0; y--) {
			if (img[x + i][y + j ] == img[x + sup][y + sx ]) {

				immagine[x][y] = img[x + sup][y + sx ];
				if (x < min_row)
					min_row = x;
				if (y < min_col)
					min_col = y;
				if (x > max_row)
					max_row = x;
				if (y > max_col)
					max_col = y;
			} else {
				immagine[x][y] = dontcare;

				n_dontcare++;

				//in questo caso il meet non soddisfa i vincoli sulla numero massimo di don't care.
				if(is_meet  && compile_opt && (n_dontcare > opt_max_dontcare) ){
					//in questo caso il meet non soddisfa i vincoli sulla dimensione massima del motivo.
					rows_meet = 0;
					cols_meet = 0;
					for(int d = 0; d < rows; d++){
						if(immagine[d] != NULL)
							delete immagine[d];
					}
					delete immagine;
					return NULL;
				}
			}
		}//for y
	}//for x

	if (is_meet) {
		rows_meet = max_row - min_row + 1;
		cols_meet = max_col - min_col + 1;

		if(rows_meet <= 0 || cols_meet <= 0){
			rows_meet = 0;
			cols_meet = 0;
			for(int d = 0; d < rows; d++){
				if(immagine[d] != NULL)
					delete immagine[d];
			}
			delete immagine;
			return NULL;
		}

		meet = new T*[rows_meet];
		for (x = 0; x < rows_meet; x++) {
			meet[x] = new T[cols_meet];
			for (y = 0; y < cols_meet; y++) {
				meet[x][y] = immagine[x + min_row][y + min_col];
			}
		}
	} else {
		rows_meet = rows;
		cols_meet = cols;
	}
	if (is_meet) {
		for (i_del = 0; i_del < rows; i_del++)
			delete immagine[i_del];
		delete immagine;
		return meet;
	} else
		return immagine;
}

/// Clear all file into the base directory.
/*!
 * \param dirname base directory.
 */
void clear_dir(char* dirname){

	DIR *pdir;
	struct dirent *pent;
	pdir=opendir(dirname);
	 if (!pdir){
		 //cout<<"Directory doesnot exist"<<endl;
		 return;
	 }

	 while ((pent=readdir(pdir)))
	 {
	  //cout<<pent->d_name;
	  string file_delete=pent->d_name;
	  ostringstream oss;
	  oss << dirname<<  "/" << file_delete;
	  string filename = oss.str();
	  remove(filename.c_str());
	 }

	 closedir(pdir);
}

///Write on disk all elements of a specific base.
/*!
 * \param dirname output directory.
 * \param base specific base set.
 * \param maximal maximal motifs set.
 */
void write_base(char* dirname, set<int> base, MyTrie<int>& maximal){
	//if (!CreateDirectory("base", NULL)) {
	if(mkdir(dirname, 0777) == -1){
		//cerr<<"Error :  Rimuovere la directory \"base\" delle basi manualmente "<<endl;
		//exit(1);
	}
	for(set<int>::iterator it = base.begin(); it != base.end(); it++){

		int** motif = maximal.getMotif(*it);
		int motif_r = maximal.getRows(*it);
		int motif_c = maximal.getCols(*it);
		ostringstream oss;
		oss << dirname<< "/base_" << *it << ".bmp";
		string filename = oss.str();
		BMPImage consintimage = BMPImage(filename.c_str());
		consintimage.set_int(motif, motif_r, motif_c);
		consintimage.writeBmpFile();
	}
}

///Write on disk all autocorrelation images generated from a specific bite.
/*!
 * Scrive su disco tutte le immagini ottenute dal calcolo delle autocorrelazioni del bite
 * in posizione (sup, sx) dell'immaine img di dimensione height*widh.
 * \n\b nota. è possibile limitare il numero delle autocorrelazioni e quindi delle immagini generate.
 */
void write_autocorrelation(char* filename, int** img, int sup, int sx, int height, int width,
		int limit = -1) {

	//if (!CreateDirectory("ac", NULL)) {
	if(mkdir(filename, 0777) == -1){
		//cerr<<"Error :  Rimuovere la directory \"ac\" delle autocorrelazioni manualmente "<<endl;
		//exit(1);
	}

	int i = 0;
	int j = 0;
	int x = 0;
	int n_dontcare = 0;
	int rows;
	int cols;
	int dontcare = 0xFFFFFF;
	int heigh_limit = height;
	int width_limit = width;
	if (limit > 0) {
		heigh_limit = sup + limit;
		width_limit = sx + limit;
	}

	for (i = sup; i < heigh_limit; i++) {
		for (j = 0; j < width_limit; j++) {
			if (i == sup && j < sx)
				j = sx;
			int** autoc = meet(img, sup, sx, i, j, height, width, dontcare,
					rows, cols, n_dontcare, true);
			if (rows > 0 && cols > 0) {
				ostringstream oss;
				oss << filename << "/meet_" << sup << "_" << sx << "_" << i << "_" << j << ".bmp";
				string filename = oss.str();
				BMPImage consintimage = BMPImage(filename.c_str());
				consintimage.set_int(autoc, rows, cols);
				consintimage.writeBmpFile();

				for (x = 0; x < rows; x++)
					delete autoc[x];
				delete autoc;
			}
			// seconda tipologia di autocorrelazione (j = -j)
			if(j != 0){
				autoc = meet(img, sup, sx, i, -j, height, width, dontcare,
						rows, cols, n_dontcare, true);
				if (rows > 0 && cols > 0) {
					ostringstream oss;
					oss << filename << "/meet'_" << sup << "_" << sx + j<< "_" << i << "_" << sx << ".bmp";
					string filename = oss.str();
					BMPImage consintimage = BMPImage(filename.c_str());
					consintimage.set_int(autoc, rows, cols);
					consintimage.writeBmpFile();

					for (x = 0; x < rows; x++)
						delete autoc[x];
					delete autoc;
				}
			}

		}
		cout << i << " " << j << endl;
	}
}

///Called at the end of list procedure for updating existing motif list of occurrence.
/*!
 *È richiamato alla fine della procedura list per aggiornare le liste di occorrenza dei motivi precalcolati.\n
 *ret_occ[0] ==> numero di occorrenze.\n
 *ret_occ[1] ==> puntatore alle occorrenze.\n
 *int* occurence ==> occorrenze del core di autoc a distanza (c,d).
 */
template<typename T>
int** tryToExtend(MyTrie<T>& maximal, int* occurence, int n_occurence, T** autoc, int rows, int cols, int sup, int sx,
		int i, int j, int c, int d, T** img, int img_r, int img_c){
	int** ret_occ = new int*[2];
	int ret_n_occ = 0;
	//potenzialmente autoc potrebbe estendere tutte le occorrenze del suo core.
	int* ret_occurence = new int[n_occurence];

	for(int _i = 0; _i < n_occurence;){
		bool can_extend = true;
		// prendo la prima coppia di indici che mi identifica la prima occorrenza
		int _x = occurence[_i++];
		int _y = occurence[_i++];

		// verifica sulla riga
		for(int k = 0; k < cols; k++){
			if((_x - c < img_r) && (_y + k< img_c) && (_x - c > 0) && (_y + k > 0)&& img[_x - c][_y + k] == autoc[0][k]){
				// è possibile che autoc estenda l'occorrenza (_x,_y) del suo core...
			}else{
				// NON è possibile che autoc estenda l'occorrenza (_x,_y) del suo core.
				can_extend = false;
				break;
			}
		}
		if(can_extend){
			// se  e solo se è andata a buon fine la verifica sulla riga
			// procedo a verificare la colonna
			for(int h = 0; h < rows; h++){
				if((_x + h < img_r) && (_y - d < img_c) && (_x + h > 0) && (_y - d > 0) && img[_x + h][_y - d] == autoc[h][0]){
					// è possibile che autoc estenda l'occorrenza (_x,_y) del suo core...
				}else{
					// NON è possibile che autoc estenda l'occorrenza (_x,_y) del suo core.
					can_extend = false;
					break;
				}
			}
		}
		if(can_extend){
			// a questo punto se can_extend vale TRUE significa che
			// autoc estende l'occorrenza (_x,_y) del suo core!!

			ret_occurence[ret_n_occ++] = _x;
			ret_occurence[ret_n_occ++] = _y;
		}
	}

	//devo ridimensionare il vettore delle occorrenze di autoc nel caso
	//nn sia stato possibile estendere tutto le occorrenze del suo core.
	if(ret_n_occ != n_occurence){
		int* trim_occ = new int[ret_n_occ];
		int* to_del = ret_occurence;
		for(int t = 0; t< ret_n_occ; t++){
			trim_occ[t] = ret_occurence[t];
		}
		ret_occurence = trim_occ;
		delete to_del;
	}

	ret_occ[0] = &ret_n_occ;
	ret_occ[1] = ret_occurence;
	return ret_occ;
}


///Generate all bases of an input image and save on disk images for each element of each base.
/*!
 *Genera su tutte le basi dell'immagine img di dimensioni height x width utilizzando l'approccio incrementale.
 *scrive su disco l'elenco dei motivi che appartengono ciascuna della basi.
 *\n\b nota:è possibile abilitare delle stampe di debug attraverso il flag debug.
 *Il simbolo di don't care è specificato tramite l'attributo generico dontcare.
 */
template<typename T>
void process_and_save(char* dirname, T** img, int height, int width, T dontcare,
		int max_rows, int max_cols, int max_dontcare, int min_rows, int min_cols, bool optimize, bool debug) {


	if(debug)
		cout << endl << "start process" << endl << endl;

	unsigned int sizeOfL = (height*height*height*width)* sizeof(int);

	//TODO bool useL = true  //(per ora forzo il comportamento Naive -> useL = false)
	bool useL = false;
	int**** L = NULL;
	if(sizeOfL > 1000000000){
		cout << "max size of L: 1GB" << endl;
		useL = false;
	}
	if(debug && useL)
		cout << "creating " << sizeOfL << " bytes for L..." << endl;

	if(useL){
		L = get_L(height, width, NULL);
		if(debug)
			cout << "created L of: "<< sizeOfL << " bytes!" << endl;
	}

	MyCore<T> core = MyCore<T>();
	MyTrie<T> maximal = MyTrie<T>();
	set<int> maximalxy;
	set<int> novel;
	set<int> base;

	for(int x = height - 1; x >= 0; x--){
		cout << "x." << x << " " << endl;
		for(int y = width - 1; y >= 0; y--){

			//maximal.printPaths();

			//conterrà solo i motivi novelli.
			novel = set<int>();

			//conterrà i motivi massimali a (x, y)
			maximalxy = set<int>();

			if(useL)
				aggiorna_L(img, L, x, y, height, width, dontcare);

			compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
					max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

			if(debug && false){
				cout << "Novelli{ " <<endl;
				for(set<int>::iterator it = novel.begin(); it != novel.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			//in newMotif ci saranno i motivi nuovi ma non i novelli;
			set<int> newMotif = set<int>();

			//in oldMotif ci saranno i motivi massimali che erano già presenti nella base.
			set<int> oldMotif = set<int>();

			separate(maximal, base, maximalxy, newMotif, oldMotif, dontcare, x, y);

			extract(maximal, base, novel, newMotif, oldMotif, dontcare);

			if(debug && false){
				cout << "Base{ " <<endl;
				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}
			//da implementare se voglio salvare ad ogni passo le base corrente
			//save(base);

			ostringstream oss;
			oss << dirname << "_" << x;
			string filename = oss.str();

			char* directory = (char*)filename.c_str();

			if(base.size() >0)
				clear_dir(directory);

			write_base(directory,base, maximal);

			/**/
			if(debug){
				cout << "Base " << x << ", " << y << " { " <<endl;
				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					cout << *it << ": " ;
					maximal.navigate(*it);
				}
				cout << endl << "}" << endl;
			}
			/**/
		}
	}
	if(debug){
		//cout << endl << endl << "TRIE STATUS" << endl << endl;

		cout << "Maximal elems: {" << endl;
		maximal.printPaths();
		cout << "}" << endl;

		//core.print();
	}
	cout << "Last Base{ " <<endl;
	for(set<int>::iterator it = base.begin(); it != base.end(); it++){
		cout << *it << endl;
	}
	cout << endl << "}" << endl;

	if(useL)
		free_L(L, height, width);

	if(debug)
		cout << endl << "end process" << endl;

}


//returns true if m2 is submotif of m1
template<typename T>
bool containMotif(T** m1, T** m2, int rows1, int cols1, int rows2, int cols2, int** c, int** d, int& size_c, int& size_d, T dontcare){

int* temp1;
int* temp2;
bool flag=true;

if(rows2 <= rows1 && cols2 <= cols1){

			for(int i=0;i<rows1;i++){
				for(int j=0;j<cols1;j++){
					flag=true;
					if(((i+rows2)<=rows1)&&((j+cols2)<=cols1)){
						for(int k=0;k<rows2 && flag;k++)
							for(int h=0;h<cols2 && flag;h++)
								if((m1[i+k][j+h]!= m2[k][h]) && (m2[k][h]!= dontcare))
									flag=false;
						   if(flag){
							size_c++; size_d++;
							if(size_c==1 && size_d==1){
								temp1=new int[size_c];
								temp2=new int[size_d];
								temp1[0]=i;
								temp2[0]=j;
							}//if
							else{

								int* t1=new int[size_c];
								int* t2=new int[size_d];
								for(int s=0;s<(size_c-1);s++)
									t1[s]=temp1[s];
								t1[size_c-1]=i;
								for(int t=0;t<(size_d-1);t++)
									t2[t]=temp2[t];
								t2[size_d-1]=j;
								delete []temp1;
								delete []temp2;
								temp1=t1;
								temp2=t2;
							}//else
						}//if flag
					}//if mv
				}//for
			}//for

		}//if
	*c=temp1;
	*d=temp2;

	if(size_c>0 && size_d>0)
		return true;
	else
	return false;

}


#endif /* MOTIFPATTERNCORE_H_ */
