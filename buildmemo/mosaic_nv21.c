#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void do_mosaic(unsigned char* nv21data, int level,
                int left, int top, int right, int bottom, int width, int height)
{
    int width_step = level*(right - left)/100;
    int height_step = level*(bottom - top)/100;
    
    width_step = width_step>>1<<1;
    height_step = height_step>>1<<1;
    
    enum {kPixelChannelCount = 1};
    
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
                    memcpy(pixel, nv21data + kPixelChannelCount*index, kPixelChannelCount);
                }
                else
                {
                    memcpy(nv21data + kPixelChannelCount*index, pixel, kPixelChannelCount);
                }
            }
            else
            {
                preIndex = (i - 1) * width + j;
                memcpy(nv21data + kPixelChannelCount*index, nv21data + kPixelChannelCount*preIndex, kPixelChannelCount);
            }
        }
    }
    
    
    for(int i = top; i < bottom; i += height_step)
    {
        for(int j = left; j < right; j += width_step)
        {
            int u = nv21data[i*width/2+width*height+j];
            int v = nv21data[i*width/2+width*height+j+1];
            
            for(int m = i; m < i+height_step; m+=2)
            {
                for(int n = j; n < j+width_step; n+=2)
                {
                    nv21data[m*width/2+width*height+n] = u;
                    nv21data[m*width/2+width*height+n+1] = v;
                }
            }   
        }
    }
}


int main(int argc, char** argv)
{
    if(argc != 6)
    {
        printf("%s input.nv21 output.nv21 width height, level\n", argv[0]);
        return -1;
    }
    
    FILE* fp = fopen(argv[1], "r");
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    unsigned char* data_nv21 = (unsigned char*)malloc(width*height*3/2);
    
    fread(data_nv21, width*height*3/2, 1, fp);
    fclose(fp);
    
    int left = 624;
    int top = 280;
    int right = 1070;
    int bottom = 724;

    int level = atoi(argv[5]);
    
    do_mosaic(data_nv21, level, left, top, right, bottom, width, height);
    
    
    fp = fopen(argv[2], "w");
    fwrite(data_nv21, width*height*3/2, 1, fp);
    
    fclose(fp);
    free(data_nv21);
    
    return 0;
}