#include <string.h>

typedef struct _MLRect_tag
{
    int left;
    int top;
    int right;
    int bottom;
} MLRect;

#define ML_MAX(a,b) ((a) > (b) ? (a) : (b))
#define ML_MIN(a,b) ((a) < (b) ? (a) : (b))

MLRect intersection(const MLRect a, const MLRect d)
{
    int l = ML_MAX(a.left, d.left);
    int t = ML_MAX(a.top, d.top);
    int r = ML_MIN(a.right, d.right);
    int b = ML_MIN(a.bottom, d.bottom);

    if(l >= r || t >= b)
    {
        l = 0;
        t = 0;
        r = 0;
        b = 0;
    }

    MLRect c = {l, t, r, b};

    return c;
}

void drawRect(const char* nv21, int width, int height, MLRect rect, int lineWidth, int type)
{
    MLRect imageRect = {0, 0, width, height};
    MLRect rectDraw  = intersection(rect, imageRect);

    int left         = rectDraw.left;
    int top          = rectDraw.top;
    int right        = rectDraw.right;
    int bottom       = rectDraw.bottom;

    int rectW        = right - left;
    int rectH        = bottom - top;

    if(top + lineWidth + rectH > height)
    {
        top = height - lineWidth - rectH;
    }

    if(top < lineWidth / 2)
    {
        top = lineWidth / 2;
    }

    if(bottom - lineWidth < 0)
    {
        bottom = lineWidth;
    }

    if(bottom + lineWidth / 2 > height)
    {
        bottom = height - lineWidth / 2;
    }

    if(left + lineWidth + rectW > width)
    {
        left = width - lineWidth - rectW;
    }

    if(left < lineWidth / 2)
    {
        left = lineWidth / 2;
    }

    if(right - lineWidth < 0)
    {
        right = lineWidth;
    }

    if(right + lineWidth / 2 > width)
    {
        right = width - lineWidth / 2;
    }

    rectW = right - left;
    rectH = bottom - top;

    const char* plane0    = nv21;
    const char* plane1    = nv21 + width * height;

    //draw the top line
    const char* pTop      = plane0 + (top - lineWidth / 2) * width + left - lineWidth / 2;
    const char* pVUTop    = plane1 + ((top - lineWidth / 2) / 2) * width + left - lineWidth / 2;
    const char* pYBottom  = plane0 + (top + rectH - lineWidth / 2) * width + left - lineWidth / 2;
    const char* pVUBottom = plane1 + ((top + rectH - lineWidth / 2) / 2) * width + left - lineWidth / 2;

    for(int i = 0; i < lineWidth; i++)
    {
        pVUTop    = plane1 + ((top + i - lineWidth / 2) / 2) * width + left - lineWidth / 2;
        pVUBottom = plane1 + ((top + i + rectH - lineWidth / 2) / 2) * width + left - lineWidth / 2;

        memset((void*)pTop     , 255 , rectW + lineWidth);
        memset((void*)pYBottom , 255 , rectW + lineWidth);

        pTop     += width;
        pYBottom += width;

        if(0 == type)
        {
            memset((void*)pVUTop    , 0  , rectW + lineWidth);
            memset((void*)pVUBottom , 0  , rectW + lineWidth);
        }
        else
        {
            memset((void*)pVUTop    , 60 , rectW + lineWidth);
            memset((void*)pVUBottom , 60 , rectW + lineWidth);
        }
    }

    for(int i = 0; i < rectH; i++)
    {
        const char* pYLeft   = plane0 + (top + i) * width + left - lineWidth / 2;
        const char* pYRight  = plane0 + (top + i) * width + left + rectW - lineWidth / 2;
        const char* pVULeft  = plane1 + (((top + i) / 2) * width) + left - lineWidth / 2;
        const char* pVURgith = plane1 + (((top + i) / 2) * width) + left + rectW - lineWidth / 2;

        memset((void*)pYLeft  , 255 , lineWidth);
        memset((void*)pYRight , 255 , lineWidth);

        if(0 == type)
        {
            memset((void*)pVULeft  , 0  , lineWidth);
            memset((void*)pVURgith , 0  , lineWidth);
        }
        else
        {
            memset((void*)pVULeft  , 60 , lineWidth);
            memset((void*)pVURgith , 60 , lineWidth);
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
unsigned long get_file_size(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0)
    {
    	return filesize;
    }
    else
    {
    	filesize = statbuff.st_size;
    }
    return filesize;
}

int main(void)
{

int filesize = get_file_size("1680x1050.nv21");
char buf[filesize];
FILE* fp = fopen("1680x1050.nv21", "r");
fread(buf, filesize, 1, fp);

fclose(fp);

MLRect r = {300, 400, 1200, 800};

drawRect(buf, 1680, 1050, r, 5, 0);

fp = fopen("1680x1050_rect.nv21", "w");
fwrite(buf, filesize, 1, fp);

fclose(fp);

return 0;
}
