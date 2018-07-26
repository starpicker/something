#include "libyuv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    if(argc != 5)
    {
        printf("%s input.rgb24 output.rgb32 width height\n", argv[0]);
        return -1;
    }
    
    FILE* fp = fopen(argv[1], "r");
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    unsigned char* data24 = (unsigned char*)malloc(width*height*3);
    
    fread(data24, width*height*3, 1, fp);
    fclose(fp);
    
    
    const uint8* src_frame = data24;
    int src_stride_frame = width*3;
    uint8* dst_argb = (unsigned char*)malloc(width*height*4);
    int dst_stride_argb = width*4;
    
    //RAWToARGB
    libyuv::RGB24ToARGB(src_frame, src_stride_frame,
              dst_argb, dst_stride_argb,
              width, height);
                
    fp = fopen(argv[2], "w");
    fwrite(dst_argb, width*height*4, 1, fp);
    fclose(fp);
    
    return 0;
}