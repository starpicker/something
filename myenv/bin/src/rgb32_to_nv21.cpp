#include "libyuv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    if(argc != 5)
    {
        printf("%s input.rgb32 output.nv21 width height\n", argv[0]);
        return -1;
    }
    
    FILE* fp = fopen(argv[1], "r");
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    unsigned char* data32 = (unsigned char*)malloc(width*height*4);
    
    fread(data32, width*height*4, 1, fp);
    fclose(fp);
   /* 
// dont free
    unsigned char* data322 = (unsigned char*)malloc(width*height*4);
    int src_stride_frame = width*4;
	int dst_stride_bgra = width*4;

libyuv::RGBAToARGB(data32, src_stride_frame, data322, dst_stride_bgra, width, height);

*/
    
    const uint8* src_frame = data32;
    int src_stride_frame = width*4;
    uint8* dst_y = (unsigned char*)malloc(width*height*3/2);
    uint8* dst_vu = dst_y + width*height + 1;
    int dst_stride_vu = width;
    int dst_stride_y = width;
    
    libyuv::ARGBToNV21(src_frame, src_stride_frame,
              dst_y, dst_stride_y,
              dst_vu, dst_stride_vu,
              width, height);
                
    fp = fopen(argv[2], "w");
    fwrite(dst_y, width*height*3/2, 1, fp);
    fclose(fp);
    
    return 0;
}
