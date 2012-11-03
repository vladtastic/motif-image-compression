class Image
{
	private:

	public:
		<vector<vector<int>>> r;
		<vector<vector<int>>> g;
		<vector<vector<int>>> b; 
		unsigned int WIDTH;
		unsigned int HEIGHT;

		Image();
		~Image();
		void Read(char*);
		Image Bite(unsigned int, unsigned int);
};
