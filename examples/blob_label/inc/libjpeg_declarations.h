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
#include <setjmp.h>
#include "../../helpers/libjpeg/build/include/jpeglib.h"

struct ImageInfo
{
	uint32_t nWidth;
	uint32_t nHeight;
	uint8_t nNumComponent;
	uint8_t* pData;
}m_pImageInfo;



struct ErrorManager
{
	jpeg_error_mgr defaultErrorManager;
	jmp_buf jumpBuffer;
};

//pre-declaring the jpeg reader method
int read_JPEG_file (char * filename);
static void ErrorExit(j_common_ptr cinfo);
static void OutputMessage(j_common_ptr cinfo);

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

//JPEG to RGB example taken from libjpeg
GLOBAL(int)
read_JPEG_file (char * filename)
{
	jpeg_decompress_struct cinfo;
	ErrorManager errorManager;

	FILE* pFile = fopen(filename, "rb");
	if (!pFile) {return NULL;}

	// set our custom error handler
	cinfo.err = jpeg_std_error(&errorManager.defaultErrorManager);
	errorManager.defaultErrorManager.error_exit = ErrorExit;
	errorManager.defaultErrorManager.output_message = OutputMessage;
	if (setjmp(errorManager.jumpBuffer))
	{
		// We jump here on errorz
		//Cleanup();
		jpeg_destroy_decompress(&cinfo);
		fclose(pFile);
		return NULL;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pFile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	m_pImageInfo = new ImageInfo();
	m_pImageInfo->nWidth = cinfo.image_width;
	m_pImageInfo->nHeight = cinfo.image_height;
	m_pImageInfo->nNumComponent = cinfo.num_components;
	m_pImageInfo->pData = new uint8_t[m_pImageInfo->nWidth*m_pImageInfo->nHeight*m_pImageInfo->nNumComponent];

	while(cinfo.output_scanline < cinfo.image_height)
	{
		uint8_t* p = m_pImageInfo->pData + cinfo.output_scanline*cinfo.image_width*cinfo.num_components;
		jpeg_read_scanlines(&cinfo, &p, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);

	return m_pImageInfo;


}




#endif /* EXAMPLES_BLOB_LABEL_INC_LIBJPEG_DECLARATIONS_H_ */
