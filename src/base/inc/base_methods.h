/**
 * @file base_methods.h
 *
 *
 *
*/

#ifndef INC_BASE_METHODS_H_
#define INC_BASE_METHODS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

typedef struct img_buf img_buf;
//RGB or greyscale only no JPEG
struct img_buf {
	uint32_t w;   //image width
	uint32_t h;   //image height
	uint8_t c;   //number of channels in the image buffer 1 = greyscale
	uint8_t *data; //image_data values
};

struct kernel {
	float *data; //buffer of the kernel data
	uint32_t length; // total number of values within your kernel
};

uint32_t calcypos(uint32_t i, uint32_t max_x);
uint32_t calcxpos(uint32_t i, uint32_t y_pos, uint32_t max_x);
uint32_t calcarrayidx(uint32_t x_pos, uint32_t y_pos, uint32_t max_x);
uint8_t threshold_image(img_buf buf);
void binarize_image(img_buf buf,uint8_t threshold);



#endif /* INC_BASE_METHODS_H_ */
