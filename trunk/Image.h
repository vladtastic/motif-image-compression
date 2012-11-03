#include <string>

class Image
{
	private:
		unsigned int** r;
		unsigned int** g;
		unsigned int** b;
	public:
		Image();
		~Image();
		Read(string);
}
