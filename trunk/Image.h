class Image
{
	private:

	public:
		<vector<vector<int>>> p;
		unsigned int WIDTH;
		unsigned int HEIGHT;

		Image();
		~Image();
		void Read(char*);
		Image Bite(unsigned int, unsigned int);
};
