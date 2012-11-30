#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <map>
#include <fstream>

#include "Bitmap.h"
#include "Image.h"
#include "Motif.h"

int* load_pixel_matrix( char*, struct bitmap_core* );
//int save_motifs( vector< Motif >* motif_save_list, int index );

int main( int argc, char* argv[] ){

	
	int err = 0;

	
	if( argc < 2 || (argv[1] == NULL ) ){
		
		printf("Usage: executable /path/to/image \n" );
		return 0;

	}
	

	// bmp to Image object

	struct bitmap_core*  core_info = (struct bitmap_core*) malloc( sizeof(struct bitmap_core) );;

		
		
	int* bitmap_ptr = LoadBmp( argv[1], core_info );
	free(bitmap_ptr);

	//	LOADBMP RETURNS INCORRECT PIXEL MATRIX
	//	LOAD_PIXEL_MATRIX USED AS SUBSTITUTION
	//	LOADBMP USED TO FIND IMAGE DIMENSIONS

	int dimR = core_info->biHeight;
	int dimC = core_info->biWidth;

	int* pixel_array  = load_pixel_matrix( "thresholdpixelmatrix.txt" , core_info );
	
	if( pixel_array == NULL ){

		printf("Error loading pixel array!\n");
		return 1;

	}

	Image current_image = Image( pixel_array, core_info );	
	
	int num_bites = dimR * dimC;

	vector< Motif > motif_save_list;
	map< string, vector<struct coords> > motif_hash_list; 

	for( int i = 0; i < dimR; i++ ){

		for( int j = 0; j < dimC; j++ ){

			Image current_bite = current_image.Bite( (unsigned int) j, (unsigned int) i );	// how does this function work?
													// Also, on next loop iter, does it clear mem from prev var?

			Motif current_autoc = Motif();

			current_autoc.Meet( current_image, current_bite ); // this is autocor													//	This should return the AUTOCORRELATION


			//	Hash current_autoc
			//	Add autoc to a list for writing/saving
			
			motif_hash_list[ current_autoc.string_representation() ] = current_autoc.occurrences;
			


		/***************
			
			motif_save_list.push_back( current_autoc );
			
			if( ( i * dimC + j ) % 100 == 0 ){

				// Write data to disk

			}

		****************/
		

		}

	}


	// Decompose any maximal motifs...



	return 0;

}


int* load_pixel_matrix( char* filename, struct bitmap_core* bmpc ){

	FILE *fp;

	fp  = fopen(filename, "r" );

	if( fp == NULL ){

		printf("Error opening pixel matrix file!\n");
		return (int*) NULL;

	}

	int* pixel_array = (int*) malloc( bmpc->biHeight * bmpc->biWidth * sizeof( int) );

	if( pixel_array == NULL ){

		printf("Error Allocating pixel value matrix!\n");
		return (int*) NULL;

	}

	for( int i = 0; i<bmpc->biHeight; i++){
		for( int j=0; j<bmpc->biWidth; j++){

			fscanf(fp, "%d\t", &(pixel_array[ i * bmpc->biWidth + j ]) );

		}

	}

	
	fclose(fp);


	return pixel_array;

}


/*
int save_motifs( vector< Motif >* motif_save_list, int index ){

	string fname = "batch.";
	string batch_number;
	ostringstream ss(batch_number);

	ss << index;

	ss.clear();

	fname.append(batch_number);

	ofstream fp( fname.c_str(), ios::out || ios::binary );

	if( !fp.is_open() ){

		return 1;

	}

	if( fp.is_open() ){
	
		while( motif_save_list->empty() != true ){

			Motif temp_motif = motif_save_list.pop_back();
			unsigned int dimW = temp_motif.WIDTH;
			unsigned int dimH = temp_motif.HEIGHT:
			unsigned int numOccur = temp_motif.occurrences.size();

			fp << "#D";
			fp << temp_motif.WIDTH << "\t" << temp_motif.HEIGHT << "\n";
		
		
			fp << "#P" << "\n";
		
			for( unsigned int i = 0; i < dimH; i++ ){

				for( unsigned int j = 0; j < dimW; j++ ){

					fp << temp_motif.p[i][j] << " ";

				}

			}

		
			fp << "#O" << "\n";

			for( unsigned int i = 0; i < numOccur; i++ ){

				fp << temp_motif.occurrences[i] << " ";

			}

			fp << "\n";

		}

	}

	fp.close();		

	return 0;

}
*/
