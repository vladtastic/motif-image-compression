//Function declarations for Motif class 
#include "Motif.h"

//Constructor which takes in the main image I and bite B and performs auto-correlation
Motif::Motif(Image& I, Image& B)
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
		for(it = row.begin(); it != row.end(); i++)
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
		}
		else
		{
			break;
		}
	}
	
	//If p is not empty after these removals, we know that the meet is not a null set
	if(p.empty())
	{
		this->HEIGHT = 0;
		this->WIDTH = 0;
		return;
	}
	

	
}

//Destructor
Motif::~Motif()
{}

//Comparison to remove duplicates
bool Motif::operator==(const Motif& other)
{}
