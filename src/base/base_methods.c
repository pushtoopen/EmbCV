/**
 * @file base_methods.c
 *
 *
 *
*/

#include "base_methods.h"

/**
 * Calculates Y/Height/Row position of a pixel based on index and width of picture.
 *
 * This assumes that the index i goes from top left, across each row, rows counting down.
 * The y value will increase, as you work your way down the picture.
 * This uses the drop of the decimal place from an integer cast to determine the y position.
 * y is based on the rows of the picture
 * y's index is assumed to start at 0.
 * This means if a picture is 320 x 240 then largest y is 239
 * @param[in] i = index
 * @param[in] max_x = picture width
 * @param[out] y_pos = height of the pixel
*/
uint32_t calcypos(uint32_t i, uint32_t max_x)
{
	uint32_t y_pos = (uint32_t)(i/max_x); // this must be a non floating type.
	return y_pos;
}

/**
 * Calculate the X/Width/Column position of a pixel given it's buffer index and
 *
 * This assumes that the index i goes from top left, across each row
 * rows increment as they are counting down the picture.
 * The x value will increase, as you work your way across the picture, restarting at the left side.
 * This requires the calculation of the y position from calcypos.
 * max_x is the maximum number of columns in the picture;
 * x's index is assumed to start at 0.
 * This means if a picture is 320 x 240, then the largest x is 319
 * so if i = maxcolumns, then thats actually + 1 past the end of the max columns
 * and therefore x would be 0 starting new at a new row.
 * @param[in] i = index
 * @param[in] y_pos = current pixel height
 * @param[in] max_x = picture width
 * @param[out] x_pos = horizontal position of the pixel
*/
uint32_t calcxpos(uint32_t i, uint32_t y_pos, uint32_t max_x)
{
	uint32_t x_pos = i-(max_x*y_pos);
	return x_pos;
}

/**
 * Calculates the index of the picture buffer based on the input x and y values.
 *
 * This assumes that the index i goes from top left starting at 0, across each row
 * rows increment as they are counting down the picture.
 * max_x is the maximum number of columns in the picture, indexed from 0 (e.g. pic width 320 = 319)
 * x & y's index is assumed to start at 0.
 * This means if a picture is 320 x 240, then the largest x is 319, including as an input
 * for example, to calculate the maximum index, maxcols = 320 and maxrows = 240:
 * maxi = calcarrayidx(maxcols-1,maxrows-1,maxcols);
 * @param[in] i = index
 * @param[in] y_pos = current pixel height
 * @param[in] max_x = picture width
 * @param[out] array_idx = index of the picture buffer based on x/y pixel pos
*/
uint32_t calcarrayidx(uint32_t x_pos, uint32_t y_pos, uint32_t max_x)
{
	uint32_t arrayindex = x_pos + max_x*y_pos; //i = uint32_t
	return arrayindex;
}


/**
 * Calculates OTSU threshold for a greyscale image
 *
 * does not edit the image in any way
 * image must be greyscale (one channel) else it returns a threshold of 255 (max val of uint8)
 * @param[in] buf = image buffer & characteristics
 * @param[out] threshold = Otsu threshold value
*/
uint8_t threshold_image(img_buf buf){

	uint16_t GREYLEVEL = 256; // this means that the values range from  0 to 255 for the image thats being binarized.


	if(buf.c > 1)
	{
		//does not work on non-greyscale images.
		return (uint8_t)255;
	}

	float threshold = 0;
	double pd[GREYLEVEL];
	double w[GREYLEVEL];
	double mu[GREYLEVEL];
	double sigma_max = 0;
	uint32_t histogram[GREYLEVEL];
	double sigma[GREYLEVEL];

	uint32_t i = 0;

	for (i = 0; i < GREYLEVEL; i++)
	{
		histogram[i] = 0;
	}
	for (i = 0; i < buf.h*buf.w; ++i) {
		histogram[(unsigned char)(255. * buf.data[i])]++;
	}
	for (i = 0; i < GREYLEVEL; i++) {
		pd[i] = (double)histogram[i] / (buf.w*buf.h);
	}

	w[0] = pd[0];

	mu[0] = 0;

	for (i = 1; i < GREYLEVEL; i++) {
		w[i] = w[i - 1] + pd[i];
		mu[i] = mu[i - 1] + i * pd[i];
	}

	for (i = 0; i < GREYLEVEL - 1; i++) {
		if (w[i] != 0.0 && w[i] != 1.0){
			sigma[i] = pow(mu[GREYLEVEL - 1] * w[i] - mu[i], 2) /
			(w[i] * (1.0 - w[i]));
		}
		else
		{
			sigma[i] = 0.0;
		}
		if (sigma[i] > sigma_max) {
			sigma_max = sigma[i];
			threshold = (float)i;
		}
	}
	return (uint8_t)threshold;
}

/**
 * Given a uint8_t threshold, edits a picture's pixels to be either 0 or 255
 *
 * @param[in] buf = image buffer & characteristics
 * @param[in] threshold = Otsu threshold value
*/
void binarize_image(img_buf buf,uint8_t threshold)
{
	for (int i = 0; i < buf.w*buf.h*buf.c; ++i)	{
			if (buf.data[i] > threshold){
				//buf.data[i] = 255;
				buf.data[i] = 255;				
			}
			else
			{
				buf.data[i] = 0;
			}
		}
}

