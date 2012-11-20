#ifndef MOTIF_H
#define MOTIF_H

#include "Image.h"
#include <vector>
#include <utility>

using namespace std;

typedef pair<unsigned int, unsigned int> Coordinate;

class Motif
{
	public:
		vector<Coordinate> Occurrences;

		vector< vector<int> > p;

		unsigned int HEIGHT;
		unsigned int WIDTH;
		
		Motif();
		~Motif();
		void Meet(const Image&, const Image&);
		bool operator==(const Motif&);
};


#endif
