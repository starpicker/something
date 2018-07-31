#include <stdio.h>
#include "bmpfile.h"
int addwords(bmpfile_t *bmp,rgb_pixel_t pixel,unsigned int x,unsigned int y,char*c);
int printstr(bmpfile_t *bmp,rgb_pixel_t pixel,unsigned int x,unsigned int y,char*c);
int
main(int argc, char **argv)
{
	bmpfile_t *bmp;
	int i, j;
	rgb_pixel_t pixel = {128, 64, 0, 0};
	

	if ((bmp = bmp_create(500,50,32) )== NULL) {
		printf("Invalid depth value: %s.\n", argv[4]);
		return 1;
	}
/*	for(i=0;i<600;i++)
	{
		for(j=0;j<800;j++)
		{
			pixel.green=256;
			pixel.red=256;
			pixel.blue=256;
			pixel.alpha=0;
			bmp_set_pixel(bmp,j,i,pixel);
		}
	}
	*/
    pixel.blue=0;
	pixel.red=0;
	pixel.green=0;
	pixel.alpha=0;
    printstr(bmp,pixel,12,13,"hellohello");// \n can cause freeze
	bmp_save(bmp, "1.bmp");
	bmp_destroy(bmp);

	return 0;
}
