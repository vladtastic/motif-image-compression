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


#ifndef COMPACTTRIE_H_
#define COMPACTTRIE_H_

#include <cstdlib>
#include <map>
#include <utility>

using namespace std;

template<typename T>
class CompactTrie; //Forward declaration

/*! \class NodeTrie
 *  \brief Abstraction of node concept.
 *  \author Michele Mastroianni
 *  \mail michele.mastroianni@gmail.com
 *  \version 0.1
 *  \date    25-mar-2009
 *  \see CompactTrie
 */
/*!
 *  The CompactTrie class uses a support inner class as abstraction of node concept.
 */
template<typename T>
class NodeTrie {
	public:
		///Pair containing linearized motif symbols and motif occurrence.
		/*!
		 * Each occurrence is stored as two consecutive elements of the array, the first one beeing
		 * the row index and the second one being the column index.
		 */
		pair<T*, int*>* value;
		///Number of symbols of the motif.
		int n_value;
		///Size of the array containing the occurrences of the 2D array stored from the root to the current node.
		int n_occurence;
		///Pointer to the parent of the current node.
		NodeTrie* parent;
		///Pointer of pointer to the children of the current node.
		NodeTrie** children;
		///Number of children of the current node.
		int n_children;
		///Identifier of the current node.
		int id;

		~NodeTrie();
		NodeTrie(int n_node);
		NodeTrie(int n_node, pair<T*, int*>* value, int n_val, int n_occ);
		int insValueRic(T* val, int n_value, int* occ, int n_occ,
				int& offset, CompactTrie<T>* ct, T dontcare, bool try_to_ins, bool debug);
		int insRic(T* val, int n_val, int* occ, int n_occ,
				int& cur, CompactTrie<T>* ct , T dontcare, NodeTrie<T>* node);
		int tryToInsRic(T* val, int n_val, int* occ, int n_occ,
						int& cur, CompactTrie<T>* ct , T dontcare, NodeTrie<T>* node);
	};

/*! \class CompactTrie
 *  \brief Ordered data structure used to store a patricia trie
 *  \author Michele Mastroianni
 *	\mail michele.mastroianni@gmail.com
 *  \version 0.1
 *  \date    25-mar-2009
 *  \see NodeTrie
 *  \see MyTrie
 *	\note Last upd. 14-apr-2011 by Alessia Amelio, aamelio@deis.unical.it
 */
/*!
 * It is a specialized set data structure based on the trie that is used to store a set of strings.
 * In contrast with a regular trie, the edges of a CompactTrie are labelled with sequences of characters rather than
 * with single characters. These can be strings of characters, bit strings such as integers, or generally arbitrary
 * sequences of objects in lexicographical order.
 */
template<typename T>
class CompactTrie {

private:
	///Number of nodes
	int n_node;

public:
	///Root of the trie.
	NodeTrie<T>* root;
	///Mapping between node identifier and nodeTrie pointer.
	map<int, NodeTrie<T>*> node_map;
	///Mapping between nodeTrie pointer and node identifier.
	map<NodeTrie<T>*, int> pointer_map;
	///Mapping between node identifier on a CompactTrie and node identifier on a MyTrie.
	///\see MyTrie
	map<int, int> my_trie_compact_trie;

	~CompactTrie();
	CompactTrie();
	CompactTrie(MyTrie<T>* mt, T dontcare);
	CompactTrie(MyTrie<T>* mt, set<int> s,  T dontcare);

	bool contain(int* occurence, int n_occurence, int* occ, int n_occ);

	int insValue(T* val, int n_value, int* occ, int n_occ, T dontcare, bool debug);

	int tryToInsValue(T* val, int n_value, int* occ, int n_occ, T dontcare);

	void getValues();
	///Gets Number of nodes stored on trie.
	/*!
	 * \return number of nodes stored on trie.
	 */
	int getNNode(){
		return n_node;
	}
	///Generates identifiers for new nodes.
	/*!
	 * \return the identifier.
	 */
	int nNode(){
		int ret = n_node;
		n_node++;
		return ret;
	}
	///Gets the pointer to the root node.
	/*!
	 * \return root node.
	 * \see NodeTrie
	 */
	NodeTrie<T>* getRoot(){
		return root;
	}

};

///Constructor.
/*!
 * constructs an empty NodeTrie from node id.
 * \param n_node identifier of node.
 */
template<typename T>
NodeTrie<T>::NodeTrie(int n_node){

	id = n_node;
	value = new pair<T*, int*>(NULL, NULL);
	n_value = 0;
	n_occurence = 0;
	parent = NULL;
	children = NULL;
	n_children = 0;

}

///Constructor.
/*!
 * Constructs NodeTrie from node id, motif symbols and motif occurrence.
 * \param n_node identifier of node.
 * \param val contains motif symbols and motif occurrence.
 * \param n_val number of symbols of motif in val.
 * \param n_occ size of occurrence array in val.
 */
template<typename T>
NodeTrie<T>::NodeTrie(int n_node, pair<T*, int*>* val, int n_val, int n_occ){

	id = n_node;
	value = val;
	n_value = n_val;
	n_occurence = n_occ;
	parent = NULL;
	children = NULL;
	n_children = 0;

}


template<typename T>
int NodeTrie<T>::tryToInsRic(T* val, int n_val, int* occ, int n_occ,
		int& cur, CompactTrie<T>* ct , T dontcare, NodeTrie<T>* node){

	bool debug = false;

	if(debug){
		cout << "n_val: " << n_val << ", cur: " << cur << endl;
	}

	bool trovato = false;
	//un figlio che ha il primo carattere uguale al primo carattere del pezzettino di motivo passato

	int i_child = 0;
	for(int i = 0; (i< node->n_children) && !trovato; i++){
		if(debug){
			cout << "node->children[i]->value->first[0] = " << node->children[i]->value->first[0] <<endl;
			cout << "val[cur] = " << val[cur] << endl;
		}
		if(node->children[i]->value->first[0] == val[cur]){

			if(cur==(n_val-1))
			   if(ct->contain(node->children[i]->value->second, node->children[i]->n_occurence, occ, n_occ))
					return -1;
					else return 1;
			trovato = true;
			i_child = i;
		}else{
			if(val[cur] == dontcare && node->children[i]->value->first[0] != dontcare){
				if(ct->contain(node->children[i]->value->second, node->children[i]->n_occurence, occ, n_occ))
					return -1;
				else if(cur==(n_val-1)) return 1;


			}else{

				//return 1;

			}
		}
	}

	if(trovato){
		if(debug){
			cout << "found common prefix: ";
			for(int g = 0; g <= i_child; g++)
				cout << "(" << val[cur + g] << ")";
			cout << endl;
		}
		int min_size = n_val - cur;

		if(node->children[i_child]->n_value < min_size)
			min_size = node->children[i_child]->n_value;

		if(min_size == n_val - cur){
			bool equal = true;
			int node_branch = -1;

			for(int j = 0; (j< min_size) && equal; j++){
				if(val[cur +j] != node->children[i_child]->value->first[j]){
					equal = false;
					node_branch = j;
					if(debug){
						cout << "first mismatch (" << val[cur +j] << ") in position " << node_branch << endl;
					}
				}
			}

			if(equal){

				 return 1;

			}else{

				if(val[cur + node_branch] == dontcare && node->children[i_child]->value->first[node_branch] != dontcare){
					if(ct->contain(node->children[i_child]->value->second, node->children[i_child]->n_occurence, occ, n_occ)){
						return -1;
					}else return 1;
				}else{

					//return 1;

				}
			}

		}else{//è più grande l'altro
			if(debug){
				cout << "the other is greater" << endl;
			}
			bool equal = true;
			int node_branch = -1;
			for(int x = 0; (x < node->children[i_child]->n_value && equal); x++){
				if(val[cur + x] != node->children[i_child]->value->first[x]){
					equal = false;
					node_branch = x;
				}
			}
			if(debug){
				cout << "node_branch: " << node_branch << endl;
			}
			if(equal){
				cur = cur + node->children[i_child]->n_value;

				tryToInsRic(val, n_val, occ, n_occ, cur, ct, dontcare, node->children[i_child]);
			}else{
				if(val[cur + node_branch] == dontcare && node->children[i_child]->value->first[node_branch] != dontcare){
					if(ct->contain(node->children[i_child]->value->second, node->children[i_child]->n_occurence, occ, n_occ)){

						return -1;
					}
				}else{
					return 1;
				}
			}
		}
	}else{

		return 1;

	}

	return -1;
}


template<typename T>
int NodeTrie<T>::insRic(T* val, int n_val, int* occ, int n_occ,
		int& cur, CompactTrie<T>* ct , T dontcare, NodeTrie<T>* node){

	bool debug = false;

	if(debug){
		cout << "n_val " << n_val << ", cur " << cur << endl;
	}

	bool trovato = false;
	//un figlio che ha il primo carattere uguale al primo carattere del pezzettino di motivo passato

	int i_child = 0;
	for(int i = 0; (i< node->n_children) && !trovato; i++){
		if(debug){
			cout << "node->children[i]->value->first[0] = " << node->children[i]->value->first[0] <<endl;
			cout << "val[cur] = " << val[cur] << endl;
		}
		if(node->children[i]->value->first[0] == val[cur]){
			trovato = true;
			i_child = i;
		}
	}

	if(trovato){
		if(debug){
			cout << "found common prefix: ";
			for(int g = 0; g <= i_child; g++)
				cout << "(" << val[cur + g] << ")";
			cout << endl;
		}
		int min_size = n_val - cur;
		if(node->children[i_child]->n_value < min_size)
			min_size = node->children[i_child]->n_value;

		if(min_size == n_val - cur){
			bool equal = true;
			int node_branch = -1;

			for(int j = 0; (j< min_size) && equal; j++){
				if(val[cur +j] != node->children[i_child]->value->first[j]){
					equal = false;
					node_branch = j;
					if(debug){
						cout << "first mismatch (" << val[cur +j] << ") in position " << node_branch << endl;
					}
				}
			}

			if(equal){
				if(debug){
					cout << "no branches. Update only the occurrences for: ";
					for(int jj = 0; jj< min_size; jj++){
						cout << val[cur +jj] << " ";
					}
					cout << endl;
				}
				if((n_val - cur == node->children[i_child]->n_value)){
					int _n_occ = node->children[i_child]->n_occurence;
					int* union_occ = MyTrie<T>::mergeOccurence(node->children[i_child]->value->second, _n_occ, occ, n_occ);
					pair<T*, int*>* pp = new pair<T*, int*>(node->children[i_child]->value->first, union_occ);

					delete node->children[i_child]->value;
					node->children[i_child]->value = pp;
					node->children[i_child]->n_occurence = _n_occ;

					return ct->pointer_map[node->children[i_child]];
				}else{

				}

			}else{
				if(debug){
					cout << "temp0: " ;
				}
				T* temp0 = new T[node_branch];
				int size_temp1 = node->children[i_child]->n_value - cur - node_branch;
				T* temp1 = NULL;
				if(size_temp1 > 0)
					temp1 = new T[size_temp1];
				for(int x = 0; x < node->children[i_child]->n_value - cur; x++){
					if(x < node_branch){
						temp0[x] = node->children[i_child]->value->first[x];
						if(debug){
							cout << temp0[x] << " ";
						}
					}else{
						temp1[x - node_branch] = node->children[i_child]->value->first[x];
					}
				}
				if(debug){
					cout << endl << "temp1: ";
					for(int jj = 0; jj < size_temp1; jj++ )
						cout << temp1[jj] << " ";
					cout << endl << "temp2: " ;
				}
				T* temp2 = new T[(n_val - cur - node_branch)];
				for(int x = 0; x < (n_val - cur - node_branch); x++){
					temp2[x] = val[x + cur + node_branch];
					if(debug){
						cout << temp2[x] << " ";
					}
				}
				if(debug){
					cout << endl;
				}
				pair<T*, int*>* oldpartchild = NULL;
				if(size_temp1 > 0)
					oldpartchild = new pair<T*, int*>(temp1, node->children[i_child]->value->second);
				pair<T*, int*>* newchild = new pair<T*, int*>(temp2, occ);

				int _n_occ = node->children[i_child]->n_occurence;
				int union_n_occ = _n_occ;
				int* union_occ = MyTrie<T>::mergeOccurence(node->children[i_child]->value->second,
						union_n_occ, occ, n_occ);

				pair<T*, int*>* pp = new pair<T*, int*>(temp0, union_occ);
				delete node->children[i_child]->value;
				node->children[i_child]->value = pp;
				node->children[i_child]->n_value = node_branch;
				node->children[i_child]->n_occurence = union_n_occ;

				NodeTrie<T>* newnode1 = NULL;
				if(size_temp1 > 0){
					newnode1 = new NodeTrie(ct->nNode(), oldpartchild, size_temp1, _n_occ);
					ct->node_map[ct->getNNode()] = newnode1;
					ct->pointer_map[newnode1] = ct->getNNode();
					newnode1->parent = node->children[i_child];
				}

				NodeTrie<T>* newnode2 = new NodeTrie(ct->nNode(), newchild,	n_val - cur - node_branch, n_occ);
				ct->node_map[ct->getNNode()] = newnode2;
				ct->pointer_map[newnode2] = ct->getNNode();
				newnode2->parent = node->children[i_child];

				int incr = 1;
				if(size_temp1 > 0)
					incr++;

				NodeTrie<T>** newchildren = new NodeTrie<T>*[node->children[i_child]->n_children + incr];
				int h = 0;
				for(; h < node->children[i_child]->n_children; h++)
					newchildren[h] = node->children[i_child]->children[h];
				newchildren[h++] = newnode2;
				if(size_temp1 > 0)
					newchildren[h] = newnode1;
				delete node->children[i_child]->children;
				node->children[i_child]->children = newchildren;
				node->children[i_child]->n_children = node->children[i_child]->n_children + incr;

				return ct->pointer_map[node->children[i_child]];

			}
		}else{//è più grande l'altro
			if(debug){
				cout << "the other is greater" << endl;
			}
			bool equal = true;
			int node_branch = -1;
			for(int x = 0; (x < node->children[i_child]->n_value && equal); x++){
				if(val[cur + x] != node->children[i_child]->value->first[x]){
					equal = false;
					node_branch = x;
				}
			}
			if(debug){
				cout << "node_branch: " << node_branch << endl;
			}
			if(equal){
				cur = cur + node->children[i_child]->n_value;
				int _n_occ = node->children[i_child]->n_occurence;
				int* union_occ = MyTrie<T>::mergeOccurence(node->children[i_child]->value->second,	_n_occ, occ, n_occ);
				delete node->children[i_child]->value->second;
				node->children[i_child]->value->second = union_occ;
				node->children[i_child]->n_occurence = _n_occ;
				insRic(val, n_val, occ, n_occ, cur, ct, dontcare, node->children[i_child]);
			}else{

				T* temp1 = new T[node_branch];
				for(int x = 0; x < node_branch; x++){
					temp1[x] = node->children[i_child]->value->first[x];
				}
				T* temp2 = new T[node->children[i_child]->n_value - node_branch];
				for(int x = 0; x < node->children[i_child]->n_value - node_branch; x++){
					temp2[x] = node->children[i_child]->value->first[x + node_branch ];
				}

				pair<T*, int*>* granchild = new pair<T*, int*>(temp2, node->children[i_child]->value->second);

				int _n_occ = node->children[i_child]->n_occurence;

				int* union_occ = MyTrie<T>::mergeOccurence(node->children[i_child]->value->second,
						_n_occ, occ, n_occ);
				pair<T*, int*>* pp = new pair<T*, int*>(temp1, union_occ);

				NodeTrie<T>* newnode = new NodeTrie(ct->nNode(), pp,
						(n_val -cur), _n_occ);
				ct->node_map[ct->getNNode()] = newnode;
				ct->pointer_map[newnode] = ct->getNNode();
				newnode->children = new NodeTrie<T>*[2];
				newnode->children[0] = node->children[i_child];

				node->children[i_child] = newnode;
				newnode->parent = node;

				delete node->children[i_child]->value;
				node->children[i_child]->value = granchild;

				T* temp3 = new T[n_val - node_branch];
				for(int x = 0; x < (n_val - node_branch); x++){
					temp3[x] = val[x + node_branch ];
				}

				pair<T*, int*>* p3 = new pair<T*, int*>(temp3, occ);
				NodeTrie<T>* newnodep3 = new NodeTrie(ct->nNode(), p3, n_val - node_branch, n_occ);
				ct->node_map[ct->getNNode()] = newnodep3;
				ct->pointer_map[newnodep3] = ct->getNNode();
				newnode->children[1] = newnodep3;
				newnodep3->parent = newnode;

				return ct->pointer_map[newnodep3];
			}
		}
		return ct->pointer_map[node->children[i_child]];
	}else{
		if(debug){
		 cout << "adding the child into another branch" << endl;
		 cout << " n_val: " << n_val << " cur: " << cur << endl;
		}
		T* temp1 = new T[n_val - cur];
		for(int x = 0; x < n_val - cur; x++){
			temp1[x] = val[cur + x];
		}
		pair<T*, int*>* pp = new pair<T*, int*>(temp1, occ);
		NodeTrie<T>* newnode = new NodeTrie(ct->nNode(), pp, (n_val -cur), n_occ);
		ct->node_map[ct->getNNode()] = newnode;
		ct->pointer_map[newnode] = ct->getNNode();
		newnode->parent = node;

		NodeTrie<T>** newchildren = new NodeTrie<T>*[node->n_children + 1];
		for(int x = 0; x < node->n_children; x++){
			newchildren[x] = node->children[x];
		}
		newchildren[node->n_children] = newnode;
		delete node->children;
		node->children = newchildren;
		node->n_children = node->n_children + 1;

		return ct->pointer_map[newnode];
	}
}


///Destructor.
template<typename T>
NodeTrie<T>::~NodeTrie(){
	if(value != NULL){
		delete value;
		value = NULL;
	}else{
		cout << "warning destructor NodeTrie " << endl;
	}

}

///Destructor.
template<typename T>
CompactTrie<T>::~CompactTrie(){

	if (root == NULL) {
		cout << "warning destructor CompactTrie " << endl;
		return;
	}
	if (root->children != NULL) {
		for (int i = 0; i < root->n_children; i++) {
			delete root->children[i];
		}
		delete root->children;
		root->children = NULL;
		root->n_children = 0;
	}
	delete root;
	root = NULL;

}

///Constructor.
/*!
 * Default constructor for empty CompactTrie.
 */
template<typename T>
CompactTrie<T>::CompactTrie(){
	n_node = 0;
	root =  new NodeTrie<T>(n_node);
	n_node++;
}

///Constructor.
/*!
 * Copies constructor to create CompactTrie by motifs in MyTrie.
 * \param mt source trie.
 * \param s set of motifs in MyTrie to be copied in the CompactTrie.
 * \param dontcare don't care symbol.
 */
template<typename T>
CompactTrie<T>::CompactTrie(MyTrie<T>* mt, set<int> s, T dontcare){

	bool debug = false;

	int setSize = 0;

	n_node = 0;
	root =  new NodeTrie<T>(n_node);

	n_node++;

	for(set<int>::iterator it = s.begin(); it != s.end(); it++){
		setSize++;
		T* motif = mt->getLinearMotif(*it);
		int size = mt->getLinearSize(*it);
		int* occ = mt->getOccurence(*it);
		int n_occ = mt->getNOccurence(*it);

		T* motif_clone = new T[size];
		int* occ_clone = new int[n_occ];

		for(int imc = 0; imc < size; imc++)
			motif_clone[imc] = motif[imc];

		for(int io = 0; io < n_occ; io++)
			occ_clone[io] = occ[io];

		bool debug = false;

		int id = insValue(motif_clone, size, occ_clone, n_occ, dontcare, debug);
		my_trie_compact_trie[*it] = id;
	}

	if(debug){
		cout << "CompactTrie<T>::CompactTrie(MyTrie<T>* mt, set<int> s, T dontcare) # number of novel motifs: " << setSize << endl;
	}
}

///Constructor.
/*!
 * Copies constructor to create CompactTrie by MyTrie.
 * \param mt source trie.
 * \param dontcare don't care symbol.
 */
template<typename T>
CompactTrie<T>::CompactTrie(MyTrie<T>* mt, T dontcare){
	n_node = 0;
	root =  new NodeTrie<T>(n_node);

	n_node++;

	for(int i = 0; i< mt->getNPaths(); i++){
		T* motif = mt->getLinearMotif(i);
		int size = mt->getLinearSize(i);
		int* occ = mt->getOccurence(i);
		int n_occ = mt->getNOccurence(i);

		T* motif_clone = new T[size];
		int* occ_clone = new int[n_occ];

		for(int imc = 0; imc < size; imc++)
			motif_clone[imc] = motif[imc];

		for(int io = 0; io < n_occ; io++)
			occ_clone[io] = occ[io];

		int id = insValue(motif_clone, size, occ_clone, n_occ, dontcare);
		my_trie_compact_trie[i] = id;
	}
}

///Prints on standard output motifs stored in the trie.
template<typename T>
void CompactTrie<T>::getValues(){
	typename map<int, NodeTrie<T>*>::iterator it = node_map.begin();

	for(; it != node_map.end(); it++){
		NodeTrie<T>* p = (*it).second;
		cout<<"entrato"<<endl;
		if((p != NULL) && p->parent != NULL){
			for(int i = 0; i< p->n_value; i++)
				cout << p->value->first[i] << ", ";
			cout << endl;
			for(int i = 0; i< p->n_occurence; i++)
				cout << p->value->second[i] << ", ";
			cout << endl<<endl;
		}
	}
}

///Tries to insert a motif in the trie.
/*!
 * Tries to insert the val motif into the compact trie.
 * As soon as there is a dont' care into the val motif corresponding
 * to a solid character into the trie, the current node is taken and it is verified
 * if a mismatch occurs. If the occ list is contained into the list of the curr node,
 * val has to be eliminated (without inserting it into the trie), otherwise
 * val has to be inserted.
 * \param val symbols of motif to insert in the trie.
 * \param n_val number of symbols of motif.
 * \param occ vector of occurrence of motif.
 * \param n_occ size of vector of occurrence.
 * \param cur reference to the CompactTrie.
 * \param dontcare don't care symbol.
 * \return returns the id for the inserted motif.
 */
template<typename T>
int CompactTrie<T>::tryToInsValue(T* val, int n_value, int* occ, int n_occ, T dontcare){

	bool debug = false;

	if(debug){
		cout << "CompactTrie<T>::tryToInsValue, T* val: ";
		for(int v = 0; v < n_value; v++)
			cout << "(" << val[v] << ") ";
		cout << endl;
	}
	if(root->n_children == 0){// è la prima volta che inserisco nella trie
		pair<T*, int*>* p = new pair<T*, int*>(val, occ);
		NodeTrie<T>* nn =  new NodeTrie<T>(n_node, p, n_value, n_occ);
		nn->parent = root;
		root->children = new NodeTrie<T>*[1];
		root->n_children = 1;
		root->children[0] = nn;
		node_map[n_node] = nn;
		pointer_map[nn] = n_node;
		int ret = n_node;
		n_node++;
		return ret;
	}
	else{
		int cur = 0;

		if(root->tryToInsRic(val, n_value, occ, n_occ, cur, this, dontcare, root) == 1)
		{
			return root->insRic(val, n_value, occ, n_occ, cur, this, dontcare, root);
		}else
			return -1;

	}
}

///Checkes if a list of occurrence can be contained in an other.
/*!
 * Checkes if the occurrences of a list of occurrence is contained an other list of occurrence.
 * \param u_occurence first list of occurrence.
 * \param u_n_occurence size of vector u_occurrence.
 * \param m_occ second list of occurrence.
 * \param m_n_occ size of vector m_occ.
 * \return return true if the occurrences of the first list are contained in the second one, false otherwise.
 */

template<typename T>
bool CompactTrie<T>::contain(int* u_occurence, int u_n_occurence, int* m_occ, int m_n_occ){

int conta=0;
	for(int i = 0; i< m_n_occ-1; i+=2){
		bool flag=true;
		for(int j = 0; j< u_n_occurence-1 && flag; j+=2){
			if(m_occ[i]==u_occurence[j]){
				if(m_occ[i+1]==u_occurence[j+1]){
					conta++; flag=false;}
				}

				}
			}

			if(conta>=(m_n_occ/2)) {return true;}
				return false;
}


///Inserts a motif in the trie.
/*!
 * Used to insert motifs in the trie.
 * \param val symbols of motif.
 * \param n_value number of elements in val array.
 * \param occ vector of occurrences of the motif.
 */
template<typename T>
int CompactTrie<T>::insValue(T* val, int n_value, int* occ, int n_occ, T dontcare, bool debug = false){

	if(debug){
		cout << "CompactTrie<T>::insValue, T* val: ";
		for(int v = 0; v < n_value; v++)
			cout << "(" << val[v] << ") ";
		cout << endl;
	}

	if(root->n_children == 0){// è la prima volta che inserisco nella trie
		pair<T*, int*>* p = new pair<T*, int*>(val, occ);
		NodeTrie<T>* nn =  new NodeTrie<T>(n_node, p, n_value, n_occ);
		nn->parent = root;
		root->children = new NodeTrie<T>*[1];
		root->n_children = 1;
		root->children[0] = nn;
		node_map[n_node] = nn;
		pointer_map[nn] = n_node;
		int ret = n_node;
		n_node++;
		return ret;
	}
	else{
		int cur = 0;
		return root->insRic(val, n_value, occ, n_occ, cur, this, dontcare, root);
	}

	if(debug){
		cout << "return CompactTrie<T>::insValue" << endl;
	}
}

/*void testTrie(){
	cout<< "testtrie " << endl;
	MyTrie<char> t = MyTrie<char>();
	char* m1 = "CBA";
	int id_m1 = 0;
	int* occ_m1 = new int[2];
	occ_m1[0] = 0;
	occ_m1[1] = 1;
	char* m2 = "CCA";
	int id_m2 = 0;
	int* occ_m2 = new int[2];
	occ_m2[0] = 0;
	occ_m2[1] = 2;
	char* m3 = "AD";
	int id_m3 = 0;
	int* occ_m3 = new int[2];
	occ_m3[0] = 0;
	occ_m3[1] = 3;
	char* m4 = "ACD";
	int id_m4 = 0;
	int* occ_m4 = new int[2];
	occ_m4[0] = 0;
	occ_m4[1] = 4;
	t.insElements(m1, 3, occ_m1, 2, id_m1);
	t.insElements(m2, 3, occ_m2, 2, id_m2);
	t.printPaths();

	CompactTrie<char> ct = CompactTrie<char>(&t, '.');
	cout << "--------------" << endl;
	ct.getValues();
	cout << "--------------" << endl;

	ct.insValue(m3, 2, occ_m3, 2, '.', true);
	ct.getValues();
	ct.insValue(m4, 3, occ_m4, 2, '.', true);
	ct.getValues();

}*/

///Used to insert Node into the CompactTrie.
/*!
 * Inserts recursively motif into a NodeTrie of CompactTrie.
 * \param val symbols of the motif to insert in the trie.
 * \param n_val number of symbols of the motif.
 * \param occ vector of occurrence of the motif.
 * \param n_occ size of vector of occurrence.
 * \param cur reference to the CompactTrie.
 * \param dontcare don't care symbol.
 * \param try_to_ins if it is true, it means that the function is used for testing the redundancy of the motif.
 */
/**/
template<typename T>
int NodeTrie<T>::insValueRic(T* val, int n_val, int* occ, int n_occ,
		int& cur, CompactTrie<T>* ct , T dontcare, bool try_to_ins = false, bool debug = false){

	if(cur == n_val){//ho consumato tutti i simboli
		if(debug)
			cout << "consumed all symbols" << endl;
		int* union_occ = MyTrie<T>::mergeOccurence(value->second, n_occurence, occ, n_occ);
		pair<T*, int*>* pp = new pair<T*, int*>(value->first, union_occ);

		delete value;
		value = pp;
		return ct->pointer_map[this];
	}
//nn ho consumato tutt i simboli, ma sul nodo sono finiti
	//devo andare ricorsivamente sui figli del nodo corrente
	if(debug)
		cout << "going recursively on the curr node's children" << endl;
	for(int q = 0; q < n_children; q++){
		bool almeno_uno = false;
//verifico quanti elementi del nodo corrente hanno un match con
		//il nodo che voglio inserire (T* val)
		int i = 0;
		for(; i < children[q]->n_value; i++){
			if(try_to_ins){

				if(i < n_val && val[i] == dontcare && children[q]->value->first[i] != dontcare){

					if(ct->contain(value->second, n_occurence, occ, n_occ)){
						return -1;
					}
					else{
						if(debug){
							cout << "match found... " << val[i] << endl;
							cout << "this: " << this << endl;
						}
						almeno_uno = true;
						//incremento il contatore sul numero dei match
						cur++;
						continue;
					}
				}else{

					 if(children[q]->value->first[i] == dontcare){
						 if(debug){
							 cout << "match found... " << val[i] << endl;
							 cout << "this: " << this << endl;
						 }
						 almeno_uno = true;
						 //incremento il contatore sul numero dei match
						 cur++;
						 continue;
					 }

					continue;
				}
			}

			if(children[q]->value->first[i] == val[i]){
				if(debug){
					cout << "match found... " << val[i] << endl;
					cout << "this: " << this << endl;
				}
				almeno_uno = true;
				//incremento il contatore sul numero dei match
				cur++;
			}else{
				break;
			}
		}


		if(almeno_uno){
//a questo punto è finito il prefisso comune
			//spezzo il nodo e gli aggiungo i 2 suffissi che non hanno in comune
			//parte comune
			T* prefix = new T[i];
			if(debug){
				cout << "split the node" << endl;
				cout << "prefix: ";
			}
			for(int x = 0; x < i; x++){
				prefix[x] = children[q]->value->first[x];
				if(debug)
					cout << prefix[x] << " ";
			}
//primo branch
			if(debug)
				cout << endl << "brach_1: ";
			int n_val_b1 = children[q]->n_value - i;
			NodeTrie<T>* br1 = NULL;
			if(n_val_b1 > 0){
				T* branch_1 = new T[n_val_b1];
				for(int x = 0; x < n_val_b1; x++){
					branch_1[x] = children[q]->value->first[x + i];
					if(debug)
						cout << branch_1[x] << " ";
				}
				int branch_1_n_occ = children[q]->n_occurence;
				int* branch_1_occ = new int[branch_1_n_occ];
				for(int v = 0; v < branch_1_n_occ; v++){
					branch_1_occ[v] = children[q]->value->second[v];
				}
				pair<T*, int*>* p1 = new pair<T*, int*>(branch_1, branch_1_occ);
				br1 = new NodeTrie(ct->nNode(), p1, n_val_b1, branch_1_n_occ);
				br1->parent = this;
				ct->node_map[ct->getNNode()] = br1;
				ct->pointer_map[br1] = ct->getNNode();
			}
//secondo branch
			if(debug)
				cout << endl << "brach_2: ";
			int n_val_b2 = n_val - i;
			NodeTrie<T>* br2 = NULL;
			if(n_val_b2 > 0){
				T* branch_2 = new T[n_val_b2];
				for(int x = 0; x < n_val_b2; x++){
					branch_2[x] = val[x + i];
					if(debug)
						cout << branch_2[x] << " ";
//stò consumando i simboli rimanenti
					cur++;
				}
				if(debug)
					cout << endl;
				pair<T*, int*>* p2 = new pair<T*, int*>(branch_2, occ);
				br2 = new NodeTrie(ct->nNode(), p2, n_val_b2, n_occ);
				br2->parent = this;
				ct->node_map[ct->getNNode()] = br2;
				ct->pointer_map[br2] = ct->getNNode();

			}
			//aggiorna nodo con la parte comune e l'unione delle occorrenze
			int* union_occ = MyTrie<T>::mergeOccurence(children[q]->value->second, children[q]->n_occurence, occ, n_occ);
			pair<T*, int*>* nuova = new pair<T*, int*>(prefix, union_occ);

			delete children[q]->value;

			children[q]->value = nuova;
			children[q]->n_value = i;

			int incr = 0;
			if(br1 != NULL)
				incr++;
			if(br2 != NULL)
				incr++;

			NodeTrie<T>** chi = new NodeTrie<T>*[children[q]->n_children + incr];
			for(int r = 0; r < children[q]->n_children; r++)
				chi[r] = chi[r];
			if(br2 != NULL)
				chi[children[q]->n_children] = br2;
			if(br1 != NULL)
				chi[children[q]->n_children+1] = br1;

			children[q]->n_children = n_children + incr;

			children[q]->children = chi;

			if(br2 != NULL){
				return br2->insValueRic(val, n_val, occ, n_occ, cur, ct, dontcare, try_to_ins, debug);
			}
			if(br1 != NULL){
				return br1->insValueRic(val, n_val, occ, n_occ, cur, ct, dontcare, try_to_ins, debug);
			}
		}


	}

	if(cur == 0){
// non c'è proprio il prefisso, aggiungo un nuovo nodo alla root
		if(debug)
			cout << "prefix doesn't exist, adding a new node to the root" << endl;
		pair<T*, int*>* p = new pair<T*, int*>(val, occ);

		NodeTrie<T>* nn =  new NodeTrie<T>(ct->nNode(), p, n_val, n_occ);
		nn->parent = this;
		NodeTrie<T>** chi = new NodeTrie<T>*[n_children + 1];
		for(int s = 0; s < n_children; s++){
			chi[s] = children[s];
		}

		children = chi;
		children[n_children] = nn;
		n_children = n_children + 1;

		ct->node_map[ct->getNNode()] = nn;
		ct->pointer_map[nn] = ct->getNNode();
		cur = n_val;
		return ct->getNNode();
	}
	return -1;
}
/**/

#endif /* COMPACTTRIE_H_ */
