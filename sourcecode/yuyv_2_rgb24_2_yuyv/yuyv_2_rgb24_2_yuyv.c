//makefile
/*
CFLAGS := -W -Wall
LDFLAGS :=
all: yuv2rgb rgb2yuv
yuv2rgb.o : main.c
 gcc $(CFLAGS) -c -o $@ $<
rgb2yuv.o : main.c
 gcc $(CFLAGS) -DRGB2YUV -c -o $@ $<
yuv2rgb: yuv2rgb.o
 gcc $(LDFLAGS) -o $@ $^
rgb2yuv: rgb2yuv.o
 gcc $(LDFLAGS) -o $@ $^
clean:
 -rm -f *.o
 -rm -f yuv2rgb rgb2yuv
*/
 
/*
 * Convert between YUY2 (YUYV) and RGB.
 *
 * Copyright 2007 (c) Logitech. All Rights Reserved.
 *
 * This code and information is provided "as is" without warranty of
 * any kind, either expressed or implied, including but not limited to
 * the implied warranties of merchantability and/or fitness for a
 * particular purpose.
 */
#include <stdio.h>
#include <stdlib.h>

int convert_rgb_to_yuv_pixel(int r, int g, int b)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int y, u, v;
 y =   0.299 * (r - 128) + 0.587 * (g - 128) + 0.114 * (b - 128) + 128;
 u = - 0.147 * (r - 128) - 0.289 * (g - 128) + 0.436 * (b - 128) + 128;
 v =   0.615 * (r - 128) - 0.515 * (g - 128) - 0.100 * (b - 128) + 128;
 if(y > 255) y = 255;
 if(u > 255) u = 255;
 if(v > 255) v = 255;
 if(y < 0) y = 0;
 if(u < 0) u = 0;
 if(v < 0) v = 0;
 pixel[0] = y;
 pixel[1] = u;
 pixel[2] = v;
 return pixel32;
}

int convert_rgb_to_yuv_buffer(unsigned char *rgb, unsigned char *yuv, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel32;
 int y0, u0, v0, y1, u1, v1;
 for(in = 0; in < width * height * 3; in += 6) {
  pixel32 = convert_rgb_to_yuv_pixel(rgb[in], rgb[in + 1], rgb[in + 2]);
  y0 = (pixel32 & 0x000000ff);
  u0 = (pixel32 & 0x0000ff00) >>  8;
  v0 = (pixel32 & 0x00ff0000) >> 16;
  pixel32 = convert_rgb_to_yuv_pixel(rgb[in + 3], rgb[in + 4], rgb[in + 5]);
  y1 = (pixel32 & 0x000000ff);
  u1 = (pixel32 & 0x0000ff00) >>  8;
  v1 = (pixel32 & 0x00ff0000) >> 16;
  yuv[out++] = y0;
  yuv[out++] = (u0 + u1) / 2;
  yuv[out++] = y1;
  yuv[out++] = (v0 + v1) / 2;
 }
 return 0;
}

int convert_rgb_to_yuv_file(char *rgbfile, char *yuvfile, unsigned int width, unsigned int height)
{
 FILE *in, *out;
 unsigned char *yuv, *rgb;
 in = fopen(rgbfile, "rb");
 if(!in)
  return 1;
 out = fopen(yuvfile, "wb");
 if(!out)
  return 1;
 rgb = malloc(width * height * 3);
 if(rgb == NULL)
  return 2;
 yuv = malloc(width * height * 2);
 if(yuv == NULL)
  return 2;
 fread(rgb, width * height * 3, 1, in);
 fclose(in);
 if(convert_rgb_to_yuv_buffer(rgb, yuv, width, height))
  return 3;
 fwrite(yuv, width * height * 2, 1, out);
 fclose(out);
 return 0;
}

int convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}

int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 return 0;
}

int convert_yuv_to_rgb_file(char *yuvfile, char *rgbfile, unsigned int width, unsigned int height)
{
 FILE *in, *out;
 unsigned char *yuv, *rgb;
 in = fopen(yuvfile, "rb");
 if(!in)
  return 1;
 out = fopen(rgbfile, "wb");
 if(!out)
  return 1;
 yuv = malloc(width * height * 2);
 if(yuv == NULL)
  return 2;
 rgb = malloc(width * height * 3);
 if(rgb == NULL)
  return 2;
 fread(yuv, width * height * 2, 1, in);
 fclose(in);
 if(convert_yuv_to_rgb_buffer(yuv, rgb, width, height))
  return 3;
 fwrite(rgb, width * height * 3, 1, out);
 fclose(out);
 return 0;
}

int main(int argc, char **argv)
{
 int ret = 0;
 char *infile, *outfile;
 unsigned int width, height;
 if(argc < 5) {
#ifdef RGB2YUV
  fprintf(stderr, "Convert RGB images to YUY2 (YUYV).\n\n");
#else
  fprintf(stderr, "Convert YUY2 (YUYV) images to RGB.\n\n");
#endif
  fprintf(stderr, "Syntax: %s width height input output\n", argv[0]);
  return 1;
 }
 width = atoi(argv[1]);
 height = atoi(argv[2]);
 infile = argv[3];
 outfile = argv[4];
#ifdef RGB2YUV
 printf("RGB2YUY: %s => %s\n", infile, outfile);
 ret = convert_rgb_to_yuv_file(infile, outfile, width, height);
#else
 printf("YUV2RGB: %s => %s\n", infile, outfile);
 ret = convert_yuv_to_rgb_file(infile, outfile, width, height);
#endif
 if(ret == 0) {
  printf("Done.\n");
 } else {
  printf("Failed (error = %d). Aborting.\n", ret);
  return 1;
 }
 return 0;
}
