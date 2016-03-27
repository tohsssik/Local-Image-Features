#include<opencv2/opencv.hpp>
#include<iostream>
#include"ReadCfg.h"
#include"Sift.h"
using namespace std;
using namespace cv;

//程序的初始化函数
void init()
{
    ReadCfg readCfg;
}
int main()
{   
    init(); 
    
    //读入图片，并且做高斯二维模糊
    Mat image;
    image = imread(ReadCfg::data[0].path, 1);
    if(!image.data)
    {
        cout<<"error!"<<endl;
        return -1;
    }
    
    SIFT(image);
    
    namedWindow("Lena.jpg", WINDOW_AUTOSIZE);
    imshow("Lena.jpg", image);
    waitKey(0);
}