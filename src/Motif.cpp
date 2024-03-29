//Function declarations for Motif class 
#include "Motif.h"
#include <string>
#include <sstream>



//Constructor
Motif::Motif()
{
	HEIGHT = 0;
	WIDTH = 0;

}

Motif::Motif( unsigned int x, unsigned int y )		// x , y are the WIDTH and HEIGHT of the Motif respectively
{

	if( x > (unsigned int) 0 ){

		WIDTH = x;

	}


	if( y > (unsigned int) 0 ){

		HEIGHT = y;

	}

}
	

//Takes in image I and bite B and performs auto-correlation to obtain meet
void Motif::Meet(const Image& I, const Image& B)
{
	int n_row = B.HEIGHT;
	int n_col = B.WIDTH;

	//Calculate consensus	
	for(int i = 0; i < n_row; i++)
	{
		vector <int> corr;
		for(int j = 0; j < n_col; j++)
		{
			if(I.p[i][j] == B.p[i][j])
				corr.push_back(I.p[i][j]);
			else
				corr.push_back(-1);
		}
		this->p.push_back(corr);
	}

	//Convert to meet
	//Remove rows from bottom
	bool solid = false;
	while (!p.empty())
	{
		vector<int> row = p.back();
		vector<int>::iterator it;
		for(it = row.begin(); it != row.end(); it++)
		{
			if(*it != -1)
			{
				solid = true;
				break;
			}
		}
		if(solid == false)
		{
			p.pop_back();
			n_row--;
		}
		else
		{
			break;
		}
	}
	
	//If p is empty after these removals, we know that the meet is a null set
	if(p.empty())
	{
		this->HEIGHT = 0;
		this->WIDTH = 0;
		return;
	}
	
	//remove non-solid top rows
	solid = false;
	while(!solid)
	{
		vector<int> row = p.front();
		vector<int>::iterator it;
		for(it = row.begin(); it != row.end(); it++)
		{
			if(*it != -1)
			{
				solid = true;
				break;
			}
		}
		if(solid == false)
		{
			p.erase(p.begin());
			n_row--;
		}
	}

	//remove non-empty right columns
	solid = false;
	while(!solid)
	{
		for(int i = 0; i <= n_row; i++)
		{
			if(p[i][n_col-1] != -1)
			{
				solid = true;
				break;
			}
		}
		if(solid == false)
		{
			vector< vector<int> >::iterator it;
			for(it = p.begin(); it != p.end(); it++)
			{
				it->pop_back();
			}
			n_col--;
		}
	}

	//remove non-empty left columns
	solid = false;
	while(!solid)
	{
		for(int i = 0; i <= n_row; i++)
		{
			if(p[i][0] != -1)
			{
				solid = true;
				break;
			}
		}
		if(solid == false)
		{
			vector< vector<int> >::iterator it;
			for(it = p.begin(); it != p.end(); it++)
			{
				it->erase(it->begin());
			}
			n_col--;
		}
	}

	this->HEIGHT = n_row;
	this->WIDTH = n_col;
}

//Destructor
Motif::~Motif()
{}

//Overload equality operator
bool Motif::operator==(const Motif& other)
{

	int arg1_dimH = this->HEIGHT;
	int arg1_dimW = this->WIDTH;
	int arg1_size = arg1_dimW * arg1_dimH;

	int arg2_dimH = other.HEIGHT;
	int arg2_dimW = other.WIDTH;
	int arg2_size = arg2_dimW * arg2_dimH;

	if( arg1_size == arg2_size ){

		// Check value by value
		for( int i = 0; i < arg1_dimH; i++){
			for( int j = 0; j < arg1_dimW; j++ ){

				if( this->p[i][j] != other.p[i][j] ){

					return false;

				}

			}

		}

		return true;
	}

	else{

		return false;

	}
}


//Overload less than operator
bool Motif::operator<(const Motif& other)
{

	unsigned int arg1_dimH = this->HEIGHT;
	unsigned int arg1_dimW = this->WIDTH;
	unsigned int arg1_size = arg1_dimW * arg1_dimH;

	unsigned int arg2_dimH = other.HEIGHT;
	unsigned int arg2_dimW = other.WIDTH;
	unsigned int arg2_size = arg2_dimW * arg2_dimH;

	if( arg1_size < arg2_size ){


		return true;

	}

	if( arg1_size > arg2_size ){

		return false;

	}

	else{
		// Motifs are same size
		// Check value by value
		for( unsigned int i = 0; i < arg1_dimH; i++){
			for( unsigned int j = 0; j < arg1_dimW; j++ ){

				if( this->p[i][j] > other.p[i][j] ){

					return false;

				}

			}

		}


		// arg1 < arg2 OR arg1 == arg2

		bool check;

		check = *(this)==other;

		if( check == true ){

			return false;
		
		}

		else{

			return true;

		}
	}


}


string Motif::string_representation(){

	unsigned int dimW = this->WIDTH;
	unsigned int dimH = this->HEIGHT;

	string temp;
	ostringstream ss(temp);
	string stringrep = "";

	for( unsigned int i = 0; i < dimH; i++ ){

		for( unsigned int j = 0; j < dimW; j++ ){

			ss << this->p[i][j];

			stringrep.append(temp);

			ss.clear();


		}

	}

	return stringrep;

}
