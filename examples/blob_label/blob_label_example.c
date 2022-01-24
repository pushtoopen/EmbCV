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
 * Its very important to ensure the input calues are one single array
 * as only a uint8_t, meaning values are not floats and only range from 0 to 255
 *
 * scan pattern checks all pixels around the O pixel
 * labeled X before creating a new blob
 *               XXX
 *               XO
 */


//method to print a greyscale picture to csv. allows you to see what the blob detect method is doing.
//not required for base usage
#include "print_to_csv.h"

// standard methods for blob labelling, required to use label_image_blobs
#include "blob_label_methods.h"

// included so we can printf debug
#include <stdio.h>
#include <stdint.h>

//main is required for GCC "hosted" environments, different for different embedded platforms
int main() {
	img_buf imgin; //base struct for an image
	char *filename = "blobs.jpg";
	FILE *pFile;                 /* csv converted source file */
	FILE *fp; // output of shell
	char filein[5000000]; //input csv of greyscale file. 5000000 is a little over 2X the size of characters needed for a full 4K resolution photo (3 chars per pixel and a comma)
	char sizeparams[50];
	char shellcmd[50];
	char *token; // for breaking up csvs
	//read in a JPEG and convert to RGB

	// This runs the python script to read in, convert the picture to greyscale, and write to a csv/text file
	// also catches the python print output which gives us the pixel width, pixel height, and nmber of channels of the picture output to csv.
	sprintf(shellcmd, "python3 ./openimggreyscale.py %s",filename);
  	fp = popen(shellcmd, "r");
  	if (fp == NULL) {
    	printf("Failed to run system python command\n" );
    	exit(1);
  	}

	// reads in print output of the python file to get the characteristics of the picture
	fgets(sizeparams, sizeof(sizeparams), fp); 
	int i=0;
 	char *strArr[3] = {0};
 	char* strSplit = sizeparams;
 	while ((token = strtok_r(strSplit , ",", &strSplit))) {
 	      strArr[i++] = token;
	}
   	fclose(fp);
	imgin.w = atoi(strArr[0]);
	imgin.h = atoi(strArr[1]);
	imgin.c = atoi(strArr[2]);
	uint64_t numvals = imgin.w*imgin.h*imgin.c;
	imgin.data  = calloc(numvals,sizeof(uint8_t));
	// Python file complete


	// This reads in the temporary file made by python and parses it into an array: coverting the string values to integers.
	if ((pFile = fopen("tempcsv.txt", "rb")) == NULL) {
		printf("\n tempcsv.txt from the python script attempted to be loaded and failed to load\n\n");
		exit(0);
	}
	fgets(filein, sizeof(filein), pFile); //reads entire picture into one buffer
	i=0;

   	token = strtok(filein, ",");
 	while( token != NULL ) {
		//(token = strtok_r(strSplit , ",", &strSplit));
		imgin.data[i++] = atoi(token);
		token = strtok(NULL, ",");

		//printf("\n imgval in: %d",imgin.data[i-1]);

	}

	fclose(pFile);
	remove("tempcsv.txt"); // deletes the temporary output from python

	//thresholds, binarizes, then labels
	printf("Labeling image and overwriting the input image\r\n");
	label_image_blobs(imgin); 	   // this overwrites the input image
	printf("Outputting image to a csv file for ease of analysis\r\n");
	print_greyscale_to_csv(imgin); // creates a csv file that shows the image label values in the same structure, requires fileIO capability
	return 0;
}

