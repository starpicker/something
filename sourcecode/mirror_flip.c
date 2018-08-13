void MirrorImage(unsigned char* y, unsigned char* uv, int w, int h, int stride)
{
    unsigned char*  srcY  = y;
    unsigned char*  srcUV = uv;
    int width             = w;
    int height            = h;
    int pitch             = stride;
    int widthOffet        = pitch - width;
    unsigned char temp;
    int startIndex, endIndex;

    // mirror y
    for(int i = 0; i < height; i++)
    {
        startIndex = i * pitch;
        endIndex   = (i + 1) * pitch - 1 - widthOffet;

        while(startIndex < endIndex)
        {
            temp             = srcY[startIndex];
            srcY[startIndex] = srcY[endIndex];
            srcY[endIndex]   = temp;
            startIndex++;
            endIndex--;
        }
    }

    // mirror u and v
    for(int i = 0; i < height / 2; i++)
    {
        startIndex = i * pitch;
        endIndex   = (i + 1) * pitch - 2 - widthOffet;

        while(startIndex < endIndex)
        {
            temp                  = srcUV[startIndex];
            srcUV[startIndex]     = srcUV[endIndex];
            srcUV[endIndex]       = temp;
            temp                  = srcUV[startIndex + 1];
            srcUV[startIndex + 1] = srcUV[endIndex + 1];
            srcUV[endIndex + 1]   = temp;
            startIndex           += 2;
            endIndex             -= 2;
        }
    }
}

void FlipImage(unsigned char* y, unsigned char* uv, int w, int h, int stride)
{
    unsigned char* srcY       = y;
    unsigned char* srcUV      = uv;
    int width                 = w;
    int height                = h;
    int pitch                 = stride;
    unsigned char* tempBuf    = (unsigned char*)malloc(sizeof(unsigned char) * width);
    int startIndex, endIndex;

    // flip y
    for(int i = 0; i < height / 2; i++)
    {
        startIndex = i * pitch;
        endIndex   = (height - i - 1) * pitch;
        memcpy(tempBuf           , &srcY[startIndex] , width);
        memcpy(&srcY[startIndex] , &srcY[endIndex]   , width);
        memcpy(&srcY[endIndex]   , tempBuf           , width);
    }

    // flip u and v
    for(int i = 0; i < height / 4; i++)
    {
        startIndex = i * pitch;
        endIndex   = (height / 2 - i - 1) * pitch;
        memcpy(tempBuf            , &srcUV[startIndex] , width);
        memcpy(&srcUV[startIndex] , &srcUV[endIndex]   , width);
        memcpy(&srcUV[endIndex]   , tempBuf            , width);
    }

    free(tempBuf);
}
