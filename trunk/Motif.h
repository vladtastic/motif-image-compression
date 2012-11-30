#ifndef MOTIF_H
#define MOTIF_H

#include "Image.h"
#include <vector>
#include <utility>
#include <string>
#include <sstream>


using namespace std;

struct coords{
	
	unsigned int x;
	unsigned int y;

};

class Motif
{
	public:
		vector<struct coords> occurrences;

		vector< vector<int> > p;

		unsigned int HEIGHT;
		unsigned int WIDTH;
		
		Motif();
		Motif( unsigned int x, unsigned int y );
		~Motif();
		void Meet(const Image&, const Image&);
		bool operator==(const Motif&);
		bool operator<(const Motif&);
		string string_representation();
};
	


#endif
