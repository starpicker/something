#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

#include "jpeglib.h"
#include "libyuv.h"

int jpeg_2_rgb24(const char* jpeg, unsigned char** rgb24, int* width, int* height)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * infile;
	JSAMPARRAY buffer;
	int row_stride;

	if ((infile = fopen(jpeg, "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", jpeg);
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	printf("image_width = %d\n", cinfo.image_width);
	printf("image_height = %d\n", cinfo.image_height);
	printf("num_components = %d\n", cinfo.num_components);

    *width = cinfo.image_width;
    *height = cinfo.image_height;

	jpeg_start_decompress(&cinfo);

	printf("cinfo.out_color_space = %d\n", cinfo.out_color_space);

    if(cinfo.out_color_space != JCS_RGB)
    {
        return -1;
    }

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    *rgb24 = (unsigned char*)malloc((*width) * (*height) * 3);
    unsigned char* temp = *rgb24;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);

        memcpy(temp, (const char*)*buffer, row_stride);
        temp += row_stride;
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return 0;
}

//MUST free return pointer
unsigned char* jpeg_2_nv21(const char* jpeg, int* jpeg_width, int* jpeg_height)
{
    int width, height;
    unsigned char* rgb24;
	int ret = jpeg_2_rgb24(jpeg, &rgb24, &width, &height);
    if(ret != 0)
    {
        printf("now only support rgb24 in jpg\n");

        return 0;
    }

    {
      int i;
      for (i = 0; i < width*height*3; i += 3)
      {
        unsigned char r = rgb24[i+2];
        unsigned char g = rgb24[i+1];
        unsigned char b = rgb24[i];

        rgb24[i] = r;
        rgb24[i+1] = g;
        rgb24[i+2] = b;
      }
    }

    const uint8* src_frame = rgb24;
    int src_stride_frame = width*3;
    uint8* dst_rgb32 = (unsigned char*)malloc(width*height*4);
    int dst_stride_rgb32 = width*4;
    libyuv::RGB24ToARGB(src_frame, src_stride_frame, dst_rgb32, dst_stride_rgb32, width, height);
    uint8* src_argb = dst_rgb32;
    int src_stride_argb = width*4;

    uint8* dst_y = (unsigned char*)malloc(width*height*3/2);
    int dst_stride_y = width;
    uint8* dst_vu = dst_y+width*height;
    int dst_stride_vu = width;

    libyuv::ARGBToNV21(src_argb, src_stride_argb, dst_y, dst_stride_y, dst_vu, dst_stride_vu, width, height);

    free(rgb24);
    free(dst_rgb32);
    //free(dst_y);

    *jpeg_width = width;
    *jpeg_height = height;

    return dst_y;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("usage: %s jpegfile\n", argv[0]);

        return -1;
    }

    int w,h;
    unsigned char* p = jpeg_2_nv21(argv[1], &w, &h);
    char out[256];
    sprintf(out, "%dx%d.nv21", w, h);

    FILE* fp = fopen(out, "w");
    fwrite(p, 1, w*h*3/2, fp);
    fclose(fp);

    free(p);

	return 0;
}
