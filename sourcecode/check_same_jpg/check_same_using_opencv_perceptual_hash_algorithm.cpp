#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "usage: "  << argv[0] << " 1.jpg 2.jpg" << endl;

        return 0;
    }

    string strSrcImageName = argv[1];
    string strDstImageName = argv[2];

    cv::Mat matSrc, matSrcRZ;

    matSrc = cv::imread(strSrcImageName, CV_LOAD_IMAGE_COLOR);
    CV_Assert(matSrc.channels() == 3);

    cv::Mat matDst, matDstRZ;
    matDst = cv::imread(strDstImageName, CV_LOAD_IMAGE_COLOR);
    CV_Assert(matDst.channels() == 3);

    cv::resize(matSrc, matSrcRZ, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
    cv::resize(matDst, matDstRZ, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

    cv::cvtColor(matSrcRZ, matSrcRZ, CV_BGR2GRAY);
    cv::cvtColor(matDstRZ, matDstRZ, CV_BGR2GRAY);

    int iAvg1 = 0, iAvg2 = 0;
    int arr1[64], arr2[64];

    for (int i = 0; i < 8; i++)
    {
        uchar* data1 = matSrcRZ.ptr<uchar>(i);
        uchar* data2 = matDstRZ.ptr<uchar>(i);

        int tmp = i * 8;

        for (int j = 0; j < 8; j++)
        {
            int tmp1 = tmp + j;

            arr1[tmp1] = data1[j] / 4 * 4;
            arr2[tmp1] = data2[j] / 4 * 4;

            iAvg1 += arr1[tmp1];
            iAvg2 += arr2[tmp1];
        }
    }

    iAvg1 /= 64;
    iAvg2 /= 64;

    for (int i = 0; i < 64; i++)
    {
        arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
        arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
    }

    int iDiffNum = 0;

    for (int i = 0; i < 64; i++)
        if (arr1[i] != arr2[i])
            ++iDiffNum;

    cout<<"iDiffNum = "<<iDiffNum<<endl;

    //if (iDiffNum <= 5)
        //cout<<"two images are very similar!"<<endl;
    //else if (iDiffNum > 10)
        //cout<<"they are two different images!"<<endl;
    //else
        //cout<<"two image are somewhat similar!"<<endl;

    //getchar();
    return 0;
}
