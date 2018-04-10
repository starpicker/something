/**
 * libbmp 16 点阵字符扩展 支持 GBK 与ascii
 * Copyright (C) 2014 weidiyang
 * mail: ywd600@gmail.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * $Id$
 */
extern unsigned char zmk16[];
extern unsigned char hzk16[];
#include <stdio.h>
#include "bmpfile.h"

int addwords(bmpfile_t *bmp,rgb_pixel_t pixel,unsigned int x,unsigned int y,char*c)
{
	int i,j,retv;
	unsigned char *p=NULL;

    if(((*c)&0x80)==0)//ascii
	{
		retv=1;
		if(*c<0x20)
			return -1;
        p=zmk16+16*(*c-0x20);
		for(i=0;i<16;i++)
			for(j=0;j<8;j++)
			{
				if(!(p[i]&(1<<j)))
					continue;
				if(i<8)
				{
		           bmp_set_pixel(bmp,x+i,y+j,pixel);
				}
				else
				{
					bmp_set_pixel(bmp,x+i-8,y+j+8,pixel);
				}
			}

	}
	else//汉字
	{
		retv=2;
		p=hzk16+(((((*(unsigned short*)c)>>8)&0xff)-0xa1)+(((*(unsigned short*)c)&0xff)-0xa1)*94)*32;
		for(i=0;i<32;i++)
			for(j=0;j<8;j++)
			{
				if(!(p[i]&(1<<j)))
					continue;
				if(i%2)
				{
					bmp_set_pixel(bmp,x+16-j,y+i/2,pixel);
				}
				else
				{
					bmp_set_pixel(bmp,x+8-j,y+i/2,pixel);
				}
			}
	}
	return retv;
}
int printstr(bmpfile_t *bmp,rgb_pixel_t pixel,unsigned int x,unsigned int y,char*c)
{
	int len=0;
	while(c[len])
	{
		len+=addwords(bmp,pixel,x+8*len,y,c+len);
	}
	return len;
}