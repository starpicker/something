/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary 
and confidential information. 

The information and code contained in this file is only for authorized ArcSoft 
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER 
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy, 
distribute, modify, or take any action in reliance on it. 

If you have received this file in error, please immediately notify ArcSoft and 
permanently delete the original and any copy of any file and any printout 
thereof.
*******************************************************************************/

#include <android/log.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ammem.h"
#include "asvloffscreen.h"
#include "arcsoft_high_dynamic_range.h"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "maleitest", __VA_ARGS__)

extern unsigned char* jpeg_2_nv21(const char* jpeg, int* jpeg_width, int* jpeg_height);

int main(int argc, char** argv)
{
    //argc == 2 get width and height for android shell having less-featured file
    if(argc < 2)
    {
        printf("usage: %s input.jpg [Orientation]\n",
                argv[0]);
                
        LOGE("usage: %s input.jpg [Orientation]\n",
                argv[0]);
                
        return -1;
    }
    
    MRESULT res;
    MFloat fConValue  = 0.0f;
    MInt32 EVArray[3] = { 0 };
    MInt32 i32ShotNumber = 0;
    
    MInt32 fOrientation  = (argc == 2)?0:atoi(argv[2]);
    ASVLOFFSCREEN fPreview;

	int lWidth, lHeight;
	MByte* pImgData;

    pImgData = jpeg_2_nv21(argv[1], &lWidth, &lHeight);
    if(pImgData == 0)
    {
        printf("jpeg_2_nv21 fail\n");
        
        return -1;
    }
    
    fPreview.i32Width = lWidth;
    fPreview.i32Height = lHeight;
    fPreview.u32PixelArrayFormat = ASVL_PAF_NV21;
    fPreview.pi32Pitch[0] = lWidth;
    fPreview.pi32Pitch[1] = lWidth;
    fPreview.pi32Pitch[2] = 0;
    fPreview.ppu8Plane[0] = pImgData;
    fPreview.ppu8Plane[1] = fPreview.ppu8Plane[0] + fPreview.i32Height*fPreview.pi32Pitch[0];
    fPreview.ppu8Plane[2] = 0;
    
    printf("w: %d, h: %d, o: %d\n", fPreview.i32Width, fPreview.i32Height, fOrientation);
    
    if(argc == 2)
        return 0;
    // FILE* fp = fopen("3040x4056.nv21", "w");
    // fwrite(fPreview.ppu8Plane[0], 1, lWidth*lHeight*3/2, fp);
    // fclose(fp);
    
    res = ARC_HDR_ParameterDetect(&fPreview, fOrientation, -12, 12, 6,
            &i32ShotNumber, EVArray, &fConValue);

    printf("ARC_HDR_ParameterDetect res = %ld, fConValue: %f, i32ShotNumber: %d, EVArray:{%2d, %2d, %2d}\n", res, fConValue, i32ShotNumber,         EVArray[0], EVArray[1], EVArray[2]);
    
    LOGE("ARC_HDR_ParameterDetect res = %ld, fConValue: %f, i32ShotNumber: %d, EVArray:{%2d, %2d, %2d}\n", res, fConValue, i32ShotNumber,       EVArray[0], EVArray[1], EVArray[2]);
    
    if(fPreview.ppu8Plane[0])
        MMemFree(MNull, fPreview.ppu8Plane[0]);
    
	return res;
}