class Image
{
	private:
		<vector<vector<int>>> r;
		<vector<vector<int>>> g;
		<vector<vector<int>>> b; 
		unsigned int WIDTH;
		unsigned int HEIGHT;
	public:
		Image();
		~Image();
		void Read(char*);
		Image Bite(unsigned int, unsigned int);
};
