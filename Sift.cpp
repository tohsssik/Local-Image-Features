#include"Sift.h"
#include<iostream>
using namespace std;
#include<cmath>

//获取一维高斯的值
inline float getGaussion(float k, int x)
{
    return static_cast<float>(SQARTPI*(1/k)*exp( -(x*x)/(2*k*k) ) );
}

//--------------xjMat----------------------

xjMat::xjMat(unsigned char* d, int r, int c, int inc):
    data(d), rows(r), cols(c), channels(inc)
{
    
}


xjMat::xjMat(const Mat& M)
{
    if(!M.isContinuous())
    {
        cerr<<"error, the matrix's data is not continuous!"<<endl;
        exit(1);
    }
    data = M.data;
    rows = M.rows;
    cols = M.cols;
    channels = M.channels();
}

/*inline*/ unsigned char xjMat::get(int i, int j, int k) const
{
    //为了追求效率，这里不进行下标检查
    if(i<0 || j<0) return 0;
    else return *(data + i*cols*channels + j*channels +k);
}


/*inline*/ unsigned char& xjMat::set(int i, int j, int k)
{
    return *(data + i*cols*channels + j*channels +k);
}





//------------GaussSeparate-------------------

GaussSeparate::GaussSeparate(float keno)
{
    this->keno = keno;
    length = static_cast<int>(6*keno+1);
    if( (length&1) == 0) ++length;
    x = new float[length];
    
    //生成对应的模板
    int h = length>>1;
    float sum = 0;
    for(int i=0;i<=h; ++i)
    {
        x[i] = getGaussion(keno, i-h);
        sum += x[i] + x[i];
    }
    sum -= x[h];
    sum += sum;
    //归一化
    for(int i=0; i<=h; ++i)
    {
        x[i] = x[i] / sum;
        x[length-1-i] = x[i];
    }
}

void GaussSeparate::working(Mat& M, const Mat& source)
{
    xjMat target(M.data, source.rows, source.cols, source.channels());
    const xjMat s(source);
    return working(target, s);
}
void GaussSeparate::working(xjMat& target, const xjMat& source)
{
    for(int i=0;i<source.rows;++i)
    {
        for(int j=0; j<source.cols; ++j)
        {
            float sum[3] = {0.0, 0.0, 0.0};
            int h = length>>1;
            for(int k=0; k<length; ++k)
            {
                sum[R] += source.get(i, j+k-h, R)*x[k];
                sum[R] += source.get(i+k-h, j, R)*x[k];
                sum[G] += source.get(i, j+k-h, G)*x[k];
                sum[G] += source.get(i+k-h, j, G)*x[k];
                sum[B] += source.get(i, j+k-h, B)*x[k];
                sum[B] += source.get(i+k-h, j, B)*x[k];
            }
            target.set(i,j, R) = static_cast<unsigned char>(sum[R]);
            target.set(i,j, G) = static_cast<unsigned char>(sum[G]);
            target.set(i,j, B) = static_cast<unsigned char>(sum[B]);
        }
    }
}

GaussSeparate::~GaussSeparate()
{
    delete[] x;
}


//-----------SIFT--------------
void SIFT(const Mat& image)
{
    GaussSeparate g(0.6);
    Mat temp(image);
    g.working(temp, image);
    namedWindow("LenaGau.jpg", WINDOW_AUTOSIZE);
    imshow("Lena.jpg", temp);
    waitKey(0);
}