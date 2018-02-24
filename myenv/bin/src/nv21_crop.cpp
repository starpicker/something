#include "libyuv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc != 9)
    {
        printf("%s input.nv21 output.nv21 width height left top right bottom\n", argv[0]);
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);

	int left = atoi(argv[5]);
	int top = atoi(argv[6]);
	int right = atoi(argv[7]);
	int bottom = atoi(argv[8]);

    int startX = left;
    int startY = top;
    int crop_width = right - left;
    int crop_height = bottom - top;

    unsigned char* data = (unsigned char*)malloc(width*height*3/2);
    unsigned char* data_crop = (unsigned char*)malloc(crop_width*crop_height*3/2);

    fread(data, width*height*3/2, 1, fp);
    fclose(fp);

    for(int i = 0; i < crop_height; i++)
    {
		for(int j = 0; j < crop_width; j++)
		{
			//memcpy(data_crop+i*crop_width, data+(startY+i)*width+startX, crop_height);
			memcpy(data_crop+i*crop_width, data+(startY+i)*width+startX, crop_width);
		}
    }


    for(int i = 0; i < crop_height; i+=2)
    {
        for(int j = 0; j < crop_width; j ++)
        {
            data_crop[i*crop_width/2+crop_width*crop_height+j] = data[(startY+i)*width/2+width*height+startX+j];
        }
    }

	char name[256];
	//sprintf(name, "%dx%d_%s", crop_width, crop_height, argv[2]);
sprintf(name, "%s", argv[2]);
    fp = fopen(name, "w");
    fwrite(data_crop, crop_width*crop_height*3/2, 1, fp);
    fclose(fp);

    return 0;
}
