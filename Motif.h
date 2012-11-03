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
		
		Motif(const Image&, const Image&);
		~Motif();
		bool operator==(const Motif&);
};
