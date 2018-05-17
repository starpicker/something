// Saliency.cpp: implementation of the Saliency class.
//
//////////////////////////////////////////////////////////////////////
//===========================================================================
//    Copyright (c) 2009 Radhakrishna Achanta [EPFL] 
//===========================================================================

#include "Saliency.h"
#include <cmath>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Saliency::Saliency()
{

}

Saliency::~Saliency()
{

}

//===========================================================================
///    RGB2LAB
//===========================================================================
void Saliency::RGB2LAB(
    const vector<unsigned int>&                ubuff,
    vector<double>&                    lvec,
    vector<double>&                    avec,
    vector<double>&                    bvec)
{
    int sz = int(ubuff.size());
    lvec.resize(sz);
    avec.resize(sz);
    bvec.resize(sz);

    for( int j = 0; j < sz; j++ )
    {
        int r = (ubuff[j] >> 16) & 0xFF;
        int g = (ubuff[j] >>  8) & 0xFF;
        int b = (ubuff[j]      ) & 0xFF;

        double xval = 0.412453 * r + 0.357580 * g + 0.180423 * b;
        double yval = 0.212671 * r + 0.715160 * g + 0.072169 * b;
        double zVal = 0.019334 * r + 0.119193 * g + 0.950227 * b;

        xval /= (255.0 * 0.950456);
        yval /=  255.0;
        zVal /= (255.0 * 1.088754);

        double fX, fY, fZ;
        double lval, aval, bval;

        if (yval > 0.008856)
        {
            fY = pow(yval, 1.0 / 3.0);
            lval = 116.0 * fY - 16.0;
        }
        else
        {
            fY = 7.787 * yval + 16.0 / 116.0;
            lval = 903.3 * yval;
        }

        if (xval > 0.008856)
            fX = pow(xval, 1.0 / 3.0);
        else
            fX = 7.787 * xval + 16.0 / 116.0;

        if (zVal > 0.008856)
            fZ = pow(zVal, 1.0 / 3.0);
        else
            fZ = 7.787 * zVal + 16.0 / 116.0;

        aval = 500.0 * (fX - fY)+128.0;
        bval = 200.0 * (fY - fZ)+128.0;

        lvec[j] = lval;
        avec[j] = aval;
        bvec[j] = bval;
    }
}

//==============================================================================
///    GaussianSmooth
///
///    Blur an image with a separable binomial kernel passed in.
//==============================================================================
void Saliency::GaussianSmooth(
    const vector<double>&            inputImg,
    const int&                        width,
    const int&                        height,
    const vector<double>&            kernel,
    vector<double>&                    smoothImg)
{
    int center = int(kernel.size())/2;

    int sz = width*height;
    smoothImg.clear();
    smoothImg.resize(sz);
    vector<double> tempim(sz);
    int rows = height;
    int cols = width;
   //--------------------------------------------------------------------------
   // Blur in the x direction.
   //---------------------------------------------------------------------------
    {int index(0);
    for( int r = 0; r < rows; r++ )
    {
        for( int c = 0; c < cols; c++ )
        {
            double kernelsum(0);
            double sum(0);
            for( int cc = (-center); cc <= center; cc++ )
            {
                if(((c+cc) >= 0) && ((c+cc) < cols))
                {
                    sum += inputImg[r*cols+(c+cc)] * kernel[center+cc];
                    kernelsum += kernel[center+cc];
                }
            }
            tempim[index] = sum/kernelsum;
            index++;
        }
    }}

    //--------------------------------------------------------------------------
    // Blur in the y direction.
    //---------------------------------------------------------------------------
    {int index = 0;
    for( int r = 0; r < rows; r++ )
    {
        for( int c = 0; c < cols; c++ )
        {
            double kernelsum(0);
            double sum(0);
            for( int rr = (-center); rr <= center; rr++ )
            {
                if(((r+rr) >= 0) && ((r+rr) < rows))
                {
                   sum += tempim[(r+rr)*cols+c] * kernel[center+rr];
                   kernelsum += kernel[center+rr];
                }
            }
            smoothImg[index] = sum/kernelsum;
            index++;
        }
    }}
}

