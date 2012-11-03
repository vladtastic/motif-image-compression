#include "Image.h"
#include <vector>
#include <utility>

using namespace std;

typedef pair<unsigned int, unsigned int> Coordinate;

class Motif
{
	private:
		vector<Coordinate> Occurrences;

		int** r;
		int** g;
		int** b;

		unsigned int HEIGHT;
		unsigned int WIDTH;
	public:
		Motif();
		~Motif();
		bool operator==(const Motif&);
};
