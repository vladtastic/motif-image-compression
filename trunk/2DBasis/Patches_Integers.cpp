#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
using namespace std;
int** imgFvalue;
int **image;
int **tempImage;
int rows;
int cols;
int number=0;
vector<int> dictindexes;
class temp
{
        public:
        int a;
};
class temp1
{
        public:
        int total;
        int index;
};
class match
{
        public:
        string rep;
        int index;
};

vector<match> matchvec;

class patches
{
        public:
        int freq;
        int rowsize;
        int colsize;
        int **array;
        vector<int> rowvec;
        vector<int> colvec;

 patches(int m, int n)
        {
                rowsize=m;
                colsize=n;
                freq=1;
                array = new int*[rowsize];
                for(int i = 0; i < rowsize; i++)
                    array[i] = new int[colsize];
        }
};
class tuple
{
        public:
        int fvalue;
        int k;
};
int FvalueCalculate(int rows,int cols, int i, int j)
{
        int count=0;
        for(int x=0;x<rows;x++)
        {
                for(int y=0;y<cols;y++)
                {
                        if(imgFvalue[i][j]==1)
                        {
                                count++;
                        }
                }
        }
        return count;
}
void imgValueSet(int i,int j, int rows, int cols)
{
        for(int x=i;x<i+rows;x++)
        {
                for(int y=j;y<j+cols;y++)
                {
                        imgFvalue[x][y]=0;
                        tempImage[x][y]=1;
                }
        }
}
void tempImageReset()
{
        for(int x=0;x<rows;x++)
        {
                for(int y=0;y<cols;y++)
                {
                        tempImage[x][y]=0;
                }
        }
}

void tempImageSet(int rows,int cols, int i, int j)
{
        for(int x=i;x<i+rows;x++)
        {
                for(int y=j;y<j+cols;y++)
                {
                        tempImage[x][y]=1;
                }
        }
}
void printTemp()
{
        for(int x=0;x<rows;x++)
        {
                for(int y=0;y<cols;y++)
                {
                        cout<<tempImage[x][y]<<" ";
                }
                cout<<"\n";
        }
}
int findexists(int index)
{
        for(int i=0;i<dictindexes.size();i++)
        {
                if(dictindexes[i] == index)
                        return -1;
        }
        return 0;
}
int findinImage(int i,int j, int rowsiz, int colsiz)
{
        int count=0;
        for(int x=0;x<rowsiz;x++)
        {
                for(int y=0;y<colsiz;y++)
                {
                        if(image[x+i][y+j]==0 || image[x+i][y+j]==1)
                        {
                                count++;
                        }
                }
        }
        if(count==rowsiz*colsiz)
                return 0;
        return -1;
}



int main(int argc, char *argv[])
{

        vector<patches> dict;
        vector<int> dict_count;


// Reading the input file
        int x;
        ifstream inFile;

    inFile.open(argv[1]);
    if (!inFile) {
        cout << "Unable to open file";
        return 0; // terminate with error
    }



        inFile>>rows;
        inFile>>cols;
 cout<<"Rows === "<<rows<<"\n";
        cout<<"Columns ==="<<cols<<"\n";

        image = new int*[rows];
        for(int i = 0; i < rows; i++)
            image[i] = new int[cols];

        imgFvalue = new int*[rows];
        for(int i = 0; i < rows; i++)
            imgFvalue[i] = new int[cols];

        tempImage = new int*[rows];
        for(int i = 0; i < rows; i++)
            tempImage[i] = new int[cols];

        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        inFile>>image[i][j];
                        cout << image[i][j]<<" ";
                }
                cout<<"\n";

        }
        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        imgFvalue[i][j]=1;
                        tempImage[i][j]=0;
                }
                cout<<"\n";

        }

        inFile.close();
for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        vector<int> equals;
                        vector<tuple> Fvalues;
                        for(int k=0;k<dict.size();k++)
                        {
                                int row_size=dict[k].rowsize;
                                int col_size=dict[k].colsize;
                                if(i+row_size<=rows && j+col_size<=cols)
                                {
                                        int** checkarray = new int*[row_size];
                                        for(int l = 0; l < row_size; l++)
                                            checkarray[l] = new int[col_size];
                                        for(int x=0; x< row_size; x++) // Get the same array as the dict array
                                        {
                                                for(int y=0;y<col_size;y++)
                                                {
                                                        checkarray[x][y]=image[i+x][j+y];
                                                }
                                        }
                                        //Comparison of the present position array to the dictionary array
                                        int check=0;
                                        for(int x=0; x< row_size; x++) // Get the same array as the dict array
                                        {
                                                for(int y=0;y<col_size;y++)
                                                {
                                                        if(checkarray[x][y]!=dict[k].array[x][y])
                                                        {
                                                                check=1;
                                                        }
                                                }
                                        }
					if(check==0)
                                        {

                                                        dict[k].freq=dict[k].freq+1;
                                                        dict[k].rowvec.push_back(i);
                                                        dict[k].colvec.push_back(j);
                                                        int fvalue=FvalueCalculate(row_size,col_size,i,j);
                                                        tuple *t1 = new tuple();
                                                        t1->k=k;
                                                        t1->fvalue=fvalue;
                                                        Fvalues.push_back(*t1);
                                        }
                                }
                                //Based on Fvalue do the needful
                                // if Fvalue.size()==0 or the inner dict match size ==0 .. add the only cell as the dict patch
                        }
                        if(Fvalues.size()==0)
                        {
                                patches *p1 = new patches(1,1);
                                p1->array[0][0]=image[i][j];
                                dict.push_back(*p1);
                                int size_now=dict.size()-1;
                                dict[size_now].rowvec.push_back(i);
                                dict[size_now].colvec.push_back(j);
                                imgFvalue[i][j]=0;
                                tempImage[i][j]=1;
                        }
                        else
                        {
                                int max=-1;
                                int k=-1;
                                int fval=0;
                                for(int x=0;x<Fvalues.size();x++)
                                {
                                        if(max < Fvalues[x].k)
                                        {
                                                max=Fvalues[x].k;
                                                fval = x;
                                        }
                                }
                                int x=fval;
                                k=Fvalues[x].k;
 patches *p1 = &dict[k];
//                                      patches *p1 = &ptemp;
                                int row_patch = p1->rowsize;
                                int col_patch = p1->colsize;
                                patches *p2 = new patches(row_patch+1,col_patch);
                                patches *p3 = new patches(row_patch,col_patch+1);
                                for(int x=0;x<row_patch;x++)
                                {
                                        for(int y=0;y<col_patch;y++)
                                        {
                                                p2->array[x][y]=p1->array[x][y];
                                                p3->array[x][y]=p1->array[x][y];
                                        }
                                }
                                if(i+row_patch< rows)
                                {
                                        for(int y=0;y<col_patch;y++)
                                        {
                                                p2->array[row_patch][y] = image[i+row_patch][j+y];
                                        }
                                        int flag1=0;
                                        for(int g=0;g<dict.size();g++)
                                        {
                                                int rs = dict[g].rowsize;
                                                int cs = dict[g].colsize;
                                                int count =0;
                                                if(row_patch+1 == rs && col_patch == cs)
                                                {
                                                        for(int x=0;x<row_patch+1;x++)
                                                        {
                                                                for(int y=0;y<col_patch;y++)
                                                                {
                                                                        if(dict[g].array[x][y] == p2->array[x][y])
                                                                        {
                                                                                count++;
                                                                        }
                                                                }
                                                        }
                                                        if(count==rs*cs)
                                                        {
                                                                flag1=1;
                                                                break;
                                                        }
           }
                                        }

                                        if(flag1==0)
                                        {
                                                dict.push_back(*p2);
                                                int gh = dict.size()-1;
                                                dict[gh].freq=1;
                                                dict[gh].rowvec.push_back(i);
                                                dict[gh].colvec.push_back(j);
                                                imgValueSet(i,j,row_patch+1,col_patch);
                                        }
                                }
                                if(j+col_patch<cols)
                                {
                                        for(int y=0;y<row_patch;y++)
                                        {
                                                p3->array[y][col_patch]=image[i+y][j+col_patch];
                                        }
                                        int flag1=0;
                                        for(int g=0;g<dict.size();g++)
                                        {
                                                int rs = dict[g].rowsize;
                                                int cs = dict[g].colsize;
                                                int count=0;
                                                if(row_patch == rs && col_patch+1 == cs)
                                                {
                                                        for(int x=0;x<row_patch;x++)
                                                        {
                                                                for(int y=0;y<col_patch+1;y++)
                                                                {
                                                                        if(dict[g].array[x][y] == p3->array[x][y])
                                                                        {
                                                                                count++;
                                                                        }
                                                                }
                                                        }
                                                        if(count==rs*cs)
                                                        {
                                                                flag1=1;
                                                                break;
                                                        }
                                                }

  }
                                        if(flag1==0)
                                        {
                                                dict.push_back(*p3);
                                                int gh = dict.size()-1;
                                                dict[gh].freq=1;
                                                dict[gh].rowvec.push_back(i);
                                                dict[gh].colvec.push_back(j);
                                                imgValueSet(i,j,row_patch,col_patch+1);
                                        }
                                }
                        }
                }
        }
        tempImageReset();

// Computing the Maximum Compression without pointers for each of the patch

        for(int i=0;i<dict.size();i++)
        {
                patches *p1 = &dict[i];
                for(int j=0;j<p1->rowsize;j++)
                {
                        for(int k=0;k< p1->colsize; k++)
                        {
                                cout<<p1->array[j][k]<<" ";
                        }
                        cout<<"\n";
                }
                cout<<"The frequency of the pattern is "<<p1->freq;
                cout<<"\n";
                int totalpatch = p1->rowsize * p1->colsize;
                totalpatch = totalpatch * p1->freq;
                int total = rows*cols;
                total = total - totalpatch + p1->rowsize * p1->colsize;
                cout<<"The number of bytes used to reprensent the data other than the pointers is "<< total<<"\n\n";
        }

// Computing the Maximum Compression with pointers for each of the patch.
// Here the pointers are replaced by the special symbols and the
// information about the pointers is given else where


//Producing a sample compressed file with only one compression

//Calculate the patch with the maximum Gain

        vector<temp1> item;
        for(int i=0;i<dict.size();i++)
        {
                temp1 *t1= new temp1;
                patches *p1 = &dict[i];
                int totalpatch = p1->rowsize * p1->colsize;
                totalpatch = totalpatch * p1->freq;
                int total = rows*cols;
                total = total - totalpatch + p1->rowsize * p1->colsize + (p1->freq-1)*1;
                t1->total=total;
                t1->index=i;
                item.push_back(*t1);
        }
        int min = rows*cols;
        int index;
        for(int i=0;i<item.size();i++)
        {
                for(int j =i;j<item.size();j++)
                {
                        if(item[i].total  >  item[j].total)
                        {
                                temp1 mem = item[i];
                                item[i]=item[j];
                                item[j]=mem;
                        }
                }
        }
        //Maximum patch is as follows

        cout<<"The following patch produces the maximum compression \n";

        cout<<"Now the size of the image is as follows \n";

        cout<<item[0].index<<"\n";
        cout<<item[0].total<<"\n";
        // Compressed file produced from the patch


  for(int l=0;l<item.size();l++)
        {
                index = item[l].index;
                for(int k=0;k<dict[index].rowvec.size();k++)
                {
                        cout<<l<<"\n";
                        //                                      int check = findinImage(i,j,dict[index].rowsize,dict[index].colsize);
                        int i = dict[index].rowvec[k];
                        int j = dict[index].colvec[k];
                        int tempcount=0;

                        cout<<"Inside\n";
                        int rsize = dict[index].rowsize;
                        int csize = dict[index].colsize;
                        for(int x=0;x<rsize;x++)
                        {
                                for(int y=0;y<csize;y++)
                                {
                                        if(tempImage[x+i][y+j]!=0)
                                        {
                                                tempcount=1;
                                        }
                                }
                        }
                        if(tempcount==0)
                        {
                                for(int x=i;x< i+rsize;x++)
                                {
                                        for(int y=j;y<j+csize;y++)
                                        {
                                                image[x][y] = -1;
                                        }
                                }
//                              if(rsize*csize > 1)
//                              {
                                        image[i][j]=(index+2)*-1;
//                              }
//                              else
//                                      image[i][j] =d;
                                tempImageSet(rsize,csize,i,j);

                        }
     }
        }
        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        cout<<image[i][j]<<" ";
                }
                cout<<"\n";
        }
        cout<<"\n";
        cout<<"\n";
        //After Compression the image looks like this

        ofstream myfile1;
        myfile1.open (argv[2]);



        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        tempImage[i][j]=0;
                }
        }

        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {

                        if(image[i][j]!=-1)
                        {
                                int index = -1*image[i][j]-2;
                                for(int k=0;k<dict[index].rowsize;k++)
                                {
                                        for(int l=0;l<dict[index].colsize;l++)
                                        {
                                                tempImage[i+k][j+l]=dict[index].array[k][l];
                                        }
                                }

                        }

		 }
        }
	for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {

                       if(image[i][j]!=-1)
                        {
                                int index = -1*image[i][j]-2;
                                int k = findexists(index);
                                if(k==0)
                                {
                                        match *m1= new match();
                                        m1->index=index;
                        /*              string temp = "x";
                                        int tempnum =number;
                                        string temp2="";
                                        while(tempnum > 0)
                                        {
                                                temp2=temp2+char(tempnum%10 + '0');
                                                tempnum=tempnum/10;
                                        }
                                        for(int f = temp2.size()-1; f >=0;f--)
                                        {
                                                temp = temp+temp2[f];
                                        }
                                        m1->rep=temp;
                                        number++;
                                        myfile1<<temp<<"\n";*/
                                        dictindexes.push_back(index);
                                        myfile1<<index<<"\n";
                                        myfile1<<dict[index].rowsize<<" ";
                                        myfile1<<dict[index].colsize<<"\n";
                                        for(int k=0;k<dict[index].rowsize;k++)
                                        {
                                                for(int l=0;l<dict[index].colsize;l++)
                                                {
                                                        myfile1<<dict[index].array[k][l]<<" ";
                                                        tempImage[i+k][j+l]=dict[index].array[k][l];
                                                }
                                //              myfile1<<"\n";
                                        }
                                        matchvec.push_back(*m1);
                                }
                        }

		  }
        }
        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        if(image[i][j]!=-1)
                        {
                                cout<<image[i][j]<<" ";
                                myfile1<<-1*image[i][j]<<" ";
                        }
                }
                myfile1<<"\n";
                cout<<"\n";
        }
        for(int i=0;i<rows;i++)
        {
                for(int j=0;j<cols;j++)
                {
                        cout<<tempImage[i][j]<<" ";
//                      myfile1<<tempImage[i][j]<<" ";
                }
//              myfile1<<"\n";
                cout<<"\n";
        }
        myfile1.close();

        return 0;
}

