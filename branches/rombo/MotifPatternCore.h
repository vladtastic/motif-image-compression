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

///Prints on standard output an autocorrelation.
/*!
 * Prints on standard output autocorrelation between bite at (sup, sx) and image img with size height*width.
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
void print_autocorrelation(T** img, int sup, int sx, int height, int width,
		T dontcare, bool simulate = false) {
	int rows;
	int cols;
	int i = 0;
	int j = 0;
	int x = 0;
	int n_dontcare = 0;


	for (i = sup; i < height; i++) {
			for (j = sx; j < width; j++) {

				if(sup== i && sx==j){

				}
				else{
					T** autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
								cols, n_dontcare, true);

					if (rows > 0 && cols > 0) {
						if (simulate) {

						} else {
							cout << endl << "meet [(" << sup << ", " << sx << "); ("
								<< i << ", " << j << ")]" << endl;
							print_image<T> (autoc, rows, cols);
						}

						for (x = 0; x < rows; x++)
							delete autoc[x];
						delete autoc;
					}
				}

				if (simulate)
					cout << "    DEBUG: " << i << " " << j << endl;

			}
		}

			for (i = sup+1; i < height; i++) {
				for (j = 0; j < sx; j++) {

					if(sup== i && sx==j){

						}

					else{

						T** autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
								cols, n_dontcare, true);
						if (rows > 0 && cols > 0) {
								if (simulate) {

								} else {
									cout << endl << "meet [(" << sup << ", " << sx << "); ("
									<< i << ", " << j << ")]" << endl;
									print_image<T> (autoc, rows, cols);
								}

								for (x = 0; x < rows; x++)
									delete autoc[x];
								delete autoc;
						}
					}

					if (simulate)
						cout << "    DEBUG: " << i << " " << j << endl;
				}
			}
	}


///Prints on standard output all autocorrelations.
/*!
 * Prints on standard output all autocorrelations of image img with size height*width.
 * \n\b Note: it's possible to simulate printing on standard output for testing algorithm performance
 *\param height image height.
 *\param width image width.
 *\param dontcare don't care autocorrelation symbol.
 *\param simulate testing algorithm performance flag.
 *
 */
template<typename T>
void print_autocorrelations(T** img, int height, int width,
		T dontcare, bool simulate = false) {

	for(int x = height - 1; x >= 0; x--){
		for(int y = width - 1; y >= 0; y--){
			cout << "x: " << x << " y: " << y << endl;
			print_autocorrelation(img, 0, 0, height, width, dontcare, simulate);
		}
	}
}


///Releases memory allocated from get_L.
/*!
 * Used to release memory allocated from get_L. \n\b Note: unused with listNaive approach.
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


///Allocates a size-dimesional vector.
/*!
 * Inizializes a size-dimensional vector l<i, i', h, q>. \n\b Note: unused with listNaive approach.
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


///Prints on standard output the bite (i,j) of image img with size height x width.
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

///Prints on standard output the core (i,j) of image img with size height x width.
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

///Prints on standard output the first row of bite (i,j) of image img with size height x width.
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

///Prints on standard output the row j of image img with size width.
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

///Updates the size-dimesional vector
/*!
 *
 * \b Note: unused with listNaive approach.
 */
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

///Used from aggiorna_L to update the single row.
/*!
 *
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

///Gets a specified base of motifs.

/*!Generates the specified basis of image img with size height x width using incremental approach and
 * returns it as a CompactTrie. \n
 * DEBUG prints can be enabled by debug flag and don't care symbol can be specified by dontcare generics attribute.
 * \param img input image.
 * \param height image height.
 * \param width image width.
 * \param x_i row index of basis set to extract.
 * \param y_j colum index of basis set to extract.
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

    if (debug)
        cout << endl << "start process" << endl << endl;

    unsigned int sizeOfL = (height * height * height * width) * sizeof (int);

    //TODO bool useL = true  always Naive approach
    bool useL = false;
    int**** L = NULL;
    if (sizeOfL > 1000000000) {
        cout << "max size of L: 1GB" << endl;
        useL = false;
    }
    if (debug && useL)
        cout << "creating " << sizeOfL << " bytes for L..." << endl;

    if (useL) {
        L = get_L(height, width, NULL);
        if (debug)
            cout << "created L of: " << sizeOfL << " bytes!" << endl;
    }

    MyCore<T> core = MyCore<T > ();
    MyTrie<T> maximal = MyTrie<T > ();
    set<int> maximalxy;
    set<int> novel;
    set<int> base;

    for (int x = height - 1; x >= x_i; x--) {
        if (debug)
            cout << "x." << x << " " << endl;
        for (int y = width - 1; y >= y_j; y--) {

            //contains only novel motifs.
            novel = set<int>();

            //contains maximal motifs at (x,y)
            maximalxy = set<int>();

            if (useL)
                aggiorna_L(img, L, x, y, height, width, dontcare);

            compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
                    max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

            //contains new motif but not novels.
            set<int> newMotif = set<int>();

            //contains maximal motif already present in base.
            set<int> oldMotif = set<int>();

            set<int> oldMotifgood = set<int>();

            separate(maximal, core, base, maximalxy, novel, newMotif, oldMotif, oldMotifgood, dontcare, x, y, img, height, width);

			extract(maximal, base, novel, newMotif, oldMotif, oldMotifgood, dontcare);

            /**/
        if(x==0 && y==0){

				set<int> sub = set<int>();
				set<int> sup = set<int>();

				MyTrie<T> maximalTemp = MyTrie<T>();

				for(set<int>::iterator it = base.begin(); it != base.end(); it++){
				  for(set<int>::iterator it1 = base.begin(); it1 != base.end(); it1++){
					if((*it1)!= (*it)){
						T** mi=maximal.getMotif(*it);
						int rows1=maximal.getRows(*it);
						int cols1=maximal.getCols(*it);

						T** mj=maximal.getMotif(*it1);
						int rows2=maximal.getRows(*it1);
						int cols2=maximal.getCols(*it1);

						if((rows1<=rows2) && (cols1<=cols2)){
							int* c;
							int* d;
							int size_c=0;
							int size_d=0;
							if(containMotif(mj, mi, rows2, cols2, rows1, cols1, &c, &d, size_c, size_d, dontcare)){
								sub.insert(*it);
								//mi � sottomotivo di mj
								//per ogni coppia in c e d scostamento di riga e colonna devo:
								 //1. andare nel motivo pi� grande e isolare la sottoporzione
								 //2. prendere la lista di occorrenze del motivo grande e aggiungere per ogni occorrenza lo scostamento di riga e colonna
								 //3. creare un pezzo autonomo da inserire nella maximalTemp con un indice mio

								for(int index_c=0; index_c<size_c; index_c++){
									int index_trie;
									int occ_row=c[index_c];
									int occ_col=d[index_c];

									//1. isoliamo la sottoporzione dentro mj
									//height, int width
									int dim1=rows2-occ_row;
									int dim2=cols2-occ_col;

									T** subimage = new T*[dim1];
									for (int p = 0; p < dim1; p++)
											subimage[p] = new T[dim2];

									for(int p=0; p<dim1;p++)
										for(int q=0;q<dim2;q++)
											subimage[p][q]=mj[p+occ_row][q+occ_col];

									//2. aggiorno lista occorrenze motivo grande
									int* occ_mj=maximal.getOccurence(*it1);
									int n_occ_mj=maximal.getNOccurence(*it1);

									int* new_occ_mj=new int[n_occ_mj];

									for(int p=0;p<n_occ_mj-1;p+=2){
										new_occ_mj[p]=occ_mj[p]+occ_row;
										new_occ_mj[p+1]=occ_mj[p+1]+occ_col;}
									//3. inserisco sottoporzione + sua lista occorrenze nella nuova trie
									maximalTemp.insElements(subimage, dim1, dim2, new_occ_mj, n_occ_mj, index_trie);

									sup.insert(index_trie);
									}//for index_c
							}//if containMotif

						}//if dim
				   }//if mi!=mj
			  }//for
			 }//for

			//a questo punto abbiamo sup e sub, la nostra trie maximal e la nuova trie maximal_temp che contiene sup
			//creiamo una compactTrie che riempiamo con sup, poi buttiamo dentro sub

			CompactTrie<T> compT = CompactTrie<T>(&maximalTemp, sup, dontcare);

			set<int> new_base = set<int>();

			set<int>::iterator iter = sub.begin();
				for(; iter != sub.end(); iter++){
					T* motif = maximal.getLinearMotif(*iter);
					int size = maximal.getLinearSize(*iter);
					int* occ = maximal.getOccurence(*iter);
					int n_occ = maximal.getNOccurence(*iter);
					int id = compT.tryToInsValue(motif, size, occ, n_occ, dontcare);
					if(id != -1)
						new_base.insert(*iter);
				}

			//calcolo la differenza tra sub e base e unisco a questa new_base
			set<int> difference = set<int>();
				for (set<int>::iterator it = base.begin(); it != base.end(); it++) {
					     bool chk =  (sub.find(*it) != sub.end());
					     if(!chk){
							 difference.insert(*it);
					     }
			    }
			  for (set<int>::iterator it = new_base.begin(); it != new_base.end(); it++)
			  			difference.insert(*it);

				base=difference;
			}//if x=0 && y=0

        }
    }

    if (useL)
        free_L(L, height, width);

    if (debug)
        cout << endl << "end process" << endl;

    return CompactTrie<T> (&maximal, base, dontcare);

}

///Main method for 2D irredundant motif extraction.
/*!Generates all basis of image img with size height x width using incremental approach and
 * prints on standard output the list of motifs for each basis. The maximal motifs are stored into maximal trie,\n
 * the indexes of the motifs of the final basis are stored into basis set and both returned.
 * DEBUG prints can be enabled from the debug flag and don't care symbol can be specified from dontcare generics attribute.
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
 * \param basis set of indexes of the motifs into the final basis (it has to be empty and the method has to fill in it).
 * \param maximal the trie that contains the set of maximal motifs (it has to be empty and the method has to fill in it).
 */

template<typename T>
void process(T** img, int height, int width, T dontcare,
		int max_rows, int max_cols, int max_dontcare,
		int min_rows, int min_cols, bool optimize, bool debug, set<int>* basis, MyTrie<T>& maximal) {

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
		L = get_L(height, width, 0);//L = get_L(height, width, NULL);
		if(debug)
			cout << "created L of: "<< sizeOfL << " bytes!" << endl;
	}

	MyCore<T> core = MyCore<T>();
	set<int> maximalxy;
	set<int> novel;
	set<int> base;

	//contains maximal motifs at (x,y)

	for(int x = height - 1; x >= 0; x--){

		for(int y = width - 1; y >= 0; y--){

			//contains only novel motifs.
			novel = set<int>();

			//contains maximal motifs at (x,y)
			maximalxy = set<int>();

			if(useL)
				aggiorna_L(img, L, x, y, height, width, dontcare);

			compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
					max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

			if(false){
				cout << "Novels{ " <<endl;
				for(set<int>::iterator it = novel.begin(); it != novel.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			//contains new motif but not novels.
			set<int> newMotif = set<int>();

			//contains maximal motif already present in basis.
			set<int> oldMotif = set<int>();

			set<int> oldMotifgood = set<int>();

			separate(maximal, core, base, maximalxy, novel, newMotif, oldMotif, oldMotifgood, dontcare, x, y, img, height, width);

			extract(maximal, base, novel, newMotif, oldMotif, oldMotifgood, dontcare);

	if(x==0 && y==0){

					set<int> sub = set<int>();
					set<int> sup = set<int>();

					MyTrie<T> maximalTemp = MyTrie<T>();

					for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					  for(set<int>::iterator it1 = base.begin(); it1 != base.end(); it1++){
						if((*it1)!= (*it)){
							T** mi=maximal.getMotif(*it);
							int rows1=maximal.getRows(*it);
							int cols1=maximal.getCols(*it);

							T** mj=maximal.getMotif(*it1);
							int rows2=maximal.getRows(*it1);
							int cols2=maximal.getCols(*it1);

							if((rows1<=rows2) && (cols1<=cols2)){
								int* c;
								int* d;
								int size_c=0;
								int size_d=0;
								if(containMotif(mj, mi, rows2, cols2, rows1, cols1, &c, &d, size_c, size_d, dontcare)){
									sub.insert(*it);
									//mi � sottomotivo di mj
									//per ogni coppia in c e d scostamento di riga e colonna devo:
									 //1. andare nel motivo pi� grande e isolare la sottoporzione
									 //2. prendere la lista di occorrenze del motivo grande e aggiungere per ogni occorrenza lo scostamento di riga e colonna
									 //3. creare un pezzo autonomo da inserire nella maximalTemp con un indice mio

									for(int index_c=0; index_c<size_c; index_c++){
										int index_trie;
										int occ_row=c[index_c];
										int occ_col=d[index_c];

										//1. isoliamo la sottoporzione dentro mj
										//height, int width
										int dim1=rows2-occ_row;
										int dim2=cols2-occ_col;

										T** subimage = new T*[dim1];
										for (int p = 0; p < dim1; p++)
												subimage[p] = new T[dim2];

										for(int p=0; p<dim1;p++)
											for(int q=0;q<dim2;q++)
												subimage[p][q]=mj[p+occ_row][q+occ_col];

										//2. aggiorno lista occorrenze motivo grande
										int* occ_mj=maximal.getOccurence(*it1);
										int n_occ_mj=maximal.getNOccurence(*it1);

										int* new_occ_mj=new int[n_occ_mj];

										for(int p=0;p<n_occ_mj-1;p+=2){
											new_occ_mj[p]=occ_mj[p]+occ_row;
											new_occ_mj[p+1]=occ_mj[p+1]+occ_col;}
										//3. inserisco sottoporzione + sua lista occorrenze nella nuova trie
										maximalTemp.insElements(subimage, dim1, dim2, new_occ_mj, n_occ_mj, index_trie);

										sup.insert(index_trie);
										}//for index_c
								}//if containMotif

							}//if dim
					   }//if mi!=mj
				  }//for
				 }//for

				//a questo punto abbiamo sup e sub, la nostra trie maximal e la nuova trie maximal_temp che contiene sup
				//creiamo una compactTrie che riempiamo con sup, poi buttiamo dentro sub

				CompactTrie<T> compT = CompactTrie<T>(&maximalTemp, sup, dontcare);

				set<int> new_base = set<int>();

				set<int>::iterator iter = sub.begin();
					for(; iter != sub.end(); iter++){
						T* motif = maximal.getLinearMotif(*iter);
						int size = maximal.getLinearSize(*iter);
						int* occ = maximal.getOccurence(*iter);
						int n_occ = maximal.getNOccurence(*iter);
						int id = compT.tryToInsValue(motif, size, occ, n_occ, dontcare);
						if(id != -1)
							new_base.insert(*iter);
					}

				//calcolo la differenza tra sub e base e unisco a questa new_base
				set<int> difference = set<int>();
					for (set<int>::iterator it = base.begin(); it != base.end(); it++) {
						     bool chk =  (sub.find(*it) != sub.end());
						     if(!chk){
								 difference.insert(*it);
						     }
				    }
				  for (set<int>::iterator it = new_base.begin(); it != new_base.end(); it++)
				  			difference.insert(*it);

					base=difference;
				}//if x=0 && y=0

if (debug) {
   cout << "Base " << x << ", " << y << " { " << endl;
   for (set<int>::iterator it = base.begin(); it != base.end(); it++) {
      cout << *it << ": ";
      maximal.navigate(*it);

    }
    cout << endl << "}" << endl;
   }

 }//for
}//for

if(debug){

	cout << "Maximal elems: {" << endl;
	maximal.printPaths();
	cout << "}" << endl;

}
	cout << "Last Base{ " <<endl;
	for(set<int>::iterator it = base.begin(); it != base.end(); it++){
		cout << *it << ": " ;
		maximal.navigate(*it);
	}
	cout << endl << "}" << endl;

	*basis=base;

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


///Extract procedure.
/*!
 * Generates the new basis removing redundant motifs.
 * \param maximal trie of all maximal motifs.
 * \param base set of basis of irredundant motifs.
 * \param novel set of novel motifs.
 * \param newMotif set of new motifs.
 * \param oldMotif set of old motifs.
 * \param oldMotifGood set of old motifs to put directly into the basis
 * \param dontcare dontcare symbol.
 */
template<typename T>
void extract(MyTrie<T>& maximal, set<int>& base, set<int>& novel,
		set<int>& newMotif, set<int>& oldMotif, set<int>& oldMotifgood, T dontcare) {

	CompactTrie<T> cc = CompactTrie<T>(&maximal, novel, dontcare);

	set<int>::iterator iter_old = oldMotif.begin();
	for(; iter_old != oldMotif.end(); iter_old++){
		T* motif = maximal.getLinearMotif(*iter_old);
		int size = maximal.getLinearSize(*iter_old);
		int* occ = maximal.getOccurence(*iter_old);
		int n_occ = maximal.getNOccurence(*iter_old);

		int id = cc.tryToInsValue(motif, size, occ, n_occ, dontcare);

		if(id != -1){
			cc.my_trie_compact_trie[*iter_old] = id;}
		}

set<int>::iterator iter_new = newMotif.begin();
	for(; iter_new != newMotif.end(); iter_new++){
		T* motif = maximal.getLinearMotif(*iter_new);
		int size = maximal.getLinearSize(*iter_new);
		int* occ = maximal.getOccurence(*iter_new);
		int n_occ = maximal.getNOccurence(*iter_new);

		int id = cc.tryToInsValue(motif, size, occ, n_occ, dontcare);

		if(id != -1)
			cc.my_trie_compact_trie[*iter_new] = id;
	}

	//inizialize the reference of the new basis.
	base = set<int>();

	map<int, int>::iterator it = cc.my_trie_compact_trie.begin();
	for(; it != cc.my_trie_compact_trie.end(); it++ ){
		base.insert((*it).first);
	}

	set<int>::iterator iter_old_good = oldMotifgood.begin();
	for(; iter_old_good != oldMotifgood.end(); iter_old_good++){
		base.insert((*iter_old_good));
	}

}


///Separates between old motifs and new motifs (but not novel).
/*!
 *From old motif eliminates the motifs without occourences at (i,j) that are submotif of old motif or new motif of old basis.
 *From old motif without occurrences at (i,j) eliminates submotifs of motifs extracted at (i,j).
 * \param maximal trie of all maximal motifs.
 * \param base set of basis of irredundant motifs.
 * \param maximalxy set of motifs extracted at (i,j).
 * \param newMotif set of new motifs.
 * \param oldMotif set of old motifs.
 * \param oldMotifGood set of old motifs to put directly into the basis.
 * \param dontcare dontcare symbol.
 * \param i first coordinate of just extracted motif.
 * \param j second coordinate of just extracted motif.
*/
template<typename T>
void separate(MyTrie<T>& maximal, MyCore<T>& core, set<int>& base, set<int>& maximalxy,
set<int>& novel,
         set<int>& newMotif, set<int>& oldMotif, set<int>& oldMotifgood,
T dontcare, int i, int j, T** image, int h, int w) {

     set<int> o_primo;
     set<int> o_secondo;

     for (set<int>::iterator it = base.begin(); it != base.end(); it++) {

         bool chk =  (maximalxy.find(*it) != maximalxy.end());
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

     for (set<int>::iterator it = maximalxy.begin(); it !=maximalxy.end(); it++) {
         bool chk = (o_primo.find(*it) != o_primo.end());
             if(chk){

             }else{

                 newMotif.insert(*it);
             }
     }

set<int>lista=set<int>();

for (set<int>::iterator o2_c = o_secondo.begin(); o2_c !=o_secondo.end(); o2_c++){

	T** old_motif_curr=maximal.getMotif(*o2_c);

	int dim_r=maximal.getRows(*o2_c);
	int dim_c=maximal.getCols(*o2_c);

	if(((w-j)>=dim_c) && ((h-i)>=dim_r)){
		bool flag=true;

		for(int cd=0;cd<dim_r && flag;cd++)
			for(int cf=0;cf<dim_c && flag;cf++)
				if((old_motif_curr[cd][cf]!= image[cd+i][cf+j]) && (old_motif_curr[cd][cf]!=dontcare))
					flag=false;
		if(flag){

			o_primo.insert(*o2_c);
			oldMotif.insert(*o2_c);
			lista.insert(*o2_c);

		}

	}
}

for (set<int>::iterator l = lista.begin(); l !=lista.end(); l++)
	o_secondo.erase(*l);

/* Note that the only case in which an old motif M not occurring at
[i, j] can be obliterated is when M is a submotif of some other motifs in
Class I or II*/

     for (set<int>::iterator o2 = o_secondo.begin(); o2 !=o_secondo.end(); o2++) {
         bool submotif=false;
         for (set<int>::iterator o1 = o_primo.begin(); o1 !=o_primo.end(); o1++) {

             int* OPrimo = maximal.getOccurence(*o1);

             int* OSecondo = maximal.getOccurence(*o2);

             int n_occ = maximal.getNOccurence(*o2);

             int n_occPrimo=maximal.getNOccurence(*o1);
             bool b_condition = (n_occ==n_occPrimo);
             bool c_condition = false;
             bool trovato = false;
             int off_r=0;
             int off_c=0;

             if(b_condition){
                 for(int index=2; (index<n_occ-1)&&(!trovato); index+=2){
                     if((OPrimo[0]-OSecondo[0] ==OPrimo[index]-OSecondo[index])
&& (OPrimo[1]-OSecondo[1] == OPrimo[index+1]-OSecondo[index+1])){
                                 trovato=true;
                                 off_r = OSecondo[0]-OPrimo[0];
                                 off_c = OSecondo[1]-OPrimo[1];
                     }
                 }
                 if (trovato) c_condition=true;
             }

            if(c_condition && off_r >=0 && off_c >=0){

		   				T** motif_o2 = maximal.getMotif(*o2);
		   				T** motif_o1 = maximal.getMotif(*o1);

		   				int r_o1 = maximal.getRows(*o1);
						int c_o1 = maximal.getCols(*o1);

		   				int r_o2 = maximal.getRows(*o2);
		   				int c_o2 = maximal.getCols(*o2);

		   					bool flag=true;
		   					for(int x=0;x<r_o2 && flag;x++){
		   			           	for(int y=0;y<c_o2 && flag;y++){
									if((x+off_r)<r_o1 && (y+off_c<c_o1)){
   			              		if(motif_o1[x+off_r][y+off_c]!= motif_o2[x][y] && motif_o2[x][y]!=dontcare)
		   								flag=false;
		   								}else flag=false;
		   			                   }//for
		   			              }//for
		   							if(flag){
		   			         			submotif=true;
		   			         			break;}
                 }//if c
			   }//for

         if(!submotif){
             for (set<int>::iterator o = newMotif.begin(); o !=newMotif.end(); o++) {

                 int* OPrimo  = maximal.getOccurence(*o);

                 int* OSecondo = maximal.getOccurence(*o2);

                 int n_occ = maximal.getNOccurence(*o2);

                 int n_occPrimo=maximal.getNOccurence(*o);
                 bool b_condition = (n_occ==n_occPrimo);
                 bool c_condition = false;
                 bool trovato = false;
                 int off_r=0;
                 int off_c=0;

                 if(b_condition){

                     for(int index=2; (index<n_occ-1)&&(!trovato);index+=2){
                         if(((OPrimo[0]-OSecondo[0]) == (
OPrimo[index]-OSecondo[index]))
&& ((OPrimo[1]-OSecondo[1]) == (OPrimo[index+1]-OSecondo[index+1]))){

                                     trovato=true;
                                     off_r = OSecondo[0]-OPrimo[0];
                                     off_c = OSecondo[1]-OPrimo[1];
                         }
                     }
                 if (trovato) {c_condition=true;}
             }

            if(c_condition && off_r >=0 && off_c >=0){

					T** motif_o2 = maximal.getMotif(*o2);
					T** motif_o = maximal.getMotif(*o);

					int r_o2 = maximal.getRows(*o2);
					int c_o2 = maximal.getCols(*o2);

					int r_o = maximal.getRows(*o);
					int c_o = maximal.getCols(*o);

					bool flag=true;
					for(int x=0;x<r_o2 && flag;x++){
			           	for(int y=0;y<c_o2 && flag;y++){
							if((x+off_r)<r_o && (y+off_c<c_o)){
			              	if(motif_o[x+off_r][y+off_c]!= motif_o2[x][y] && motif_o2[x][y]!=dontcare)
								flag=false;
								}else flag=false;

			                   }//for
			              }//for
							if(flag){
			         			submotif=true;
			         			break;}
                 }//if c

			             }//for
         }//if
         if(!submotif){
             for (set<int>::iterator ow = novel.begin(); ow !=novel.end(); ow++) {
                 int* OPrimo = maximal.getOccurence(*ow);

                 int* OSecondo = maximal.getOccurence(*o2);
                 int n_occ = maximal.getNOccurence(*o2);

                 int n_occPrimo=maximal.getNOccurence(*ow);

                 bool b_condition = (n_occ==n_occPrimo);
                 bool c_condition = false;
                 bool trovato = false;

                 int off_r=0;
                 int off_c=0;

                 if(b_condition){
                     for(int index=2; (index<n_occ-1)&&(!trovato);index+=2){
                         if(((OPrimo[0]-OSecondo[0]) == (OPrimo[index]-OSecondo[index]))
							&& ((OPrimo[1]-OSecondo[1]) == (OPrimo[index+1]-OSecondo[index+1]))){
                                     trovato=true;
                                     off_r = OSecondo[0]-OPrimo[0];
                                     off_c = OSecondo[1]-OPrimo[1];
                         }
                     }
                 if (trovato) c_condition=true;
             }

 if(c_condition && off_r >=0 && off_c >=0){

	T** motif_o2 = maximal.getMotif(*o2);
	T** motif_ow = maximal.getMotif(*ow);

	int r_o2 = maximal.getRows(*o2);
	int c_o2 = maximal.getCols(*o2);

	int r_ow = maximal.getRows(*ow);
	int c_ow = maximal.getCols(*ow);

		bool flag=true;
		for(int x=0;x<r_o2 && flag;x++){
           	for(int y=0;y<c_o2 && flag;y++){
			if((x+off_r)<r_ow && (y+off_c<c_ow)){

              	if(motif_ow[x+off_r][y+off_c]!= motif_o2[x][y] && motif_o2[x][y]!=dontcare)
					flag=false;

				}else flag=false;
                   }//for
              }//for
				if(flag){
         			submotif=true;
         			break;}
                 }//if c
             }//for
         }//if
         if(!submotif){
             oldMotifgood.insert(*o2);
         }
     }

}


///List L procedure.
/*!
 * This method verifies if core's rows and columns at distance c and d correspond to the first\n
 * row and column of the maximal motif at the current step. If it is verified, the list of\n
 * occurrences of the maximal motif is extended.
 * This procedure lets to obtain a huge saving in computational time, however a big memory space\n
 * is required, so it can be used only for reduced dimension arrays.
 * The procedure is alternative to listNaive.
 * \param L vector that will be deallocated
 * \param maximalxy set of motifs extracted at (i,j).
 * \param core set of cores
 * \param maximal trie of all maximal motifs.
 * \param novel set of indexes of novel motifs
 * \param autoc current autocorrelation
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param i first coordinate of second bite.
 * \param j second coordinate of second bite.
 * \param rows number of rows for autoc
 * \param cols number of columns for autoc
 * \param img original image
 * \param img_r image number of rows
 * \param img_c image number of cols
 * \param dontcare don't care character
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
			novel.insert(index);
		}
	}
}


///List naive procedure.
/*!
 * For each meet this method generates the list of occurrences for the region R(i,j)
 * It identifies also the novel motifs. At the end of the procedure, the method updates the list of occurrences for the existing motifs.
 * \param L vector that will be deallocated
 * \param maximalxy set of motifs extracted at (i,j).
 * \param core set of cores
 * \param maximal trie of all maximal motifs.
 * \param novel set of indexes of novel motifs
 * \param autoc current autocorrelation
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param i first coordinate of second bite.
 * \param j second coordinate of second bite.
 * \param rows number of rows for autoc
 * \param cols number of columns for autoc
 * \param img original image
 * \param img_r image number of rows
 * \param img_c image number of cols
 * \param dontcare don't care character
 */

template<typename T>
void listNaive(int****L, set<int>& maximalxy, MyCore<T>& core, MyTrie<T>& maximal, set<int>& novel, T** autoc, int sup,
		int sx, int i, int j, int rows, int cols, T** img, int img_r, int img_c, T dontcare) {

	bool debug = false;
//calcolo gli indici del core
	int c = -1;
	int d = -1;
	int n_cups;
	int* cups = get_cups(autoc, rows, cols, dontcare, n_cups);

	if(cups[n_cups-2] == -1 || maximal.getRoot() == NULL){
// se il core è vuoto ...
		if((rows*cols)>1){

			int dim_r=rows-1;
			int dim_c=cols-1;

			int n_occurence=0;

			for (int inde = sup+1; inde < (img_r-dim_r+1); inde++) {
				for (int indi = sx+1; indi < (img_c-dim_c+1); indi++) {
					n_occurence++;
					}
			}

			for (int inde = sup+2; inde < (img_r-dim_r+1); inde++) {
				for (int indi = 1; indi < (sx-dim_c+1); indi++) {
					n_occurence++;
					}
			}

			int* occurence=new int[n_occurence*2];
			int index_occ=n_occurence*2-1;

			for (int inde = sup+1; inde < (img_r-dim_r+1); inde++) {
				for (int indi = sx+1; indi < (img_c-dim_c+1); indi++) {
					occurence[index_occ]=indi;
					index_occ--;
					occurence[index_occ]=inde;
					index_occ--;
				}
			}

			for (int inde = sup+2; inde < (img_r-dim_r+1); inde++) {
				for (int indi = 1; indi < (sx-dim_c+1); indi++) {
					occurence[index_occ]=indi;
					index_occ--;
					occurence[index_occ]=inde;
					index_occ--;
				}
			}

		int** ret_occ;
		int now_size = 4;
		int* now_occ = new int[now_size];

		now_occ[0] = i;
		now_occ[1] = j;
		now_occ[2] = sup;
		now_occ[3] = sx;
		ret_occ = tryToExtend(maximal, occurence, n_occurence*2, autoc, rows, cols, sup, sx, i, j, 1, 1, img, img_r, img_c);

		int arg_size = *(ret_occ[0]);
		int* args = ret_occ[1];

		int* up_occ = MyTrie<T>::mergeOccurence(now_occ, now_size, args, arg_size);

		delete args;

		int index;

		int novel_occ = *maximal.insElements(autoc, rows, cols, up_occ, now_size, index)[0];

		if(novel_occ == 4){
// inserisco il motivo novello nell'insieme dei novelli
			novel.insert(index);
		}else{
			if(novel.find(index) != novel.end())
					novel.erase(index);
			maximalxy.insert(index);
		}
//inserisco il puntatore al core del motivo corrente
		if(index != -1){
			if(debug)
				cout << "EXT motif: " << endl;
			core.insElement(i, j, sup, sx, -1, -1, index);
		}

		maximal.updateTrie(i, j, sup, sx, autoc, rows, cols, dontcare);

	}//if autoc

	else{

		int index;
		int n_ret_occ = 0;

		n_ret_occ = *maximal.insElements(autoc, rows, cols, i, j, sup, sx, index)[0];

		if(index != -1){
			if(debug){
				cout << "-core empty, motif: " << endl;
				printBite(autoc, 0, 0, rows, cols);
			}

			core.insElement(i, j, sup, sx, -1, -1, index);
		}

		if(n_ret_occ == 4){

			novel.insert(index);

		}else{

			if(novel.find(index) != novel.end())
					novel.erase(index);

	      maximalxy.insert(index);

		}
	}
}
else{
		c = cups[n_cups-2];
		d = cups[n_cups-1];
		if(debug){
			cout << "-core full, motif: " << endl;
			printBite(autoc, 0, 0, rows, cols);
		}
		int core_id = -1;

		if(debug){
			cout << "autoc: sup:" << sup << " sx:" << sx << " i:" << i << " j:" << j << endl;
			cout << "GET core: " << endl;
			printCore(autoc, c, d, rows, cols);
		}

		core_id = core.getCore(i, j, sup, sx, c, d);
	// il core è già stato inserito a qualche passo precedente.
		int n_occurence = maximal.getNOccurence(core_id);
		int* occurence = maximal.getOccurence(core_id);

		int** ret_occ;

		int now_size = 4;
		int* now_occ = new int[now_size];
		now_occ[0] = i;
		now_occ[1] = j;
		now_occ[2] = sup;
		now_occ[3] = sx;

		ret_occ = tryToExtend(maximal, occurence, n_occurence, autoc, rows, cols, sup, sx, i, j, c, d, img, img_r, img_c);

		int arg_size = *(ret_occ[0]);
		int* args = ret_occ[1];

		if(debug){
			cout << "before ";
			for(int u = 0; u<arg_size; u++)
				cout << " " << args[u];
			cout << endl;
		}

		int* up_occ = MyTrie<T>::mergeOccurence(now_occ, now_size, args, arg_size);

		if(debug){
			cout << "after ";
			for(int u = 0; u<now_size; u++)
				cout << " " << up_occ[u];
			cout << endl;
		}

		delete args;

		int index;

		int novel_occ = *maximal.insElements(autoc, rows, cols, up_occ, now_size, index)[0];

		if(novel_occ == 4){

			novel.insert(index);
		}else{
			if(novel.find(index) != novel.end())
					novel.erase(index);
			maximalxy.insert(index);
		}

		if(index != -1){
			if(debug)
				cout << "EXT motif: " << endl;
			core.insElement(i, j, sup, sx, -1, -1, index);
		}

		maximal.updateTrie(i, j, sup, sx, autoc, rows, cols, dontcare);
	}
}

///List procedure.
/*!
 * ListNaive or ListL is choosen considering the dimension of the input image.
 * \param L vector that will be deallocated
 * \param maximalxy set of motifs extracted at (i,j).
 * \param core set of cores
 * \param maximal trie of all maximal motifs.
 * \param novel set of indexes of novel motifs
 * \param autoc current autocorrelation
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param i first coordinate of second bite.
 * \param j second coordinate of second bite.
 * \param rows number of rows for autoc
 * \param cols number of columns for autoc
 * \param img original image
 * \param img_r image number of rows
 * \param img_c image number of cols
 * \param dontcare don't care character
 */

template<typename T>
void list(int****L, set<int>& maximalxy, MyCore<T>& core, MyTrie<T>& maximal, set<int>& novel, T** autoc, int sup,
		int sx, int i, int j, int rows, int cols, T** img, int img_r, int img_c, T dontcare) {

	if(L != NULL){
		listL(L, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, dontcare);

	}else{
		listNaive(L, maximalxy, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, img, img_r, img_c, dontcare);
	}
}

///Compilation of all the motifs with a maximal occurrence at [i, j].
/*!
 * Generates all the meets among each bite and all the bites of the current region.
 * \param maximalxy set of motifs extracted at (i,j).
 * \param core set of cores
 * \param maximal trie of all maximal motifs.
 * \param novel set of indexes of novel motifs
 * \param img original image
 * \param L vector that will be deallocated
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param height image number of rows
 * \param width image number of cols
 * \param dontcare don't care character
 * \param max_rows maximal rows limit for motif that could be extracted.
 * \param max_cols maximal columns limit for motif that could be extracted.
 * \param max_dontcare maximal dontcare limit for motif that could be extracted.
 * \param min_rows minimal rows limit for motif that could be extracted.
 * \param min_cols minimal columns limit for motif that could be extracted.
 * \param optimize flag for enabling optimization in motif extraction.
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
		for (j = sx; j < width; j++) {

			if(sup== i && sx==j){

			}//sono interessato ai 2-motif non devo prendere il meet del bite(i,j) con se stesso
			else{
				//COMPILE: autoc è un motivo massimale con occorrenza a (i,j)

				T** autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
						cols, n_dontcare, optimize, max_rows, max_cols, max_dontcare,
						min_rows, min_cols);
				if (rows > 0 && cols > 0) {
cout<<"meet:"<<" sup:"<<sup<<" sx:"<<sx<<" i:"<<i<<" j:"<<j<<endl;
					list(L, maximalxy, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, img, height, width, dontcare);
					int index_curr=maximal.find(autoc,rows,cols,-1);
					int* occ_curr=maximal.getOccurence(index_curr);

					for (x = 0; x < rows; x++)
						delete autoc[x];
					delete autoc;
				}
			}
		}
	}

	for (i = sup+1; i < height; i++) {
			for (j = 0; j < sx; j++) {

				if(sup== i && sx==j){

				}
				else{

					T** autoc = meet(img, sup, sx, i, j, height, width, dontcare, rows,
							cols, n_dontcare, optimize, max_rows, max_cols, max_dontcare,
							min_rows, min_cols);
					if (rows > 0 && cols > 0) {
cout<<"meet:"<<" sup:"<<sup<<" sx:"<<sx<<" i:"<<i<<" j:"<<j<<endl;
						list(L, maximalxy, core, maximal, novel, autoc, sup, sx, i, j, rows, cols, img, height, width, dontcare);
						for (x = 0; x < rows; x++)
							delete autoc[x];
						delete autoc;
					}
				}
			}
	}

}

///Returns an array of integer that are in sequence rows index and column index for cusps of a specific motif.
/*!
 * Returns as output an array of integers representing the row-column indexes of the cups.
 * In the two last positions of the array, the indexes of the cup identifying the core of the input meet are contained.
 * \n\b note. If the core does not exist, the indexes are both -1.
 * \param autoc current autocorrelation
 * \param height number of rows for autoc
 * \param width number of cols for autoc
 * \param dontcare don't care character
 * \param n_cups number of cups
 * \return returns an array of indexes of the cups for autoc
 */

template<typename T>
int* get_cups(T** autoc, int height, int width, T dontcare, int& n_cups) {

	//casi particolari in una dimensione
	if(height == 1){
		//ho solo una riga
		n_cups = 0;
		//al più posso avere width-1 cuspidi
		//ogni cuspide ha 2 indici che la individuano
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

///Computes the meet between bite at (sup, sx) and bite at (i, j) of an image img.
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
 * \return returns the meet between bite at (sup, sx) and bite at (i, j) for img
 */
template<typename T>
T** meet(T** img, int sup, int sx, int i, int j, int rows, int cols,
		T dontcare, int& rows_meet, int& cols_meet, int& n_dontcare,
		bool compile_opt = true, int opt_row_max_size = -1,
		int opt_col_max_size = -1, int opt_max_n_dontcare = -1,
		int opt_row_min_size = -1, int opt_col_min_size = -1) {
	return consensus(img, sup, sx, i, j, rows, cols, dontcare, rows_meet,
			cols_meet, n_dontcare, compile_opt, true,
			opt_row_max_size, opt_col_max_size, opt_max_n_dontcare,
			opt_row_min_size, opt_col_min_size);
}

///Computes the consensus between bite at (sup, sx) and bite at (i, j) of an image img.
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
 * \return returns the consensus between bite at (sup, sx) and bite at (i, j) for img.
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

		for (x = 0; x < (rows); x++) {
			// y = 0

			if (img[x + i][y + j ] == img[x + sup][y + sx ]) {
				break;
			} else {
				first_col_n_dontcare++;
			}
		}
		if (first_col_n_dontcare == (rows) && (cols) >= 1) {
			// prima colonna tutti don't care alla procedura compile questo nn serve
			rows_meet = 0;
			cols_meet = 0;
			return NULL;
		}

		//azzero x
		x = 0;
		for (y = 0; y < (cols); y++) {
			// x = 0
			if (img[x + i][y + j ] == img[x + sup][y + sx ]) {
				break;
			}else {
				first_row_n_dontcare++;
			}
		}
		if (first_row_n_dontcare == (cols) && (rows) >= 1) {

			// prima riga tutti don't care alla procedura compile questo nn serve
			rows_meet = 0;
			cols_meet = 0;

			return NULL;
		}
	}
	//

	T** immagine = new T*[rows];
	T * *meet;


	//devo allocare prima il vettore per intero
	//perchè altrimenti ho problemi a deallocare.
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


/// Clears all files into the basis directory.
/*!
 * \param dirname basis directory.
 */
void clear_dir(char* dirname){

	DIR *pdir;
	struct dirent *pent;
	pdir=opendir(dirname);
	 if (!pdir){
		 return;
	 }

	 while ((pent=readdir(pdir)))
	 {
	  string file_delete=pent->d_name;
	  ostringstream oss;
	  oss << dirname<<  "/" << file_delete;
	  string filename = oss.str();
	  remove(filename.c_str());
	 }

	 closedir(pdir);
}

///Writes on disk all elements of a specific basis.
/*!
 * \param dirname output directory.
 * \param base specific basis set.
 * \param maximal maximal motifs set.
 */
void write_base(char* dirname, set<int> base, MyTrie<int>& maximal) {

    if (mkdir(dirname, 0777) == -1) {

    }
    for (set<int>::iterator it = base.begin(); it != base.end(); it++) {

        int** motif = maximal.getMotif(*it);
        int motif_r = maximal.getRows(*it);
        int motif_c = maximal.getCols(*it);
        ostringstream oss;
        oss << dirname << "/base_" << *it << ".bmp";
        string filename = oss.str();
        BMPImage consintimage = BMPImage(filename.c_str());
        consintimage.set_int(motif, motif_r, motif_c);
        consintimage.writeBmpFile();
    }
}
///Writes on disk all autocorrelations generated from a specific bite.
/*!
 * This method writes on disk all images obtained from the autocorrelations of bite in pos (sup, sx)\n
 * for the image with dimension height*width.
 * \n\b note. it is possible to limit the generated autocorrelations number.
 * \param filename name of the folder where meets have to be stored.
 * \param img input image.
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param height rows number of input image.
 * \param width columns number of input image.
 * \param limit limit on the extension of the autocorrelations.
 */
void write_autocorrelation(char* filename, int** img, int sup, int sx, int height, int width,
		int limit = -1) {

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
		for (j = sx; j < width_limit; j++) {
			if (i == sup && j == sx){}

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
		}
	}

	for (i = sup+1; i < heigh_limit; i++) {
			for (j = 0; j < sx; j++) {
				if (i == sup && j == sx){}

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
			}
		}

}

///Called at the end of list procedure for updating existing motif list of occurrences.
/*!
 * This method is called at the end of the list procedure for updating exsisting motif list of occurrences.\n
 * In particular, the procedure tries to extend the occurrences of the core for autoc in order to update\n
 * the autoc's list of occurrences.
 * \param maximal all maximal motifs
 * \param occurence list of occurrences for autoc's core.
 * \param n_occurence number of occurrences for autoc's core
 * \param autoc current autocorrelation
 * \param rows rows number for autoc
 * \param cols cols number for autoc
 * \param sup first coordinate of first bite.
 * \param sx second coordinate of first bite.
 * \param i first coordinate of second bite.
 * \param j second coordinate of second bite.
 * \param c row offset for core
 * \param d column offset for core
 * \param img input image
 * \param img_r rows number for img
 * \param img_c cols number for img
 * \note ret_occ[0] ==> occurrences number, ret_occ[1] ==> pointer to the occurrences, int* occurence ==> occurrences of the core for autoc at distance (c,d).
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

		// verifica a destra di sx


	 if((_y > sx-1) && (_x - c >= sup) && (_y - d >= sx)){

			//verifica sulla riga


		  for(int k = 0; k < cols; k++){



			if((_y - d + k < img_c) && ((img[_x - c][_y - d + k] == autoc[0][k])||autoc[0][k]=='.')){

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
				if((_x - c + h < img_r) && (_y - d > 0) && ((img[_x - c + h][_y - d] == autoc[h][0])||autoc[h][0]=='.')){
					//cout<<"img[_x - c+h][_y - d]:"<<img[_x - c + h][_y - d]<<endl;
				    //cout<<"autoc[h][0]:"<<autoc[h][0]<<endl;
					// è possibile che autoc estenda l'occorrenza (_x,_y) del suo core...
				}else{
					// NON è possibile che autoc estenda l'occorrenza (_x,_y) del suo core.
					can_extend = false;
					break;
				}
			}
		}


		}else if((_x - c >= sup+1) && (_y - d >=0)){

			//verifica sulla riga

					  for(int k = 0; k < cols; k++){
						if((_y - d + k < img_c) && ((img[_x - c][_y - d + k] == autoc[0][k])||autoc[0][k]=='.')){
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
							if((_x - c + h < img_r) && (_y - d > 0) && ((img[_x - c + h][_y - d] == autoc[h][0])||autoc[h][0]=='.')){
								// è possibile che autoc estenda l'occorrenza (_x,_y) del suo core...
							}else{
								// NON è possibile che autoc estenda l'occorrenza (_x,_y) del suo core.
								can_extend = false;
								break;
							}
						}
				}

			} else {
			// NON è possibile che autoc estenda l'occorrenza (_x,_y) del suo core.
			can_extend = false;

		}


		if(((_x - c) < 0) || ((_y - d) < 0)){
			can_extend = false;
		}

		if(can_extend){
			// a questo punto se can_extend vale TRUE significa che
			// autoc estende l'occorrenza (_x,_y) del suo core!!

			ret_occurence[ret_n_occ++] = _x - c;
			ret_occurence[ret_n_occ++] = _y - d;
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


 ///Generates all basis of an input image and saves on disk images for each element of each basis.
 /*!Generates all basis of image img with size height x width using incremental approach and
  * saves on disk the list of motifs for each basis.\n
  * DEBUG prints can be enabled from the debug flag and don't care symbol can be specified from dontcare generics attribute.
  * \param dirname name of the folder where basis has to be saved.
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
void process_and_save(char* dirname, T** img, int height, int width, T dontcare,
		int max_rows, int max_cols, int max_dontcare, int min_rows, int min_cols, bool optimize, bool debug) {

	if(debug)
		cout << endl << "start process" << endl << endl;

	unsigned int sizeOfL = (height*height*height*width)* sizeof(int);

	bool useL = false;
	int**** L = NULL;
	if(sizeOfL > 1000000000){
		cout << "max size of L: 1GB" << endl;
		useL = false;
	}
	if(debug && useL)
		cout << "creating " << sizeOfL << " bytes for L..." << endl;

	if(useL){
		L = get_L(height, width, 0);//L = get_L(height, width, NULL);
		if(debug)
			cout << "created L of: "<< sizeOfL << " bytes!" << endl;
	}

	MyTrie<T> maximal = MyTrie<T>();
	MyCore<T> core = MyCore<T>();
	set<int> maximalxy;
	set<int> novel;
	set<int> base;

	//contains maximal motifs at (x,y)

	for(int x = height - 1; x >= 0; x--){

		for(int y = width - 1; y >= 0; y--){

			//contains only novel motifs.
			novel = set<int>();

			//contains maximal motifs at (x,y)
			maximalxy = set<int>();

			if(useL)
				aggiorna_L(img, L, x, y, height, width, dontcare);

			compile(maximalxy, core, maximal, novel, img, L, x, y, height, width, dontcare,
					max_rows, max_cols, max_dontcare, min_rows, min_cols, optimize);

			if(false){
				cout << "Novels{ " <<endl;
				for(set<int>::iterator it = novel.begin(); it != novel.end(); it++){
					cout << *it << ", ";
				}
				cout << "}" << endl;
			}

			//contains new motif but not novels.
			set<int> newMotif = set<int>();

			//contains maximal motif already present in basis.
			set<int> oldMotif = set<int>();

			set<int> oldMotifgood = set<int>();

			separate(maximal, core, base, maximalxy, novel, newMotif, oldMotif, oldMotifgood, dontcare, x, y, img, height, width);

			extract(maximal, base, novel, newMotif, oldMotif, oldMotifgood, dontcare);

	if(x==0 && y==0){

					set<int> sub = set<int>();
					set<int> sup = set<int>();

					MyTrie<T> maximalTemp = MyTrie<T>();

					for(set<int>::iterator it = base.begin(); it != base.end(); it++){
					  for(set<int>::iterator it1 = base.begin(); it1 != base.end(); it1++){
						if((*it1)!= (*it)){
							T** mi=maximal.getMotif(*it);
							int rows1=maximal.getRows(*it);
							int cols1=maximal.getCols(*it);

							T** mj=maximal.getMotif(*it1);
							int rows2=maximal.getRows(*it1);
							int cols2=maximal.getCols(*it1);

							if((rows1<=rows2) && (cols1<=cols2)){
								int* c;
								int* d;
								int size_c=0;
								int size_d=0;
								if(containMotif(mj, mi, rows2, cols2, rows1, cols1, &c, &d, size_c, size_d, dontcare)){
									sub.insert(*it);
									//mi � sottomotivo di mj
									//per ogni coppia in c e d scostamento di riga e colonna devo:
									 //1. andare nel motivo pi� grande e isolare la sottoporzione
									 //2. prendere la lista di occorrenze del motivo grande e aggiungere per ogni occorrenza lo scostamento di riga e colonna
									 //3. creare un pezzo autonomo da inserire nella maximalTemp con un indice mio

									for(int index_c=0; index_c<size_c; index_c++){
										int index_trie;
										int occ_row=c[index_c];
										int occ_col=d[index_c];

										//1. isoliamo la sottoporzione dentro mj
										//height, int width
										int dim1=rows2-occ_row;
										int dim2=cols2-occ_col;

										T** subimage = new T*[dim1];
										for (int p = 0; p < dim1; p++)
												subimage[p] = new T[dim2];

										for(int p=0; p<dim1;p++)
											for(int q=0;q<dim2;q++)
												subimage[p][q]=mj[p+occ_row][q+occ_col];

										//2. aggiorno lista occorrenze motivo grande
										int* occ_mj=maximal.getOccurence(*it1);
										int n_occ_mj=maximal.getNOccurence(*it1);

										int* new_occ_mj=new int[n_occ_mj];

										for(int p=0;p<n_occ_mj-1;p+=2){
											new_occ_mj[p]=occ_mj[p]+occ_row;
											new_occ_mj[p+1]=occ_mj[p+1]+occ_col;}
										//3. inserisco sottoporzione + sua lista occorrenze nella nuova trie
										maximalTemp.insElements(subimage, dim1, dim2, new_occ_mj, n_occ_mj, index_trie);

										sup.insert(index_trie);
										}//for index_c
								}//if containMotif

							}//if dim
					   }//if mi!=mj
				  }//for
				 }//for

				//a questo punto abbiamo sup e sub, la nostra trie maximal e la nuova trie maximal_temp che contiene sup
				//creiamo una compactTrie che riempiamo con sup, poi buttiamo dentro sub

				CompactTrie<T> compT = CompactTrie<T>(&maximalTemp, sup, dontcare);

				set<int> new_base = set<int>();

				set<int>::iterator iter = sub.begin();
					for(; iter != sub.end(); iter++){
						T* motif = maximal.getLinearMotif(*iter);
						int size = maximal.getLinearSize(*iter);
						int* occ = maximal.getOccurence(*iter);
						int n_occ = maximal.getNOccurence(*iter);
						int id = compT.tryToInsValue(motif, size, occ, n_occ, dontcare);
						if(id != -1)
							new_base.insert(*iter);
					}

				//calcolo la differenza tra sub e base e unisco a questa new_base
				set<int> difference = set<int>();
					for (set<int>::iterator it = base.begin(); it != base.end(); it++) {
						     bool chk =  (sub.find(*it) != sub.end());
						     if(!chk){
								 difference.insert(*it);
						     }
				    }
				  for (set<int>::iterator it = new_base.begin(); it != new_base.end(); it++)
				  			difference.insert(*it);

					base=difference;
				}//if x=0 && y=0

			ostringstream oss;
			oss << dirname << "_" << x;
			string filename = oss.str();

			char* directory = (char*) filename.c_str();

			if (base.size() > 0)
			    clear_dir(directory);

			write_base(directory, base, maximal);

if (debug) {
   cout << "Base " << x << ", " << y << " { " << endl;
   for (set<int>::iterator it = base.begin(); it != base.end(); it++) {
      cout << *it << ": ";
      maximal.navigate(*it);

    }
    cout << endl << "}" << endl;
   }

 }//for
}//for

if(debug){

	cout << "Maximal elems: {" << endl;
	maximal.printPaths();
	cout << "}" << endl;

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


#endif /* MOTIFPATTERNCORE_H_ */
