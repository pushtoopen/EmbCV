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
#include <string.h>
#include "jpeglib.h"

#ifndef JCS_EXTENSIONS
#define JCS_EXT_RGB  6
#endif

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


//pre-declaring the jpeg reader method
void read_JPEG_file_Single_Line_Greyscale (char * filename, struct ImageInfo *m_pImageInfo);
void write_JPEG_file_Single_Line_Greyscale (char * filename, struct ImageInfo *m_pImageInfo);
static void ErrorExit(j_common_ptr cinfo);
static void OutputMessage(j_common_ptr cinfo);

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


void my_error_exit(j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr)cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}



//JPEG to RGB example taken from libjpeg. Reads whole picture into one single array. No array per row is done in this example.

void read_JPEG_file_Single_Line_Greyscale (char * filename, struct ImageInfo *m_pImageInfo)
{
	struct jpeg_decompress_struct *cinfo = calloc(1,sizeof(struct jpeg_decompress_struct));
	struct my_error_mgr jerr;
	//struct ImageInfo m_pImageInfo;

	/* More stuff */
	FILE *pFile;                 /* source file */
	JSAMPARRAY buffer;		/* Output buffer, array of pointers to each row read in, we read the whole picture into one row in this example */
	int row_stride;               /* physical row width in output buffer */

	//system("pwd");
	if ((pFile = fopen(filename, "rb")) == NULL) {
		printf("\n File attempted to be loaded in read_JPEG_file failed to load\n\n");
		exit(0);
	}

	// set our custom error handler
	cinfo->err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		// We jump here on errorz
		//Cleanup();
		jpeg_destroy_decompress(cinfo);
		fclose(pFile);
		printf("\n Failure setting the jump to the correct buffer location during file load in read_JPEG_file\n\n");
		exit(0);
	}

	jpeg_create_decompress(cinfo);
	jpeg_stdio_src(cinfo, pFile);
	jpeg_read_header(cinfo, TRUE);
	//changes to the desired characteristics of the returned image must be done after jpeg_read_header is run. see "http://apodeline.free.fr/DOC/libjpeg/libjpeg-3.html" for more info on options
	//jpeg_set_colorspace(cinfo,JCS_GRAYSCALE);
	cinfo->out_color_space = JCS_GRAYSCALE; //sets to greyscale, same as setitng it equal to 1. see J_COLOR_SPACE enum definition for other options.

	//run this after all options have been set.
	jpeg_start_decompress(cinfo);

	/* JSAMPLEs per row in output buffer */
	//cinfo->output_components = 1;
	row_stride = cinfo->output_width * cinfo->output_components;
	m_pImageInfo->pData = calloc(cinfo->image_width*cinfo->image_height*cinfo->output_components,sizeof(uint8_t));  //< --- this is the typical way that a buffer would be allocated, but using 1 dimension instead of 3 because we're making this greyscale.
	buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr)cinfo, JPOOL_IMAGE, row_stride, 1);


	//reads the entire picture into buffer. the 3rd input to read_scanlines is the number of picture rows you'd like to read into the buffer
	uint64_t i = 0;
	uint8_t *currentloc = NULL;
	uint16_t numlinesread = 0;
	  while (cinfo->output_scanline < cinfo->output_height) {
	    /* jpeg_read_scanlines expects an array of pointers to scanlines.
	     * Here the array is only one element long, but you could ask for
	     * more than one scanline at a time if that's more convenient.
	     */
		  numlinesread = jpeg_read_scanlines(cinfo, buffer, 1); // jpeg_read_scanlines returns the number of lines read
		  //printf("number of lines read: %d\r\n",numlinesread);
		//each scanline is one row of the picture.
	    currentloc = (m_pImageInfo->pData + cinfo->output_scanline*row_stride);
	    memcpy(currentloc,buffer,cinfo->image_width);
	    i++;
	  }

	m_pImageInfo->nWidth = cinfo->image_width;
	m_pImageInfo->nHeight = cinfo->image_height;
	m_pImageInfo->nNumComponent = cinfo->output_components;// normally this is euqla to cinfo->num_components, but that value is 3 because we read in an RGB, but we're converting to greyscale so it should only be 1.
    //m_pImageInfo.pData = buffer[0]; //since buffer is an array of pointers to rows, and we only read in one large row, we assign to [0] to change from a pointer of pointers "**", to a pointer "8" .

    jpeg_finish_decompress(cinfo);
	jpeg_destroy_decompress(cinfo);
	free(cinfo);
    //free(buffer);
	fclose(pFile);


	//return m_pImageInfo;


}

void write_JPEG_file_Single_Line_Greyscale (char * filename, struct ImageInfo *m_pImageInfo)
{

//	 struct jpeg_compress_struct *cinfo = calloc(1,sizeof(struct jpeg_compress_struct)); // This struct contains the JPEG compression parameters and pointers to working space
	 struct jpeg_compress_struct cinfo; // This struct contains the JPEG compression parameters and pointers to working space

	 struct jpeg_error_mgr jerr; // This struct represents a JPEG error handler.
	 uint8_t *currentloc = NULL;
	 /* More stuff */
	 FILE *outfile;                /* target file */
	 JSAMPARRAY row_pointer;      /* pointer to JSAMPLE row[s] */
	 //JSAMPARRAY buffer;
	 int row_stride;               /* physical row width in image buffer */

	 cinfo.err = jpeg_std_error(&jerr);
	 //jerr.pub.error_exit = my_error_exit;
	 jpeg_create_compress(&cinfo);

	 if ((outfile = fopen(filename, "wb")) == NULL) {
	   fprintf(stderr, "can't open %s\n", filename);
	   exit(1);
	 }
	  jpeg_stdio_dest(&cinfo, outfile);
	  //cinfo->data_precision = 8; // this should set with the defaults... but doesnt?
	  //cinfo.in_color_space = JCS_GRAYSCALE;   //must be set before set defaults is called
	  cinfo.in_color_space = JCS_GRAYSCALE;   //must be set before set defaults is called

      cinfo.image_width = m_pImageInfo->nWidth;      /* image width and height, in pixels */
	  cinfo.image_height = m_pImageInfo->nHeight;
	  cinfo.input_components = m_pImageInfo->nNumComponent;           /* # of color components per pixel */
	  cinfo.comp_info->component_id = 1;
      cinfo.comp_info->h_samp_factor = 1;
      cinfo.comp_info->v_samp_factor = 1;
      cinfo.comp_info->quant_tbl_no = 0;
      cinfo.comp_info->dc_tbl_no = 0;
      cinfo.comp_info->ac_tbl_no = 0;
	  //cinfo.input_components = 1;           /* # of color components per pixel */
	  
	  jpeg_default_colorspace (&cinfo);
	  jpeg_set_defaults(&cinfo); //must be called for basic settings to be set.

    /* colorspace of input image */
	  //jpeg_set_colorspace(cinfo,JCS_GRAYSCALE);
	  //jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	  jpeg_start_compress(&cinfo, TRUE);
	  row_stride = cinfo.image_width * cinfo.input_components; /* JSAMPLEs per row in image_buffer */
	 // while (cinfo->next_scanline < cinfo->image_height) {
	    /* jpeg_write_scanlines expects an array of pointers to scanlines.
	     * Here the array is only one element long, but you could pass
	     * more than one scanline at a time if that's more convenient.
	     */
	//	currentloc = m_pImageInfo->pData + cinfo->next_scanline * cinfo->image_width;
	  //  memcpy(row_pointer,currentloc,cinfo->image_width);
	  //  (void)jpeg_write_scanlines(cinfo, row_pointer, 1);
	  //}

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    //row_pointer[0] = m_pImageInfo->pData[cinfo.next_scanline * row_stride];
	currentloc = m_pImageInfo->pData + cinfo.next_scanline * cinfo.image_width;
	memcpy(row_pointer,currentloc,cinfo.image_width);
    (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
  //free(cinfo);
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
