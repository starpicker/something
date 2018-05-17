// Saliency.h: interface for the Saliency class.
//
//////////////////////////////////////////////////////////////////////
//===========================================================================
//    Copyright (c) 2009 Radhakrishna Achanta [EPFL]
//===========================================================================

#if !defined(_SALIENCY_H_INCLUDED_)
#define _SALIENCY_H_INCLUDED_

#include <vector>
#include <cfloat>
#include <climits>
using namespace std;


class Saliency
{
public:
    Saliency();
    virtual ~Saliency();

public:

//template <typename T>
//void GetSaliencyMap(
        //const vector<unsigned int>&                inputimg,//INPUT: ARGB buffer in row-major order
        //const int&                        width,
        //const int&                        height,
        //vector<T>&                    salmap,//OUTPUT: Floating point buffer in row-major order
        //const bool&                        normalizeflag = true);//false if normalization is not needed


//===========================================================================
///    GetSaliencyMap
///
/// Outputs a saliency map with a value assigned per pixel. The values are
/// normalized in the interval [0,255] if normflag is set true (default value).
//===========================================================================
template <typename T>
void GetSaliencyMap(
    const vector<unsigned int>&        inputimg,
    const int&                        width,
    const int&                        height,
    vector<T>&                    salmap,
    const bool&                        normflag = true)
{
    int sz = width*height;
    salmap.clear();
    salmap.resize(sz);

    vector<double> lvec(0), avec(0), bvec(0);
    RGB2LAB(inputimg, lvec, avec, bvec);
    //--------------------------
    // Obtain Lab average values
    //--------------------------
    double avgl(0), avga(0), avgb(0);
    {for( int i = 0; i < sz; i++ )
    {
        avgl += lvec[i];
        avga += avec[i];
        avgb += bvec[i];
    }}
    avgl /= sz;
    avga /= sz;
    avgb /= sz;

    vector<double> slvec(0), savec(0), sbvec(0);

    //----------------------------------------------------
    // The kernel can be [1 2 1] or [1 4 6 4 1] as needed.
    // The code below show usage of [1 2 1] kernel.
    //----------------------------------------------------
    vector<double> kernel(0);
    kernel.push_back(1.0);
    kernel.push_back(2.0);
    kernel.push_back(1.0);

    GaussianSmooth(lvec, width, height, kernel, slvec);
    GaussianSmooth(avec, width, height, kernel, savec);
    GaussianSmooth(bvec, width, height, kernel, sbvec);

    {for( int i = 0; i < sz; i++ )
    {
        salmap[i] = (slvec[i]-avgl)*(slvec[i]-avgl) +
                    (savec[i]-avga)*(savec[i]-avga) +
                    (sbvec[i]-avgb)*(sbvec[i]-avgb);
    }}

    if( true == normflag )
    {
        vector<T> normalized;
        Normalize(salmap, width, height, normalized);
        swap(salmap, normalized);
    }
}
private:

    void RGB2LAB(
        const vector<unsigned int>&                ubuff,
        vector<double>&                    lvec,
        vector<double>&                    avec,
        vector<double>&                    bvec);

    void GaussianSmooth(
        const vector<double>&            inputImg,
        const int&                        width,
        const int&                        height,
        const vector<double>&            kernel,
        vector<double>&                    smoothImg);

    template <typename T>
    T getTypeMaxValue(void)
    {
        if(std::is_same<typename std::decay<T>, int>::value)
            return INT_MAX;
        else if(std::is_same<typename std::decay<T>, double>::value)
            return DBL_MAX;
        else if(std::is_same<typename std::decay<T>, unsigned char>::value)
            return UCHAR_MAX;
        else
            return DBL_MAX;
    }
    //==============================================================================
    ///    Normalize
    //==============================================================================
    template <typename T>
    void Normalize(
        const vector<T>&            input,
        const int&                        width,
        const int&                        height,
        vector<T>&                    output,
        const int&                        normrange = 255)
    {
        T maxval(0);
        //T minval(DBL_MAX);
        T minval(getTypeMaxValue<T>());
        {int i(0);
        for( int y = 0; y < height; y++ )
        {
            for( int x = 0; x < width; x++ )
            {
                if( maxval < input[i] ) maxval = input[i];
                if( minval > input[i] ) minval = input[i];
                i++;
            }
        }}
        T range = maxval-minval;
        if( 0 == range ) range = 1;
        int i(0);
        output.clear();
        output.resize(width*height);
        for( int y = 0; y < height; y++ )
        {
            for( int x = 0; x < width; x++ )
            {
                output[i] = ((normrange*(input[i]-minval))/range);
                i++;
            }
        }
    }

};

#endif // !defined(_SALIENCY_H_INCLUDED_)
