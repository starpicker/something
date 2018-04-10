#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void do_mosaic(unsigned char* rgbdata, int level,
                int left, int top, int right, int bottom, int width)
{
    int width_step = level*(right - left)/100;
    int height_step = level*(bottom - top)/100;
    
    enum {kPixelChannelCount = 3};
    
    unsigned char pixel[kPixelChannelCount] = {0};
    int index, preIndex, i, j;
    for(i = top; i < bottom; i++)
    {
        for(j = left; j < right; j++)
        {
            index = i * width + j;        
            if((i - top) % height_step == 0)
            {
                if((j - left) % width_step == 0)
                {
                    memcpy(pixel, rgbdata + kPixelChannelCount*index, kPixelChannelCount);
                }
                else
                {
                    memcpy(rgbdata + kPixelChannelCount*index, pixel, kPixelChannelCount);
                }
            }
            else
            {
                preIndex = (i - 1) * width + j;
                memcpy(rgbdata + kPixelChannelCount*index, rgbdata + kPixelChannelCount*preIndex, kPixelChannelCount);
            }
        }
    }
    
}


int main(int argc, char** argv)
{
    if(argc != 6)
    {
        printf("%s input.rgb24 output.rgb24 width height, level\n", argv[0]);
        return -1;
    }
    
    FILE* fp = fopen(argv[1], "r");
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    unsigned char* data24 = (unsigned char*)malloc(width*height*3);
    
    fread(data24, width*height*3, 1, fp);
    fclose(fp);
    
    int left = 625;
    int top = 279;
    int right = 1071;
    int bottom = 724;

    int level = atoi(argv[5]);
    
    do_mosaic(data24, level, left, top, right, bottom, width);
    
    
    fp = fopen(argv[2], "w");
    fwrite(data24, width*height*3, 1, fp);
    
    fclose(fp);
    free(data24);
    
    return 0;
}