/*
 * blob_label_example.c
 *
 * This example reads in a JPEG, coverts to greyscale, thresholds it,
 * binarizes it, and then labels it in that order
 * this is all done within the same memory location.
 * This means the input to the binarization and label methods
 * destroys the picture. This is done on purpose as these methods
 * attempt to use as little amount of memory as possible.
 *
 * The label method scans from the top left to the bottom right of the picture
 * and changes each pixel to a value indicating what blob it belongs to
 * To reduce the memory footprint, the maximum label is 255 and minimum is 2
 * 0 indicates no object and 1 indicates an unlabeled object
 *
 * scan pattern checks all pixels around the O
 * labeled X before creating a new blob
 *               XXX
 *               XO
 */

//basic declarations for using libjpeg. no need if you already have the picture in RGB/greyscale
#include "libjpeg_declarations.h"

//method to print a greyscale picture to csv. allows you to see what the blob detect method is doing.
//not required for base usage
#include "print_to_csv.h"

// standard methods for blob labelling, required to use label_image_blobs
#include "blob_label_methods.h"

// included so we can printf debug
#include <stdio.h>

//main is required for GCC "hosted" environments, different for different embedded platforms
int main() {
	img_buf imgin; //base struct for an image
	struct ImageInfo readimage; //same thing as imgin but libjpeg. not needed if imgin is already populated.

	//read in a JPEG and convert to RGB
	printf("Reading in blobs.jpg");
	readimage = read_JPEG_file_Single_Line_Greyscale("./blobs.jpg");

    imgin.data = readimage.pData;
    imgin.c = readimage.nNumComponent;
    imgin.w = readimage.nWidth;
    imgin.h = readimage.nHeight;
	//convert to greyscale
	//thresholds, binarizes, then labels
	printf("Labelling image and overwriting the input image");
	label_image_blobs(imgin); 	   // this overwrites the input image
	printf("Outputting image to a csv file for ease of analysis");
	print_greyscale_to_csv(imgin); // creates a csv file that shows the image label values in the same structure, requires fileIO capability
	free(readimage.pData);
	return 0;
}

