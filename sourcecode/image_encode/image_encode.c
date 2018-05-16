#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{
    if(argc != 6)
    {
        printf("usage: %s input width height comp output\n", argv[0]);
        
        return -1;
    }

    FILE* fp = fopen(argv[1], "r");
    int w = atoi(argv[2]);
    int h = atoi(argv[3]);
    int c = atoi(argv[4]);
    char buffer[w*h*c];
    fread(buffer, w*h*c, 1, fp);
    fclose(fp);
    
    stbi_write_jpg(argv[5], w, h, c, buffer, 90);
    
    return 0;
}