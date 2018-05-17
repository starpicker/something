#include "Saliency.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <tuple>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;
using std::make_tuple;
using std::find;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    // Assume we already have an unsigned integer buffer inputImg of
    // inputWidth and inputHeight (in row-major order).
    // Each unsigned integer has 32 bits and contains pixel data in ARGB
    // format. I.e. From left to right, the first 8 bits contain alpha
    // channel value and are not used in our case. The next 8 bits
    // contain R channel value; the next 8 bits contain G channel value;
    // the last 8 bits contain the B channel value.
    //
    // Now create a Saliency object and call the GetSaliencyMap function on it.
    // salmap is a floating point output (in row major order)
    //
    
    if(argc < 3)
    {
        printf("usage: %d widht height [doublemap]\n", argv[0]);
        return 0;
    }
    
    int inputWidth = atoi(argv[1]);
    int inputHeight = atoi(argv[2]);
    int doublemap = 1;
    if(argc == 4)
        doublemap = atoi(argv[3]);
    
    char temp[inputWidth*inputHeight*4];
    string filename = std::to_string(inputWidth) + "x" + std::to_string(inputHeight) + ".rgb32";
    FILE* fp = fopen(filename.c_str(), "r");
    fread(temp, inputWidth*inputHeight*4, 1, fp);

    Saliency sal;
    vector<unsigned int>inputImg;
    vector<unsigned char> salmapChar;
    vector<double> salmapDouble;


    for(int h = 0; h < inputHeight; h++)
    {
        unsigned char* p = (unsigned char*)temp + h *
                            inputWidth*4;

        for(int w = 0; w < inputWidth; w++)
        {
            unsigned int t = 0;

            //rgba
            t+=*p++;
            t<<=8;
            t+=*p++;
            t<<=8;
            t+=*p++;
            p++;

            inputImg.push_back(t);
        }
    }

    
    if(doublemap == 0)
        sal.template GetSaliencyMap<unsigned char>(inputImg, inputWidth, inputHeight, salmapChar);
    else
        sal.template GetSaliencyMap<double>(inputImg, inputWidth, inputHeight, salmapDouble);

    char temp1[inputWidth*inputHeight];
    
    if(doublemap == 1)
    for(int i = 0 ; i < salmapDouble.size(); i++)
    {
        temp1[i] = (char)salmapDouble[i];
    }

    filename = std::to_string(inputWidth) + "x" + std::to_string(inputHeight) + ".grey";
    FILE* fp1 = fopen(filename.c_str(), "w");

    if(doublemap == 0)
        fwrite(&salmapChar[0], salmapChar.size(), 1, fp1);
    else
        fwrite(temp1, salmapDouble.size(), 1, fp1);

    fclose(fp1);
    
    return 0;
}
