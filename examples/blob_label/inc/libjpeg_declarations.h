/*
 * libjpeg_declarations.h
 *
 *  Created on: Nov 14, 2020
 *      Author: jmccrory
 */

#ifndef EXAMPLES_BLOB_LABEL_INC_LIBJPEG_DECLARATIONS_H_
#define EXAMPLES_BLOB_LABEL_INC_LIBJPEG_DECLARATIONS_H_


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>
#include "../../helpers/libjpeg/build/include/jpeglib.h"

#define FALSE 0
#define TRUE 1
//#define NULL ((char *)0)
struct ImageInfo
{
	uint32_t nWidth;
	uint32_t nHeight;
	uint8_t nNumComponent;
	uint8_t* pData;
};



struct ErrorManager
{
	struct jpeg_error_mgr defaultErrorManager;
	jmp_buf jumpBuffer;
};

//pre-declaring the jpeg reader method
struct ImageInfo read_JPEG_file (char * filename);
static void ErrorExit(j_common_ptr cinfo);
static void OutputMessage(j_common_ptr cinfo);

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


struct ErrorManager errorManager;
struct ImageInfo m_pImageInfo;
int row_stride;		/* physical row width in output buffer */


//JPEG to RGB example taken from libjpeg

struct ImageInfo read_JPEG_file (char * filename)
{

	struct jpeg_decompress_struct cinfo;
	JSAMPARRAY buffer;		/* Output buffer, this is a char pointer at its base type */
	FILE* pFile = fopen(filename, "rb");
	if (!pFile) {
		printf("\n File attempted to be loaded in read_JPEG_file\n\n");
		exit(0);
	}

	// set our custom error handler
	cinfo.err = jpeg_std_error(&errorManager.defaultErrorManager);
	errorManager.defaultErrorManager.error_exit = ErrorExit;
	//errorManager.defaultErrorManager.output_message = OutputMessage;
	if (setjmp(errorManager.jumpBuffer))
	{
		// We jump here on errorz
		//Cleanup();
		jpeg_destroy_decompress(&cinfo);
		fclose(pFile);
		printf("\n Failure setting the jump to the correct buffer location during file load in read_JPEG_file\n\n");
		exit(0);
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pFile);
	jpeg_read_header(&cinfo, TRUE);
	//changes to the desired characteristics of the returned image must be done after jpeg_read_header is run. see "http://apodeline.free.fr/DOC/libjpeg/libjpeg-3.html" for more info on options
	cinfo.jpeg_color_space = JCS_GRAYSCALE; //sets to greyscale, same as setitng it equal to 1. see J_COLOR_SPACE enum definition for other options.

	//run this after all options have been set.
	jpeg_start_decompress(&cinfo);

	//m_pImageInfo = new ImageInfo();
	m_pImageInfo.nWidth = cinfo.image_width;
	m_pImageInfo.nHeight = cinfo.image_height;
	m_pImageInfo.nNumComponent = cinfo.num_components;


	  /* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;
	  /* Make a one-row-high sample array that will go away when done with image */
	  buffer = calloc(cinfo.image_width*cinfo.image_height,cinfo.num_components*sizeof(uint8_t));



	//reads the entire picture into buffer. the 3rd input to read_scanlines is the number of picture rows you'd like to read into the buffer
	//since were doing the entire picture into one buffer, no while loop is needed.
    jpeg_read_scanlines(&cinfo, buffer, cinfo.image_height);
    m_pImageInfo.pData = buffer;
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);

	return m_pImageInfo;


}

static void ErrorExit(j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


#endif /* EXAMPLES_BLOB_LABEL_INC_LIBJPEG_DECLARATIONS_H_ */
