/*
 * print_to_csv.c
 *
 *
 *
 */
// this is an example how to print a greyscale image
// picture values arranged the same way the picture itself is
// this only works in an environment that supports fopen & fprintf


void print_greyscale_to_csv(img_buf imgIn)
{

	FILE *out_file = fopen("picout.csv","w");

	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t l = 0;

	for(y=0; y < imgIn.h; y++)
	{
		for(x = 0; x < imgIn.w; x++)
		{
			l = calcarrayidx(x,y,imgIn.w);
			fprintf(out_file,"%d,",(uint8_t)imgIn.data[l]);
			if((x + 1 ) == imgIn.w)
			{
				fprintf(out_file,"\n,");
			}
		}
	}

	fclose(out_file);
}
