class Image
{
	private:
		unsigned int** r;
		unsigned int** g;
		unsigned int** b;

		unsigned int WIDTH;
		unsigned int HEIGHT;
	public:
		Image();
		~Image();
		void Read(char*);
		Image Bite(unsigned int, unsigned int);
};
