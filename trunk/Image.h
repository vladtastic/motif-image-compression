#ifndef IMAGE_H
#define IMAGE_H

class Image
{
	private:

	public:
		<vector<vector<int>>> p;
		unsigned int WIDTH;
		unsigned int HEIGHT;

		Image();
		Image( int* , struct bitmap_core* );
		~Image();
		void Read(char*);
		Image Bite(unsigned int, unsigned int);
};


#endif
