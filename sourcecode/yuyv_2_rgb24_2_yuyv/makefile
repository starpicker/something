CFLAGS := -W -Wall
LDFLAGS :=
all: yuv2rgb rgb2yuv
yuv2rgb.o : yuyv_2_rgb24_2_yuyv.c 
	gcc $(CFLAGS) -c -o $@ $<
rgb2yuv.o : yuyv_2_rgb24_2_yuyv.c 
	gcc $(CFLAGS) -DRGB2YUV -c -o $@ $<
yuv2rgb: yuv2rgb.o
	gcc $(LDFLAGS) -o $@ $^
rgb2yuv: rgb2yuv.o
	gcc $(LDFLAGS) -o $@ $^
clean:
	-rm -f *.o
	-rm -f yuv2rgb rgb2yuv
