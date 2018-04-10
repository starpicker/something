#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "bmpfile.h"

#define         FB_DEV        "/dev/fb0"

int             fb_open(const char* fb_device);
int             fb_close(int fd);
int             fb_stat(int fd, int* width, int* height, int* depth);
void*           fb_mmap(int fd, unsigned int screensize);
int             fb_munmap(void* start, size_t length);

int             addwords(bmpfile_t *bmp,rgb_pixel_t pixel,unsigned int x,unsigned int y,char*c);
int             printstr(bmpfile_t *bmp,rgb_pixel_t pixel,unsigned int x,unsigned int y,char*c);
const unsigned char* 
                getCurrentSystemTime(unsigned char* date);
int             fb_write(int width, int height, int xpos, int ypos);

int
fb_write(int width_, int height_, int xpos, int ypos)
{
    int                           fbdev;
    const char*                   fb_device;
    void*                         fbmem;
    unsigned int                  screensize;
    int                           fb_width;
    int                           fb_height;
    int                           fb_depth;
    bmpfile_t*                    bmp;
	int                           i;
    int                           width = width_;
    int                           height = height_;

	rgb_pixel_t                   pixel;
    unsigned char                 date[100];
    
    pixel.red   = 255;
    pixel.green = 0;
    pixel.blue  = 0;
	pixel.alpha = 255;
    
    if((fb_device = getenv("FRAMEBUFFER")) == NULL)
        fb_device = FB_DEV;

    fbdev = fb_open(fb_device);
    fb_stat(fbdev, &fb_width, &fb_height, &fb_depth);
    
    //printf("fb_width: %d, fb_height: %d, xpos :%d, ypos :%d, width :%d, height :%d\n",
            //fb_width, fb_height, xpos, ypos, width, height);
    
    screensize = fb_width * fb_height * fb_depth / 8;
    fbmem      = fb_mmap(fbdev, screensize);
    
    memset(fbmem, 0, screensize);
    
    unsigned char* startPos = (unsigned char*)fbmem + ypos*4*fb_width + xpos*4;
    unsigned char* argb32 = (unsigned char*)malloc(width*height*4); 
    
    while(true)
    {
        bmp = bmp_create(width, height, 32);
        getCurrentSystemTime(date);
        printstr(bmp, pixel, 10, 20, (char*)date);// \n can cause freeze
        bmp_get_rgb32(bmp, argb32);
        
        for(i = 0; i < height; i++)
        {
            //printf("fbmem+screensize: %p, startPos + i*4*fb_width: %p\n", (unsigned char*)fbmem+screensize, startPos + i*4*fb_width);
            //printf("argb32+width*height*4, :%p, argb32+i*4*width: %p\n", argb32+width*height*4, argb32+i*4*width);
            memcpy(startPos + i*4*fb_width, argb32+i*4*width, 4*width);
        }
        bmp_destroy(bmp);
        
        usleep(30*1000);
    }
    
    fb_munmap(fbmem, screensize);
    fb_close(fbdev);
    free(argb32);
    
    return (0);
}

int
fb_open(const char* fb_device)
{
    int fd;

    if((fd = open(fb_device, O_RDWR)) < 0)
    {
        perror(__func__);
        return (-1);
    }
    
    return (fd);
}

int
fb_stat(int fd, int* width, int* height, int* depth)
{
    struct fb_fix_screeninfo fb_finfo;
    struct fb_var_screeninfo fb_vinfo;

    if(ioctl(fd, FBIOGET_FSCREENINFO, &fb_finfo))
    {
        perror(__func__);
        return (-1);
    }

    if(ioctl(fd, FBIOGET_VSCREENINFO, &fb_vinfo))
    {
        perror(__func__);
        return (-1);
    }

    *width = fb_vinfo.xres;
    *height = fb_vinfo.yres;
    *depth = fb_vinfo.bits_per_pixel;

    return (0);
}

void*
fb_mmap(int fd, unsigned int screensize)
{
    void*             fbmem;

    if((fbmem = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror(__func__);
        return (void *) (-1);
    }

    return (fbmem);
}

int
fb_munmap(void *start, size_t length)
{
    return (munmap(start, length));
}

int
fb_close(int fd)
{
    return (close(fd));
}

const unsigned char* getCurrentSystemTime(unsigned char* date)
{
  time_t now;
  struct tm *ptm;
  struct timeval t;
  
  time(&now);
  ptm = localtime(&now);
    
  gettimeofday(&t, 0);
  sprintf((char*)date, "%d-%02d-%02d %02d:%02d:%02d:%02d",
    (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday,
    (int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec,(int)(t.tv_usec/1000));
  
  return date;
}
