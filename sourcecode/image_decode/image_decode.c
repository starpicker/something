//gcc -I. image_decode.c -lm
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION

#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#include "stb_image.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);
        return -1;
    }
    
    int w,h,c;
    unsigned char* data = stbi_load(argv[1], &w, &h, &c, 0);
    
    if(c != 3 && c != 4)
    {
        printf("channel is %d not good, return\n", c);
        return 0;
    }
    
    char path[256];
    sprintf(path, "%dx%d.%s", w, h, (c==3)?"rgb24":"rgb32");
    FILE* fp = fopen(path, "w");
    fwrite(data, (c==3)?(w*h*3):(w*h*4), 1, fp);
    fclose(fp);
    
    stbi_image_free(data);
    
    return 0;
}