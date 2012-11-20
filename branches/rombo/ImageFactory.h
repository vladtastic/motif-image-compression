/*==========================================================================
                2D-IME (2D Irredundant Motif Extraction)
                   http://siloe.deis.unical.it/2D-IME
 ============================================================================
  Copyright (C) 2009


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.


  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
*/


#ifndef IMAGEFACTORY_H_
#define IMAGEFACTORY_H_

#include <cstdlib>
using namespace std;

/*! \class ImageFactory
 *  \brief Factory for 2D array random generation on generic alphabets
 *  \author Michele Mastroianni
 *  \mail michele.mastroianni@gmail.com
 *  \version 0.1
 *  \date    25-mar-2009
 */
template<typename T>
class ImageFactory {

private:

	int seed; ///<Seed used for random generation.
	int colors_size; ///<Size of alphabet (colors).
	T* colors; ///<Palette of colors.
	T base; ///<First color.

	///Inizializes the palette of colors.
	void initColors() {
		for (int i = 0; i < colors_size; i++) {
			colors[i] = base;
			base++;
		}
	}
public:

	///Destructor
	~ImageFactory(){
		delete colors;
	}


	///Constructor
	/*!
	 * Creates the specified factor using generics.
	 * \param size_c size of alphabet
	 * \param b first element of alphabet
	 * \param s seed for random generation of alphabet
	 */
	ImageFactory(int size_c, T b, int s) {
		seed = s;
		srand(seed);
		base = b;
		colors_size = size_c;
		colors = new T[colors_size];
		initColors();
	}

	/// Return a random 2D vector.
	/*!
	 * Gets a nxm random 2D vector of generic elements.
	 * \param n first dimension of 2D vector
	 * \param m second dimension of 2D vector
	 */
	T** get_image(int n, int m) {
		T** image = new T*[n];
		int random_integer = 0;
		for (int i = 0; i < n; i++) {
			image[i] = new T[m];
			for (int j = 0; j < m; j++) {
				random_integer = (rand() % colors_size);
				image[i][j] = colors[random_integer];
			}
		}
		return image;
	}

};

///Flip an image bottom up.
/*
 * Flips the input image bottom up.
 * \param image input image
 * \param height number of rows of input image.
 * \param width number of cols of input image.
 */
template<typename T>
void flip_img(T** image, int height, int width) {

	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			T tmp = image[i][j];
			image[i][j] = image[height - i - 1][j];
			image[height - i - 1][j] = tmp;
		}
	}
}

///Print an image on standard output.
/*
 * Prints on standard output the input image.
 * \param image input image
 * \param n number of rows of input image.
 * \param m number of cols of input image.
 */
template<typename T>
void print_image(T** image, int n, int m) {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << "[" << image[i][j] << "] ";
		}
		cout << endl;
	}
}

#endif /* IMAGEFACTORY_H_ */
