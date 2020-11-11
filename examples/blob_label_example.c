/*
 * blob_label_example.c
 *
 *
 *
 */

#include "blob_detect_methods.h"
#include "print_to_csv.h"

int main() {
	img_buf imgin;

	//read in a JPEG and convert to RGB

	//convert to greyscale

	label_image_blobs(imgin); 	   // this overwrites the input image
	print_greyscale_to_csv(imgin); // creates a csv file that shows the image rgb values in the same structure, requires fileIO capability
	return 0;
}


