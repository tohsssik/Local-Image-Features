#ifndef _SIFT
#define _SIFT

#define SQARTPI 0.39894228 //根号2 pi
#include<opencv2/opencv.hpp>
using namespace cv;
enum Channels{B, G, R};

//自定义的矩阵
struct xjMat
{
    unsigned char* data;
    int rows, cols, channels;
    public:
    xjMat(const Mat& M);
    xjMat(unsigned char* ,int inrows, int incols, int inc);
    inline unsigned char get(int i, int j, int k) const;
    inline unsigned char& set(int i, int j, int);
};

class Gaussy
{
    protected:
    
    float keno;
    int length;
    
    public:
    Gaussy(){}
    virtual ~Gaussy(){}
    virtual void working(Mat& M, const Mat& source) = 0;
    virtual void working(xjMat&, const xjMat& ) = 0;
};


//分离高斯模糊算子类
class GaussSeparate:public Gaussy
{
    //分别代表二维方向的模糊
    //范围是(6keno+1)*(6keno+1);
    double* x;
    public:
    GaussSeparate(float keno);
    ~GaussSeparate();
    
    //对图像进行高斯模糊，结果放在M 中
    void working(Mat& M, const Mat& source);
    void working(xjMat& M, const xjMat& source);
};

//二维高斯模糊算子类
class GaussTwoDimen:public Gaussy
{
    double* x;
    
    public:
    GaussTwoDimen(float keno);
    ~GaussTwoDimen();
    
    //对图像进行高斯模糊，结果放在M 中
    void working(Mat& M, const Mat& source);
    void working(xjMat& M, const xjMat& source);
};



void SIFT(const Mat& image);

#endif