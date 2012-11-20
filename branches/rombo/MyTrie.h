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

#ifndef MYTRIE_H_
#define MYTRIE_H_


#include <cstdlib>
#include <map>
using namespace std;

/*! \class MyTrie
 *  \brief Ordered data structure used to store a trie
 *  \author Michele Mastroianni
 *	\mail michele.mastroianni@gmail.com
 *  \version 0.1
 *  \date 25-mar-2009
 *  \see CompactTrie
 *	\note Last upd. 14-apr-2011 by Alessia Amelio, aamelio@deis.unical.it
 */
/*!
A trie is an ordered tree data structure used to efficiently store strings. This
implementation encodes a non-compact trie, where each edge is associated to only one element of a string,
and the string encodes a 2D array suitably linearized from the method called linear.
 */
template<typename T>
class MyTrie {
private:
	///Root of the trie.
	MyTrie* root;
	///The symbol associated to the incoming edge of the current node.
	T val;
	///Pointer to the parent of the current node.
	MyTrie* parent;
	///Pointer of pointer to the children of the current node.
	MyTrie** children;
	///Pointer of pointer to the leafs of the trie (it takes value only on root node).
	MyTrie** leaf;
	///Number of children of the current node.
	int n_children;
	///Number of leafs of the current node (it takes value only on root node).
	int n_leaf;
	///Array containing the occurrences of the 2D array stored from the root to the current node (it takes value only on leafs node).
	/*!
	 * Each occurrence is stored as two consecutive elements of the array, the first one beeing
	 * the row index and the second one being the column index.
	 */
	int* occurence;
	///Size of the array containing the occurrences of the 2D array stored from the root to the current node.
	int n_occurence;
	///Number of rows of the 2D array (it takes value only on leafs node).
	int r;
	///Number of columns of the 2D array (it takes value only on leafs node).
	int c;

	void printPathRic(void);

	int insElementsRic(T* elements, int** ret_occ, int& cur, int size,
			int* args, int args_size, int rows, int cols);

	void linear(T** , int , int , T* , int , T, int , int );


public:

	///Gets the root pointer.
	/*!
	 * \return returns the root pointer.
	 */
	MyTrie* getRoot(){
		return root;
	}

	///Each motif stored into the trie is identified by an id.
	/*!
	 * The mapping between the identifier of motif and the leaf of
	 * MyTrie is stored into the map called pointer_map.
	 */
	map<MyTrie<T>*, int> pointer_map;

	MyTrie(void);
	~MyTrie();

	void navigate(int i);

	static int* mergeOccurence(int* o1, int& size_o1, int* o2, int size_o2);

	///Gets the number of children of the current node.
	/*!
	 * \return returns the number of children.
	 */
	int getNChilder() {
		return n_children;
	}
	///Gets the symbol value of current node.
	/*!
	 * \return returns the value of the current symbol of the motif.
	 */
	T getVal() {
		return val;
	}
	///Sets the symbol value of the current node.
	/*!
	 * \param value symbol value for current node of the motif.
	 */
	void setVal(T value) {
		val = value;
	}
	///Gets the number of motifs in the trie.
	/*!
	 * \return returns the number of motifs into the trie.
	 */
	int getNPaths() {
		return n_leaf;
	}

	int** insElements(T* elements, int size, int* args, int args_size, int& index);
	int** insCore(T** elements, int rows, int cols, int sup, int sx, int b_i,
			int b_j, int c, int d, int& index);
	int** insElements(T** elements, int rows, int cols, int sup, int sx,
			int b_i, int b_j, int& index);
	int** insElements(T** elements, int rows, int cols, int* args,
			int args_size, int& index);
	int find(T** elements, int rows, int cols, T endLine);
	void updateTrie(int sup, int sx, int i, int j, T** autoc, int rows, int cols, T dontcare);
	void printPaths(void);
	T** getMotif(int i);
	T* getLinearMotif(int i);
	int* getOccurence(int i);
	int getNOccurence(int i);
	int getRows(int i);
	int getCols(int i);
	int getLinearSize(int i);

};

///Prints on standard output the elements of the specified motif stored into the trie.
/*!
 * The method prints on standard output all elements for each branch starting at the leaf i up to the root of tree.
 * \param i index of the branch that identifies the motif into the trie.
 */
template<typename T>
void MyTrie<T>::navigate(int i){

	MyTrie* p = root->leaf[i];
	while(p != NULL){
		cout << p->val << ", ";
		p = p->parent;
	}
	cout << endl;
}

///Gets the occurrences vector for a motif stored into the trie.
/*!
 * The method gets the occurrences vector for the motif identified from the branch i.
 * \param i index of the branch that identifies the motif into the trie.
 * \return returns the occurrences vector for a motif identified from the branch i.
 */
template<typename T>
int* MyTrie<T>::getOccurence(int i){
	int* ret = new int[root->leaf[i]->n_occurence];
	for(int k= 0; k< root->leaf[i]->n_occurence; k++)
		ret[k] = root->leaf[i]->occurence[k];
	return ret;
}

///Gets the occurrences vector size for a motif stored into the trie.
/*!
 * The method gets the occurrences vector size for the motif identified from the branch i.
 * \param i index of the branch that identifies the motif into the trie.
 * \return returns the occurrences vector size for a motif identified from the branch i.
 */
template<typename T>
int MyTrie<T>::getNOccurence(int i){
	return root->leaf[i]->n_occurence;
}

///Gets the row size for a motif stored into the trie.
/*!
 * The method gets the row size for the motif identified from the branch i.
 * \param i index of the branch that identifies the motif into the trie.
 * \return returns the column size for a motif identified from the branch i.
 */
template<typename T>
int MyTrie<T>::getRows(int i){
	return root->leaf[i]->r;
}

///Gets the column size for a motif stored into the trie.
/*!
 * The method gets the column size for the motif identified from the branch i.
 * \param i index of the branch that identifies the motif into the trie.
 * \return returns the column size for a motif identified from the branch i.
 */
template<typename T>
int MyTrie<T>::getCols(int i){
	return root->leaf[i]->c;
}

///Gets the linear size for a motif stored into the trie.
/*!
 * The method gets the linear size for the motif identified from the branch i.
 * \param i index of the branch that identifies the motif into the trie.
 * \return returns the linear size for a motif identified from the branch i.
 */
template<typename T>
int MyTrie<T>::getLinearSize(int i){
	return root->leaf[i]->r * (root->leaf[i]->c +1) ;
}

///Returns a linearized motif stored into the trie.
/*!
 * The method returns the linearized motif identified from the branch id.
 * \param i index of the branch that identifies the motif into the trie.
 * \return returns the linearized motif identified from the branch i.
 */
template<typename T>
T* MyTrie<T>::getLinearMotif(int i){
	MyTrie<T>* ti = root->leaf[i];
	int j = 0;
	int rows = ti->r;
	int cols = ti->c;

	T* motif = new T[rows * (cols + 1)];
	MyTrie* p = root->leaf[i];
	while(p != NULL){
		motif[j] = p->val;
		j++;
		if(j == rows * (cols + 1))
			break;
		p = p->parent;
	}

	return motif;
}

///Returns a motif stored into the trie.
/*!
 * The method returns the motif identified from the branch id.
 * \param i index of the branch that identifies the motif into the trie.
 * \returns the motif identified from the branch i.
 */
template<typename T>
T** MyTrie<T>::getMotif(int i){

	MyTrie<T>* ti = root->leaf[i];
	int r = 0;
	int c = 0;
	int rows = ti->r;
	int cols = ti->c;

	T** motif = new T*[rows];
	for(int k = 0; k< rows; k++)
		motif[k] = new T[cols];

	MyTrie* p = root->leaf[i];

	while(p != NULL){
		motif[r][c] = p->val;
		c++;
		if(c == cols){
			c = 0;
			p = p->parent;
			r++;
			if(r == rows)
				break;
		}
		p = p->parent;
	}

	return motif;
}

///Inserts linearized motifs into the trie in reverse order.
/*!
 * Method used for inserting linearized motifs into the trie in reverse order.
 * Note that the reference to the list of occurrence args can't be used after the call of the method.
 * \param elements linearized motif elements.
 * \param size linearized motif size.
 * \param args motif occurrences.
 * \param args_size size of the occurrences array for the motif.
 * \param index mantains the reference to the last motif inserted into the trie.
 *			Index can be used to obtain at any time the inserted motif or its list of occurrences.
 *\return ret_occ[0] = &n_occurence=> number of occurrences.
 *\return ret_occ[1] = occurence	=> pointer to the occurrences.
 *
 */
template<typename T>
int** MyTrie<T>::insElements(T* elements, int size, int* args, int args_size, int& index) {
	int cur = 0;
	int** ret_occ = new int*[2];
	index = insElementsRic(elements, ret_occ, cur, size, args, args_size, 1, size);
	cur = 0;
	return ret_occ;
}

///Linearizes a 2D array into a mono-dimensional array.
/*!
 *The method is used for linearizing 2D array that will be inserted into the trie.
 *\param elements 2D input array.
 *\param rows 2D input array rows.
 *\param cols 2D input array columns.
 *\param ele pointer to the mono-dimensional array result.
 *\param size  mono-dimensional array result size.
 *\param endLine end line symbol used as line separator.
 *\param c for extended use.
 *\param d for extended use.
 *
 */
template<typename T>
void MyTrie<T>::linear(T** elements, int rows, int cols, T* ele, int size, T endLine = -1, int c= 0, int d = 0 ){
	int k = size-1;

	for (int i = (c); i < rows; i++) {
		for (int j = (d); j < cols; j++) {
			ele[k] = elements[i][j];
			k--;
		}
		ele[k] = endLine;
		k--;
	}
}
///Inserts motif cores into the trie.
/*!
 *Method used for inserting motif core into the trie.
 *\param elements motif elements.
 *\param rows motif rows.
 *\param cols motif cols.
 *\param sup first row index occurrence of the core autocorrelation.
 *\param sx first column index occurrence of the core autocorrelation.
 *\param b_i second row index occurrence of the core autocorrelation.
 *\param b_j second column index occurrence of the core autocorrelation.
 *\param c used as row displacement to identify the core of motif that will be inserted into the trie.
 *\param d used as column displacement to identify the core of motif that will be inserted into the trie.
 *\param index mantains the reference to the last motif core inserted into the trie.
 *			Index can be used to obtain at any time the inserted motif or its list of occurrences.
 *\return ret_occ[0] = &n_occurence=> number of occurrences.
 *\return ret_occ[1] = occurence	=> pointer to the occurrences.
 */
/*
 * inserts into Trie the core of elements
 * It doesn't calculate the core, but uses c and d as indexes to identify it
 */
template<typename T>
int** MyTrie<T>::insCore(T** elements, int rows, int cols, int sup, int sx,
		int b_i, int b_j, int c, int d, int & index) {

	int cur = 0;
	int** ret_occ = new int*[2];
	int size = (rows - c) * (cols - d + 1);
	T* ele = new T[size];

	linear(elements, rows, cols, ele, size, c, d);

	int* args = new int[4];
	args[0] = sup + c;
	args[1] = sx + d;
	args[2] = b_i + c;
	args[3] = b_j + d;
	index = insElementsRic(ele, ret_occ, cur, size, args, 4, rows - c, cols - d);
	delete args;
	delete ele;
	cur = 0;
	return ret_occ;
}

///Inserts motifs into the trie.
/*!
 *Method used for inserting motifs into the trie.
 *\param elements motif elements.
 *\param rows motif rows.
 *\param cols motif cols.
 *\param sup first row index occurrence of the autocorrelation that will be inserted into the trie.
 *\param sx first column index occurrence of the autocorrelation that will be inserted into the trie.
 *\param b_i second row index occurrence of the autocorrelation that will be inserted into the trie.
 *\param b_j second column index occurrence of the autocorrelation that will be inserted into the trie.
 *\param index mantains the reference to the last motif inserted into the trie.
 *			Index can be used to obtain at any time the inserted motif or its list of occurrences.
 *\return ret_occ[0] = &n_occurence=> number of occurrences.
 *\return ret_occ[1] = occurence	=> pointer to the occurrences.
 */
template<typename T>
int** MyTrie<T>::insElements(T** elements, int rows, int cols, int sup, int sx,
		int b_i, int b_j, int& index) {
	int cur = 0;
	int** ret_occ = new int*[2];
	int size = rows * (cols + 1);
	T* ele = new T[size];

	linear(elements, rows, cols, ele, size);

	int* args = new int[4];
	args[0] = sup;
	args[1] = sx;
	args[2] = b_i;
	args[3] = b_j;

	index = insElementsRic(ele, ret_occ, cur, size, args, 4, rows, cols);

	delete args;
	delete ele;
	cur = 0;
	return ret_occ;
}

///Private method used for updating motif occurrences into the trie.
/*!
 *The method updates motif occurrences when some motif was inserted into the trie.
 *\param sup first row index occurrence of the autocorrelation that will be inserted into the trie.
 *\param sx first column index occurrence of the autocorrelation that will be inserted into the trie.
 *\param b_i second row index occurrence of the autocorrelation that will be inserted into the trie.
 *\param b_j second column index occurrence of the autocorrelation that will be inserted into the trie.
 *\param auto autocorrelation rappresenting the motif.
 *\param rows motif rows numbers.
 *\param cols motif column numbers.
 *\param dontcare don't care symbol.
 */
template<typename T>
void MyTrie<T>::updateTrie(int sup, int sx, int b_i, int b_j, T** autoc, int rows, int cols, T dontcare){

	if(root == NULL)
		return;

	for(int i = 0; i< root->n_leaf; i++){
		bool update = true;
		MyTrie<T>* ti = root->leaf[i];
//si devono considerare anche i sotto motivi
		if(ti->r > rows || ti->c > cols){
			//per prima cosa verifico le dimensioni del motif
			continue;
		}


		int r = 0;
		int c = 0;

		MyTrie* p = root->leaf[i];

		while(p != NULL){
			if((autoc[r][c] == p->val) || (p->val == dontcare)){

			}else{
				update = false;
				break;
			}

			c++;
			if(c == ti->c){
				c = 0;
				r++;

				if(r == ti->r)
					break;
//fine-linea
				p = p->parent;
			}

			p = p->parent;

		}

		if(update){
			int args_size = 4;
			int* args = new int[args_size];

			args[0] = sup;
			args[2] = b_i;
			args[1] = sx;
			args[3] = b_j;
			int* tmp = mergeOccurence(root->leaf[i]->occurence, root->leaf[i]->n_occurence, args, args_size);
			int* toded = root->leaf[i]->occurence;
			root->leaf[i]->occurence = tmp;

			delete toded;
		}
	}
}

///Finds, if exists, a motif into the trie.
/*!
 * The method finds the motif (if the motif exists) into the trie and returns the index of
 * the branch on the trie associated to the finding motif. Note that if the motif doesn't exist
 * the method returns -1.
 * \param elements motif elements.
 * \param rows motif rows.
 * \param cols motif cols.
 * \param endLine end line symbol used to linearize the 2D motif stored into the trie.
 * \return returns the branch that identifies the motif into the trie.
 */
template<typename T>
int MyTrie<T>::find(T** elements, int rows, int cols, T endLine = -1){

	bool found = true;
	if(root == NULL)
		return -1;


	for(int i = 0; i< root->n_leaf; i++){
		found = true;
		MyTrie<T>* ti = root->leaf[i];

		int r = 0;
		int c = 0;

		if(ti->r > rows || ti->c > cols){
			//per prima cosa verifico le dimensioni del motif
			continue;
		}

		MyTrie* p = root->leaf[i];
		while(p != NULL){
			if(elements[r][c] != p->val)
			{
				found = false;
				break;
			}

			p = p->parent;
			c++;
			if(c == cols){
				c = 0;
				r++;
				p = p->parent;
			}
			if(r == rows)
				break;
		}

		if(found)
			return i;
	}
	return -1;

}

///Inserts motifs into the trie.
/*!
 * Method used for inserting motifs into the trie.
 * Note that the reference to the list of occurrence args can't be used after calling method.
 * \param elements motif elements.
 * \param rows motif rows.
 * \param cols motif cols.
 * \param args motif occurrences.
 * \param args_size number of the motif occurrences.
 * \param index mantains the reference to the last motif inserted into the trie.
 * 			Index can be used to obtain at any time the inserted motif or its list of occurrences.
 * \return ret_occ[0] = &n_occurence=> number of occurrences.
 * \return ret_occ[1] = occurence	=> pointer to the occurrences.
 */
template<typename T>
int** MyTrie<T>::insElements(T** elements, int rows, int cols, int* args,
		int args_size, int& index) {
	int cur = 0;
	int** ret_occ = new int*[2];
	int size = rows * (cols + 1);
	T* ele = new T[size];

	linear(elements, rows, cols, ele, size);

	index = insElementsRic(ele, ret_occ, cur, size, args, args_size, rows, cols);
	delete ele;
	delete args;
	cur = 0;
	return ret_occ;
}

//Given two pairs of indices p1=(i1,j1) and p2=(i2,j2), this method returns 0 if p1<p2, 1 if p1=p2, 2 if p1>p2
int comparison(int i1, int j1, int i2, int j2){

if(i1<i2)
	return 0;
if(i1==i2)
	if(j1<j2)
		return 0;
	else
		if(j1==j2) return 1;
		else return 2;
else return 2;

}

///Returns a merged occurrences vector.
/*!
 * Method used for merging occurrences between two input vectors.
 * \param o1 first occurrences input vector.
 * \param size_o1 first occurrences input vector size. At the end, it mantains the returned merged vector size.
 * \param o2 second occurrences input vector.
 * \param size_o2 second occurrences input vector size.
 * \return merged occurrence vector.
 */
template<typename T>
int* MyTrie<T>::mergeOccurence(int* o1, int& size_o1, int* o2, int size_o2) {

		int i1,j1;
		i1=0,j1=0;

		int count=0;
		int ris=0;
		while (i1<(size_o1-1) && j1<(size_o2-1)){
			ris=comparison(o1[i1],o1[i1+1],o2[j1],o2[j1+1]);

			if(ris==2) {//p1>p2
				count+=2;
				i1+=2;}
			else{
		   		if(ris==0){//p1<p2
			   		count+=2;
			   		j1+=2;}
				else {
					count+=2;
					i1+=2;j1+=2;}
				}
			}

		while(i1<(size_o1-1)){
			count+=2;
			i1+=2;
			}
		while(j1<(size_o2-1)){count+=2;
		j1+=2;
		}

int* u=new int[count];

	int i,j,k;
	i=0,j=0,k=0;

	while (i<(size_o1-1) && j<(size_o2-1)){
			ris=comparison(o1[i],o1[i+1],o2[j],o2[j+1]);
		if (ris==2) {
			u[k]=o1[i];
			u[k+1]=o1[i+1];
			i+=2;k+=2;}
		else{
	   		if (ris==0){
		   		u[k]=o2[j];
		   		u[k+1]=o2[j+1];
		   		k+=2;j+=2;}
			else {
				u[k]=o2[j];
				u[k+1]=o2[j+1];
				i+=2;j+=2;k+=2;}
			}
		}

	while(i<(size_o1-1)){
		u[k]=o1[i];
		u[k+1]=o1[i+1];
		k+=2;i+=2;
		}
	while(j<(size_o2-1)){
		u[k]=o2[j];
		u[k+1]=o2[j+1];
		k+=2;j+=2;
	}

size_o1=count;
return u;
}


///Private methods used for recursively fill in trie with elements motifs.
/*!
 * \param elements elements motif to insert in the trie.
 * \param ret_occ used for the recursive mechanism implementation.
 * \param cur reference to the current element to insert in the trie.
 * \param size number of elements of the motif.
 * \param args motif array occurrences.
 * \param args_size motif array occurrences dimension.
 * \param rows motif rows.
 * \param cols motif cols.
 */
template<typename T>
int MyTrie<T>::insElementsRic(T* elements, int** ret_occ, int& cur, int size,
		int* args, int args_size, int rows, int cols) {

	if (cur == size) {

		//se cur è uguale a size ho consumato tutti gli elements
		//aggiorno le foglie della trie

		if (n_occurence == -1) {
			n_occurence = args_size;
			occurence = new int[args_size];
			for (int i = 0; i < args_size; i++)
				occurence[i] = args[i];
			ret_occ[0] = &n_occurence;
			ret_occ[1] = occurence;

		} else {
			//nota: se faccio il merge quì, nn ho usato il core
			int* tmp = mergeOccurence(occurence, n_occurence, args, args_size);
			int* toded = occurence;
			occurence = tmp;
			delete toded;
			// non devo aggiungere una nuova foglia faccio solo update delle occorrenze
			ret_occ[0] = &n_occurence;
			ret_occ[1] = occurence;


			int pt = -1;
			pt = root->pointer_map[this];
			return pt;
		}

		r = rows;
		c = cols;

		MyTrie<T>** tmp = root->leaf;
		MyTrie<T>** new_t = new MyTrie*[root->n_leaf + 1];
		for (int i = 0; i < root->n_leaf; i++) {
			new_t[i] = root->leaf[i];
		}

		delete tmp;

		root->leaf = new_t;
		root->leaf[root->n_leaf] = this;
		int ret_i = root->n_leaf;
		root->n_leaf++;

		root->pointer_map[this] = ret_i;
		return ret_i;
	}

	//cerco tra i figli (se ci sono) se c'è un match
	for (int i = 0; i < n_children && cur < size; i++) {
		//se c'è il match incremeto il puntatore all'elemento corrente e
		// chiamo ricorsivamente la procedura
		if (children[i]->getVal() == elements[cur]) {
			cur++;
			int ret_i = children[i]->insElementsRic(elements, ret_occ, cur, size, args,
					args_size, rows, cols);
			return ret_i;
		}
		//se nn c'è un match provo con uno degli altri figli
	}
	//se cur è minore di size nn ho consumato tutti gli elements
	if (cur < size) {
		//l'elemento non è presente nella trie lo devo creare ed aggiungere
		if (cur == 0 && root == NULL) {
			//inizializzo la trie con i puntatori alla root e alle foglie
			root = this;
		}
		MyTrie<T>* mt = new MyTrie<T> ;
		mt->setVal(elements[cur]);
		mt->parent = this;
		mt->root = root;
		MyTrie<T>** tmp_c = children;
		MyTrie<T>** new_t = new MyTrie<T>*[n_children + 1];
		for (int i = 0; i < n_children; i++) {
			new_t[i] = children[i];
		}
		delete tmp_c;
		children = new_t;

		children[n_children] = mt;
		n_children++;
		cur++;
		int ret_i = mt->insElementsRic(elements, ret_occ, cur, size, args, args_size, rows,
				cols);

		return ret_i;
	}

	//se ho previsto tutti i casi quì non ci dovrei arrivare
	return -1;
}


///Private method used to navigate recursively trie for printing motifs on standard output.
template<typename T>
void MyTrie<T>::printPathRic(void) {
	cout << val;
	if (parent->parent != NULL) {
		cout << ", ";
		parent->printPathRic();
	}
}

///Print on standard output all motifs stored in the trie.
template<typename T>
void MyTrie<T>::printPaths(void) {

	for (int i = 0; i < n_leaf; i++) {
		cout << "Motif " << i << ": {(";
		int j = 0;
		for (j = 0; j < (root->leaf[i]->n_occurence) - 1; j++)
			cout << root->leaf[i]->occurence[j] << " ,";
		cout << root->leaf[i]->occurence[j] << "); [";
		root->leaf[i]->printPathRic();
		cout << "]; " << " size:(";
		cout << "rows: " << root->leaf[i]->r << " ,";
		cout << "cols: " << root->leaf[i]->c << " ";
		cout << "); n_occ:(" << (root->leaf[i]->n_occurence) / 2 << ")}"
				<< endl;
	}
	cout << "n_elem: " << getNPaths() << endl;
}

///Constructor
template<typename T>
MyTrie<T>::MyTrie(void) {
	root = NULL;
	val = 0;
	parent = NULL;
	children = NULL;
	n_children = 0;
	leaf = NULL;
	n_leaf = 0;
	occurence = NULL;
	n_occurence = -1;

}

///Destructor.
template<typename T>
MyTrie<T>::~MyTrie() {

	if (root == NULL) {
		return;
	}

	if (root->leaf != NULL) {
		for (int i = 0; i < root->n_leaf; i++) {
			if (root->leaf[i] != NULL) {
				if (root->leaf[i]->occurence != NULL) {
					delete root->leaf[i]->occurence;
					root->leaf[i]->occurence = NULL;
					root->leaf[i]->n_occurence = -1;
				}
			}
		}
		delete root->leaf;
		root->leaf = NULL;
		root->n_leaf = 0;
	}

	if (children != NULL) {
		for (int i = 0; i < n_children; i++) {
			delete children[i];
		}
		delete children;
		children = NULL;
		n_children = 0;
	}
	leaf = NULL;
	n_leaf = 0;
	occurence = NULL;
	n_occurence = -1;
	val = 0;
	parent = NULL;

}

#endif /* MYTRIE_H_ */
