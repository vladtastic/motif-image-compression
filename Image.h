#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <utility>

using namespace std;

class Image
{
	private:

	public:
		vector< vector<int> > p;
		unsigned int WIDTH;
		unsigned int HEIGHT;

		Image();
		Image( unsigned int, unsigned int );
		Image( int* , struct bitmap_core* );
		~Image();
		void Read(char*);
		Image Bite(unsigned int, unsigned int);
};


#endif
