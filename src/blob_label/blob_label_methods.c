/**
 * @file blob_detect_methods.c
 *
 *
 *
*/

#include "blob_label_methods.h"

//declared here as these are not used outside of blob detection.
uint8_t minpixelval(struct blob_struct *stdvals);
uint8_t checkpixel(struct blob_struct* stdvals );
void nonzerovals(struct blob_struct *stdvals);
void connect_binary_blobs(img_buf imgin, struct blob_pos* blobpos );

void label_image_blobs(img_buf imgin)
{
	uint8_t threshold = threshold_image(imgin);
	binarize_image(imgin, threshold);
	connect_binary_blobs(imgin,&blobpos);
}


uint8_t minpixelval(struct blob_struct *stdvals)
{
	uint8_t out;
	out = stdvals->curval;
	uint8_t i = 0;
	if (stdvals->checklen != 0)
	{
		out = stdvals->checkvals[0];

		for(i = 1; i < stdvals->checklen; i++ )
		{
			if(stdvals->checkvals[i] < out)
			{
				out = stdvals->checkvals[i];
			}
		}
	}
	return out;
}

void nonzerovals(struct blob_struct *stdvals)
{
	// all ints should be able to be converted to uint8_t's
	uint8_t popcnt = 0;
	uint8_t i = 0;
	uint8_t interstore[5] = {0}; // 5 is the max amount of checkvals expected     XXX
	for (i = 0; i < stdvals->checklen; i++) //                                    XO
	{
		if (stdvals->checkvals[i] != 0)
		{
			interstore[popcnt] = stdvals->checkvals[i];
			popcnt++;
		}
	}
	if(popcnt > 0)
	{

		stdvals->checkvals = realloc(stdvals->checkvals,sizeof(uint8_t)*popcnt);
		for (i = 0; i < popcnt; i++)
		{
			stdvals->checkvals[i] = interstore[i];
		}
	}

		stdvals->checklen = popcnt;
}

uint8_t checkpixel(struct blob_struct* stdvals )
{

	uint8_t outval = stdvals->curval;
	uint8_t k = 0;
	for (k = 0; k < stdvals->checklen; k++)
	{
		if (stdvals->checkvals[k] < 2)
		{
			stdvals->checkvals[k] = 0;
		}
	}
	nonzerovals(stdvals);
	uint8_t minval = minpixelval(stdvals);

	if (minval < 2)
	{
		stdvals->blobcnt = 	stdvals->blobcnt + 1;
		outval = outval + stdvals->blobcnt;
	}
	else
	{
		// denote minimum blob vals as minval found
		for (k = 0; k < stdvals->checklen; k++)
		{
			stdvals->checkarray[stdvals->checkvals[k]] = minval;
		}
		outval = minval;
	}
	stdvals->curval = outval;
	return stdvals->curval;
}

void connect_binary_blobs(img_buf imgin, struct blob_pos* blobpos )
{
	struct blob_struct stdvals = {.checkvals = calloc(1,sizeof(uint8_t)), .checklen = 1, .curval = 0,.checkarray = {0}, .blobcnt = 0};
	stdvals.checkvals = NULL;
	stdvals.blobcnt = 0;
	uint32_t maxrows = imgin.h; //uint32_t
	uint32_t maxcols = imgin.w; //uint32_t
	uint8_t minouterval;
	uint32_t i = 0; //uint32 value used for several for loops
	uint32_t y = 0; //pixel rows : top to bottom (row 1 is 2nd from top)
	uint32_t x = 0; // pixel columns : left to right (col 1 is 2nd from left)
	uint16_t e = 0; //could be uint8_t but im lazy. Should never go over 255 except to end;
	uint8_t prevval = 0;
	int16_t u = 0;
	uint8_t left = 0;
	uint8_t upleft = 0;
	uint8_t up = 0;
	uint8_t upright = 0;
	uint8_t curpixel = 0;
	//setup blob relationship array
	for (e = 0; e < 256; e++) //sets checkarray to 0:255
	{
		stdvals.checkarray[e] = e;
	}



	uint32_t maxi = calcarrayidx(maxcols-1,maxrows-1,maxcols);
	// (0 = unpop unchecked, 1 = unpop checked, and 2 = pop unlabeled, > 2 = pop labeled) What this is doing is guaranteeing that the input image is binarized.
	
	// this runs through everything and ensures its either 0 or 1. it GUARANTEES a binary image.
	for (i = 0; i < maxrows*maxcols -1; i++) //-1 kuz we're indexed at 0
	{
		if (imgin.data[i] > 0)
		{
			imgin.data[i] = imgin.data[i]/imgin.data[i]; // all populated binary places are set to 1.
			printf("current modified imgin: %d, i=%d, y=%d, x=%d \n",imgin.data[i],i,calcypos(i, maxcols),calcxpos(i,calcypos(i, maxcols),maxcols));
		}
	}


	//-------------------------------------------------------
	// begin checking/modifying pixels
	//remember, this assumes imgin is binarized.

	// top-left pixel
	stdvals.curval = imgin.data[0];
	
	if (stdvals.curval == 1)
	{
		stdvals.checklen = 1; //checking one pixel to see if it's populated, creates a new blob if it is
		stdvals.checkvals = realloc(stdvals.checkvals, 1*sizeof(uint8_t));
		stdvals.checkvals[0] = 0;
		imgin.data[0] = checkpixel(&stdvals);
	}
	// top-right pixel
	stdvals.curval = imgin.data[maxcols-1];
	if (stdvals.curval == 1)
	{
		stdvals.checklen = 1; //checking one pixel to see if it's populated, creates a new blob if it is
		stdvals.checkvals = realloc(stdvals.checkvals, 1*sizeof(uint8_t));
		stdvals.checkvals[0] = 0;
		imgin.data[maxcols-1] = checkpixel(&stdvals);
	}

	// bot-left pixel
	stdvals.curval = imgin.data[calcarrayidx(0,maxrows-1,maxcols)];
	if (stdvals.curval == 1)
	{
		stdvals.checklen = 1; //checking one pixel to see if it's populated, creates a new blob if it is
		stdvals.checkvals = realloc(stdvals.checkvals, 1*sizeof(uint8_t));
		stdvals.checkvals[0] = 0;
		imgin.data[calcarrayidx(0,maxrows-1,maxcols)] = checkpixel(&stdvals);
	}

	// bot-right pixel
	stdvals.curval = imgin.data[calcarrayidx(maxcols-1,maxrows-1,maxcols)];// x & y are 0 index based
	if (stdvals.curval == 1)
	{
		stdvals.checklen = 1; //checking one pixel to see if it's populated, creates a new blob if it is
		stdvals.checkvals = realloc(stdvals.checkvals, 1*sizeof(uint8_t));
		stdvals.checkvals[0] = 0;
		imgin.data[calcarrayidx(maxcols-1,maxrows-1,maxcols)] = checkpixel(&stdvals);
	}

	//top row - Checks pixels to the left only, starts at 2nd pixel in the picture from the top left going right
	y = 0;
	for(x = 1; x < maxcols; x++ )
	{
		i = calcarrayidx(x,y,maxcols);
		stdvals.curval = imgin.data[i];
		//pre-checked column, no need on individual column/row checks
		//blank space
		if (stdvals.curval == 0)
		{
			continue;
		}
		// visited populated space
		else if(stdvals.curval > 1)
		{
			continue;
		}

		// found new blob. begin processing
		else if (stdvals.curval == 1)
		{
			stdvals.checklen = 1;
			left = imgin.data[calcarrayidx(x-1,y,maxcols)];
			stdvals.checkvals = realloc(stdvals.checkvals,stdvals.checklen*sizeof(uint8_t));
			stdvals.checkvals[0] = left;
			imgin.data[i] = checkpixel(&stdvals);
		}
	}

	//left col - Checks pixels above and to the right only, starts at 1st pixel in 2nd row of picture going down from the top
	x = 0;
	for(y = 1; y < maxrows; y++ )
	{
		i = calcarrayidx(x,y,maxcols);
		stdvals.curval = imgin.data[i];
		//pre-checked column, no need on individual column/row checks
		//blank space
		if (stdvals.curval == 0)
		{
			continue;
		}
		// visited populated space
		else if(stdvals.curval > 1)
		{
			continue;
		}

		// found new blob. begin processing
		else if (stdvals.curval == 1)
		{
			stdvals.checklen = 2;
			up = imgin.data[calcarrayidx(x,y-1,maxcols)];
			upright = imgin.data[calcarrayidx(x+1,y-1,maxcols)];
			stdvals.checkvals = realloc(stdvals.checkvals,stdvals.checklen*sizeof(uint8_t));
			stdvals.checkvals[0] = up; stdvals.checkvals[1] = upright;
			imgin.data[i] = checkpixel(&stdvals);
		}
	}

	//right col - Checks above, to the upleft, and left only, starts at last pixel on the 2nd row going down from the top 
	x = (maxcols-1);
	for(y = 1; y < maxrows; y++ )
	{
		i = calcarrayidx(x,y,maxcols);
		stdvals.curval = imgin.data[i];
		//pre-checked column, no need on individual column/row checks

		//blank space
		if (stdvals.curval == 0)
		{
			continue;
		}
		// visited populated space
		else if(stdvals.curval > 1)
		{
			continue;
		}

		// found new blob. begin processing
		else if (stdvals.curval == 1)
		{
			stdvals.checklen = 3;
			up = imgin.data[calcarrayidx(x,y-1,maxcols)];
			left = imgin.data[calcarrayidx(x-1,y,maxcols)];
			upleft = imgin.data[calcarrayidx(x-1,y-1,maxcols)];
			stdvals.checkvals = realloc(stdvals.checkvals,stdvals.checklen*sizeof(uint8_t));
			stdvals.checkvals[0] = up; stdvals.checkvals[1] = left; stdvals.checkvals[2] = upleft;
			imgin.data[i] = checkpixel(&stdvals);
		}
	}


	//middle (not top row, right column, or left column) - Checks above, to the upleft, left,and upright only, starts at 2nd pixel 2nd row going right and down from the top left
	for(y = 1; y < maxrows; y++)
	{
		for(x = 1; x < maxcols - 1; x++) //
		{ // cycle through columns
			stdvals.checklen = 4; //this is 4 because you're checking the pixel and 4 pixels around it.
			i = calcarrayidx(x,y,maxcols);
			stdvals.curval = imgin.data[i];
			//pre-checked column, no need on individual column/row checks
			if (y == 0 || y == maxcols)
			{
				continue;
			}

			//blank space
			if (stdvals.curval == 0)
			{
				stdvals.curval = 1;
				continue;
			}

			// visited populated space
			else if(stdvals.curval > 1)
			{
				continue;
			}
			// found new blob. begin processing
			else if (stdvals.curval == 1)
			{
				left = imgin.data[calcarrayidx(x-1,y,maxcols)];
				upleft = imgin.data[calcarrayidx(x-1,y-1,maxcols)];
				up = imgin.data[calcarrayidx(x,y-1,maxcols)];
				upright = imgin.data[calcarrayidx(x+1,y-1,maxcols)];
				stdvals.checkvals = realloc(stdvals.checkvals,stdvals.checklen*sizeof(uint8_t));
				stdvals.checkvals[0] = left; stdvals.checkvals[1] = upleft; stdvals.checkvals[2] = up; stdvals.checkvals[3] = upright;
				curpixel = checkpixel(&stdvals);
				imgin.data[i] = curpixel;
			}

		}
	}

	//execute the below code AFTER running all other items

	//shore up blob relationship array
	// this finds all the blobs that are the same and puts them back together
	
	// TODO:Use the "maxblob" counter to find and clean up blobs to the smallest unused label. - you could interrupt processing to do this cleanup routine mid if the blob count reaches max.

	for(u = 255; u > 0; u--)
	{
		uint8_t checkflag = 0;
		while(checkflag == 0)
		{
			prevval = stdvals.checkarray[u];
			stdvals.checkarray[u] = stdvals.checkarray[stdvals.checkarray[u]];
			if (stdvals.checkarray[u] == u || stdvals.checkarray[u] == prevval)
			{
				checkflag = 1;
			}
		}
	}
	uint8_t holder = 0;
	// clean up blobs
	for(i = 0; i < maxi+1; i++ )
	{
		y = calcypos(i, maxcols);
		x = calcxpos(i, y, maxcols);
		imgin.data[i] = stdvals.checkarray[(uint8_t)imgin.data[i]];
		holder = imgin.data[i];
		blobpos->blobposwmax[holder] = MAX(x,blobpos->blobposwmax[holder]);
		blobpos->blobposwmin[holder] = MIN(x,blobpos->blobposwmin[holder]);
		blobpos->blobposhmax[holder] = MAX(y,blobpos->blobposhmax[holder]);
		blobpos->blobposhmin[holder] = MIN(y,blobpos->blobposhmin[holder]);
		blobpos->blobpopflag[holder] = 1; // 0 will likely always be populated, but it is to be ignored because its blank space. 1 shouldnt be populated, if it is, the cleanup didnt work completely.
		blobpos->blobpxlcnt[holder]++;
		blobpos->blobxcentroid[holder] += x; //add the centroids up, divide after.
		blobpos->blobycentroid[holder] += y;
	}

	//divide all the centroids to average.
	for(i = 0; i < 256; i++ )
	{
		if (blobpos->blobpopflag[i] == 1 ){
			blobpos->blobxcentroid[i] = blobpos->blobxcentroid[i]/blobpos->blobpxlcnt[i];
			blobpos->blobycentroid[i] = blobpos->blobycentroid[i]/blobpos->blobpxlcnt[i];
		}
	}


	free(stdvals.checkvals);

}



