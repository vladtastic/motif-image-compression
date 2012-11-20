//Function declarations for Motif class 
#include "Motif.h"

//Constructor
Motif::Motif()
{}

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

//Comparison to remove duplicates
bool Motif::operator==(const Motif& other)
{}
