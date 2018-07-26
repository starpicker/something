#include "libyuv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    if(argc != 5)
    {
        printf("%s input.nv21 output.rgb32 width height\n", argv[0]);
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    unsigned char* yuvdata = (unsigned char*)malloc(width*height*3/2);
    
    fread(yuvdata, width*height*3/2, 1, fp);
    fclose(fp);
    
    
    const uint8* src_y = yuvdata;
    int src_stride_y = width;
    const uint8* src_vu = yuvdata+width*height+1;
    int src_stride_vu = width;
    uint8* dst_argb = (unsigned char*)malloc(width*height*4);
    int dst_stride_argb = width*4;
    
    libyuv::NV21ToARGB(src_y, src_stride_y, src_vu, src_stride_vu, dst_argb, dst_stride_argb, width, height);
                
    fp = fopen(argv[2], "w");
    fwrite(dst_argb, width*height*4, 1, fp);
    fclose(fp);
    
    return 0;
}
