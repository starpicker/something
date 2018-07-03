#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libyuv.h"

#include <math.h>

#define STB_IMAGE_IMPLEMENTATION

#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#include "stb_image.h"

//MUST free return pointer
unsigned char* jpeg_2_nv21(const char* jpeg, int* jpeg_width, int* jpeg_height)
{
    int width, height, comp;
    unsigned char* data = stbi_load(jpeg, &width, &height, &comp, 4);
 
    uint8* src_argb = data;
    int src_stride_argb = width*4;
    uint8* dst_abgr = (unsigned char*)malloc(width*height*4);
    int dst_stride_abgr = width*4;
    
    libyuv::ARGBToABGR(src_argb, src_stride_argb, dst_abgr, dst_stride_abgr, width, height);
    
    uint8* src_abgr = dst_abgr;
    int src_stride_abgr = width*4;

    uint8* dst_y = (unsigned char*)malloc(width*height*3/2);
    int dst_stride_y = width;
    uint8* dst_vu = dst_y+width*height;
    int dst_stride_vu = width;

    libyuv::ARGBToNV21(src_abgr, src_stride_abgr, dst_y, dst_stride_y, dst_vu, dst_stride_vu, width, height);

    stbi_image_free(data);
    //free(dst_y);

    *jpeg_width = width;
    *jpeg_height = height;

    return dst_y;
}

// int main(int argc, char* argv[])
// {
    // if(argc != 2)
    // {
        // printf("usage: %s jpegfile\n", argv[0]);

        // return -1;
    // }

    // int w,h;
    // unsigned char* p = jpeg_2_nv21(argv[1], &w, &h);
    // char out[256];
    // sprintf(out, "%dx%d.nv21", w, h);

    // FILE* fp = fopen(out, "w");
    // fwrite(p, 1, w*h*3/2, fp);
    // fclose(fp);

    // free(p);

	// return 0;
// }
