#include"Sift.h"
#include<iostream>
using namespace std;
#include<cmath>

const double TWOSQARTPI = 0.159154943;

//获取一维高斯的值
inline double getGaussion(float k, int x)
{
    return (SQARTPI*(1.0/k)*exp( -(x*x)/(2.0*k*k) ) );
}
inline double getGaussion(float k, int x, int y)
{
    double kk = k*k;
    return (TWOSQARTPI*(1.0/kk)*exp( -(x*x +y*y)/2.0/kk ) );
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
    if(i<0 || j<0 || i>=rows || j >= cols) return 0;
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
    x = new double[length];
    
    //生成对应的模板
    int h = length>>1;
    double sum = 0;
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
            double sum[3] = {0.0, 0.0, 0.0};
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



//-----------GuassTwoDimen--------
GaussTwoDimen::GaussTwoDimen(float keno)
{    
    this->keno = keno;
    length = static_cast<int>(6*keno+1);
    if( (length&1)==0 ) ++length;
    x = new double[length*length];
    //x[length][length]
    //x[i][j] = i*length+j
    int h = length>>1;
    double sum = 0;
    
    for(int i=0;i<length;++i)
    {
        for(int j=0;j<length; ++j)
        {
            x[i*length +j] = getGaussion(keno, i-h, j-h);
            sum += x[i*length+j];
        }
    }
    
    //归一化
    for(int i=0;i<length; ++i)
    {
        for(int j=0;j<length; ++j)
            x[i*length+j] /= sum;
    }
    
    //----------------
    // for(int i=0;i<length; ++i)
    // {
    //     for(int j=0;j<length; ++j)
    //         cout<<x[i*length+j]<<"  ";
    //     cout<<endl;
    // }
}

GaussTwoDimen::~GaussTwoDimen()
{
    delete[] x;
}

void GaussTwoDimen::working(Mat& target, const Mat& source)
{
    xjMat tar(target.data, source.rows, source.cols, source.channels());
    const xjMat sou(source);
    return working(tar, source);
}
void GaussTwoDimen::working(xjMat& target, const xjMat& source)
{
    for(int i=0;i<source.rows;++i)
    {
        for(int j=0;j<source.cols; ++j)
        {
            double sum[3] = {0.0, 0.0, 0.0};
            int h = length>>1;
            for(int a = 0; a<length; ++a)
            {
                for(int y=0;y<length; ++y)
                {
                    sum[R] += source.get(i+a-h, j+y-h, R)*x[a*length+y];
                    sum[G] += source.get(i+a-h, j+y-h, G)*x[a*length+y];
                    sum[B] += source.get(i+a-h, j+y-h, B)*x[a*length+y];
                }
            }
            target.set(i, j, R) = static_cast<unsigned char>(sum[R]);
            target.set(i, j, G) = static_cast<unsigned char>(sum[G]);
            target.set(i, j, B) = static_cast<unsigned char>(sum[B]);
        }
    }
}

//-----------SIFT--------------
void SIFT(const Mat& image)
{
    Gaussy* g = new GaussTwoDimen(0.6);
    //Gaussy* g = new GaussSeparate(5.0);
    Mat temp = image.clone();
    g->working(temp, image);
    namedWindow("LenaGau.jpg", WINDOW_AUTOSIZE);
    imshow("LenaGau.jpg", temp);
    waitKey(0);
    imwrite("img/Lena的二维高斯模糊_0.6.jpg", temp);
    delete g;
}