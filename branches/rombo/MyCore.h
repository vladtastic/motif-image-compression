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

#ifndef MYCORE_H_
#define MYCORE_H_

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <map>
#include <utility>

using namespace std;

///Pair object used for mapping between motif identifier and core key string.
typedef pair<int, string> Pair;
///Core iterator.
typedef map<string, Pair>::iterator CoreIt;

/*! \class MyCore
 *  \brief Wrapper class used to store pointers to motifs stored into a MyTrie and its core.
 *  \author Michele Mastroianni
 *  \mail michele.mastroianni@gmail.com
 *  \version 0.1
 *  \date    25-mar-2009
 *  \see CompactTrie
 */
/*!
 * For each motif stored into a MyTrie, it keeps trace of its core. But the core of a motif is a motif itself,
 * so the structure makes possible to obtain in cascade: motif, core, core of core and so on.
 */
template<typename T>
class MyCore {

private:
	///Map used for self referencing inner core structures.
	/*!
	 * key string is obtained by concatenating the indices of the autocorrelation that identifies the motif \n
	 * Pair value contains the motif identifier on the MyTrie structure and a key string for the core of the motif.
	 */
	map<string, Pair> core_map;
	///Numbers of core stored into the Map.
	int size;

public:
	~MyCore();
	MyCore();
	int getSize();
	void insElement(int sup, int sx, int i, int j, int c, int d, int index);
	int getCore(int sup, int sx, int i, int j, int c, int d);
	Pair getElement(int sup, int sx, int i, int j);
	CoreIt getIterator();
	void print();
};

///Prints on standard output cores status.
template<typename T>
void MyCore<T>::print(){
	cout << "core:{" << endl;
	for(CoreIt it = getIterator(); it != core_map.end(); it++)
	   {
		cout << "key: (" << (*it).first << "); value: [" << (*it).second.first << ", (" << (*it).second.second << ")]" << endl;
	   }
	cout << "}" << endl;
}

///Destructor.
template<typename T>
MyCore<T>::~MyCore() {
	//cout << "TODO ~MyCore" << endl;
}

///Constructor.
template<typename T>
MyCore<T>::MyCore(void){
	size = 0;
}

///Gets a core iterator.
template<typename T>
CoreIt MyCore<T>::getIterator(){
	return core_map.begin();
}

///Gets numbers of core stored into the Map.
template<typename T>
int MyCore<T>::getSize(){
	return size;
}

///Used for updating the cores structure after inserting motifs into the MyTrie object.
/*!
 * The method computes the key value for the autocorrelation just inserted into the MyTrie
 * and his core and updates the core map.
 *\param sup first row index occurrence of the core autocorrelation.
 *\param sx first column index occurrence of the core autocorrelation.
 *\param b_i second row index occurrence of the core autocorrelation.
 *\param b_j second column index occurrence of the core autocorrelation.
 *\param c used as row displacement to identify the core of the motif that was inserted into the trie.
 *\param d used as column displacement to identify the core of the motif that was inserted into the trie.
 *\param index MyTrie motif identifier.
 */
template<typename T>
void MyCore<T>::insElement(int sup, int sx, int i, int j, int c, int d, int index){

	bool debug = false;

	ostringstream oss;
	oss << sup << "," << sx << "," << i << "," << j ;
	string key = oss.str();
	if(debug)
	cout << "ins element : [" << key <<"]" << endl;

	ostringstream oss_core;
	oss_core << (sup + c)<< "," << (sx + d) << "," << (i ) << "," << (j ) ;
	string key_core = oss_core.str();
	if(c == -1 || d == -1)
		key_core = "NULL";

	if(debug)
		cout<< "ins element core c " << c << " d " << d << "  : [" << key_core << "] " << endl;

	 core_map[key] = Pair(index, key_core);
	 size++;
}

///Returns a pair object containing the integer identifier on the MyTrie structure and a key string for the core of the motif.
/*!
 * The method returns a pair that contains the integer identifier on the MyTrie structure
 * and a key string for the core of motif.
 *\param sup first row index occurrence of the autocorrelation.
 *\param sx first column index occurrence of the autocorrelation.
 *\param i second row index occurrence of the autocorrelation.
 *\param j second column index occurrence of the autocorrelation.
 *\return returns the pair that maps the motif with his core.
 *
 */
template<typename T>
Pair MyCore<T>::getElement(int sup, int sx, int i, int j){
	 ostringstream oss;
	 oss << sup << "," << sx << "," << i << "," << j ;
	 string key = oss.str();
	 return core_map[key];
}

///Returns a core identifier of the motif stored into a MyTrie.
/*!
 * The method returns the core index identified on a MyTrie from an autocorrelation.
 *\param sup first row index occurrence of the autocorrelation.
 *\param sx first column index occurrence of the autocorrelation.
 *\param i second row index occurrence of the autocorrelation.
 *\param j second column index occurrence of the autocorrelation.
 *\param c used as row displacement to identify the core of the motif that was inserted into the trie.
 *\param d used as column displacement to identify the core of motif that was inserted into the trie.
 *\return returns the core index.
 *
 */
template<typename T>
int MyCore<T>::getCore(int sup, int sx, int i, int j, int c, int d){

	bool debug = false;

	ostringstream oss_core;
	oss_core << (i)<< "," << (j) << "," << (i + c) << "," << (j + d) ;
	string key_core = oss_core.str();

	if(debug)
		cout << "key_core: [" << key_core << "]" << endl;
	Pair p = core_map[key_core];

	return p.first;

}

#endif /* MYCORE_H_ */
