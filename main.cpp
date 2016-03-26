#include<opencv2/opencv.hpp>
#include<iostream>
#include"ReadCfg.h"
using namespace std;
using namespace cv;
int main()
{
    Mat image;
    cout<<ReadCfg::data.size()<<endl;
    // image = imread("img/lena.jpg", 1);
    image = imread(ReadCfg::data[0].path, 1);
    if(!image.data)
    {
        cout<<"error!"<<endl;
        return -1;
    }
    namedWindow("Lena.jpg", WINDOW_AUTOSIZE);
    imshow("Lena.jpg", image);
    waitKey(0);
}