#include "SADsp.h"
#include "SAScienceDefine.h"
#include "fftw3.h"
#include <math.h>
#include <memory>
#include <memory.h>
#include <array>

SA::SADsp::SADsp()
{

}

SA::SADsp::~SADsp()
{

}

///
/// \brief 判断数值n是否是2的基次
///
/// 算法来至sp++3.0
/// \param n 数值
/// \return
///
bool SA::SADsp::isPower2( int n )
{
    int bits = 0;
    while( n )
    {
        bits += n & 1;
        n >>= 1;
    }
    return ( bits == 1 );
}

///
/// \brief 对输入数据进行加窗处理
/// \param x 输入的数据
/// \param n 数据长度
/// \param windowflag 窗的类型
///
/// WINDOWFLAG=1     no window or rectangle window
/// WINDOWFLAG=2     hanning window
/// WINDOWFLAG=3     hamming window
/// WINDOWFLAG=4     blackman window
/// WINDOWFLAG=5     bartlett window
///
void SA::SADsp::windowed(double *x, size_t n, WindowType windowflag )
{
    double  multiplier;
    size_t i;

    switch(windowflag)
    {
    case WindowRect:
        break;
    case WindowHanning://hanning window
        for(i=0;i<n;++i,++x)
        {
            multiplier=(double)(0.5-0.5*cos(PI2*i/(n-1)));
            (*x) *= multiplier;
        }
        break;
    case WindowHamming://hamming window
        for(i=0;i<n;++i,++x)
        {
            multiplier=(double)(0.54-0.46*cos(PI2*i/(n-1)));
            (*x) *= multiplier;
        }
        break;
    case WindowBlackman://blackman window
        for(i=0;i<n;++i,++x)
        {
            multiplier=(double)(0.42-0.5*cos(PI2*i/(n-1))+0.08*cos(4*PI*i/(n-1)));
            (*x) *= multiplier;
        }
        break;
    case WindowBartlett://bartlett window -- 不支持back_insert_iterator
        for(i=0;i<(n+1)/2;i++)
        {
            multiplier = (double)(2*(double)i)/((double)n-1);
            x[i]*=multiplier;
            x[n-1-i]*=multiplier;
        }
        break;
    }
}

///
/// \brief 快速傅里叶变换，属于ComplexToComplex,只要pImageData 全部等于0既是DFT
/// \param pRealData 实数部分,同时作为实数的输入，变换完后会作为变换结果的实部
/// \param pImageData 虚数部分,同时作为虚数的输入，变换完后会作为变换结果的虚部
/// \param nNumCount 数组长度
///
bool SA::SADsp::fft(double* pRealData,double* pImageData,int nNumCount)
{
    int i(0);
    fftw_complex *pOutPut;
    fftw_complex *pInPut;
    pOutPut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nNumCount);
    if(nullptr == pOutPut)
    {
        fftw_free (pOutPut);
        return false;
    }
    pInPut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nNumCount);
    if(nullptr == pInPut)
    {
        fftw_free (pInPut);
        return false;
    }
    //转移数据
    for (i=0;i<nNumCount;i++)
    {
        pInPut[i][0] = pRealData[i];
        pInPut[i][1] = pImageData[i];
    }
    //创建方案
    fftw_plan fftplan = fftw_plan_dft_1d(nNumCount,pInPut,pOutPut,FFTW_FORWARD,FFTW_ESTIMATE);
    if(nullptr == fftplan)
    {
        fftw_free (pOutPut);
        fftw_free (pInPut);
        return false;
    }
    //执行
    fftw_execute( fftplan );
    //数据转移回去
    for (i=0;i<nNumCount;i++)
    {
        pRealData[i] = pOutPut[i][0];
        pImageData[i] = pOutPut[i][1];
    }
    fftw_destroy_plan( fftplan );
    fftw_free(pOutPut );//fftw_destroy_plan之后不调用fftw_free也没检测到内存泄露，可能fftw里面用的是智能指针,或者destroy时就释放了
    fftw_free(pInPut );
    return true;
}


///
/// \brief 傅里叶逆变换
/// \param pRealData 实数部分,同时作为实数的输入，变换完后会作为变换结果的实部
/// \param pImageData 虚数部分,同时作为虚数的输入，变换完后会作为变换结果的虚部
/// \param nNumCount 数组长度
///
bool SA::SADsp::ifft(double* pRealData,double* pImageData,int nNumCount)
{
    int i(0);
    fftw_complex *pOutPut;
    fftw_complex *pInPut;
    pOutPut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nNumCount);
    if(nullptr == pOutPut)
    {
        fftw_free (pOutPut);
        return false;
    }
    pInPut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nNumCount);
    if(nullptr == pInPut)
    {
        fftw_free (pInPut);
        return false;
    }
    //转移数据
    for (i=0;i<nNumCount;i++)
    {
        pInPut[i][0] = pRealData[i];
        pInPut[i][1] = pImageData[i];
    }
    //创建方案
    fftw_plan fftplan = fftw_plan_dft_1d(nNumCount,pInPut,pOutPut,FFTW_BACKWARD,FFTW_ESTIMATE);
    if(nullptr == fftplan)
    {
        fftw_free (pOutPut);
        fftw_free (pInPut);
        return false;
    }
    //执行
    fftw_execute( fftplan );
    //数据转移回去--
    //由于fftw的逆傅里叶变换都是大了nNumCount倍，所以是要除回去
    for (i=0;i<nNumCount;i++)
    {
        pRealData[i] = pOutPut[i][0]/nNumCount;
        pImageData[i] = pOutPut[i][1]/nNumCount;
    }
    fftw_destroy_plan( fftplan );
    fftw_free(pOutPut );//fftw_destroy_plan之后不调用fftw_free也没检测到内存泄露，可能fftw里面用的是智能指针,或者destroy时就释放了
    fftw_free(pInPut );
    return true;
}

//=============================================================================
//实傅里叶变换，实数变换后为复数

///
/// \brief rfft 可指定fft的长度，若nfftSize>nNumCount , 则nfftSize比nNumCount多余的数据补0
/// \param pOrignData 原始数据指针
/// \param pRealData 实数数据指针输出 must be larger or equal nfftSize
/// \param pImageData 虚部数据指针输出 must be larger or equal nfftSize
/// \param nNumCount 原始数据长度
/// \param nfftSize 傅里叶变换长度
///
bool SA::SADsp::rfft(const double* pOrignData, double* pRealData, double* pImageData, int nNumCount, int nfftSize)
{
    int i(0);
    std::vector<double> pData;
    pData.resize (nfftSize,0);
    if(nfftSize < nNumCount)
    {
        std::copy(pOrignData,pOrignData+nfftSize,pData.data ());//复制数据到数组中
    }
    else
    {
        std::copy(pOrignData,pOrignData+nNumCount,pData.data ());//复制数据到数组中
    }


    //傅里叶变化方案
    fftw_complex *pOutPut;
    fftw_plan fftplan = nullptr;
    //为复数分配内存
    pOutPut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nfftSize);
    if(nullptr == pOutPut)
    {
        fftw_free (pOutPut);
        return false;
    }
    //实数到复数的傅里叶变化
    fftplan = fftw_plan_dft_r2c_1d(nfftSize,pData.data (),pOutPut,FFTW_ESTIMATE);
    if(NULL == fftplan)
    {
        fftw_free (pOutPut);
        return false;
    }
    //执行
    fftw_execute( fftplan );
    //转移数据
    for (i=0;i<nfftSize;++i,++pRealData,++pImageData)
    {
        *pRealData = pOutPut[i][0];//为了适应back_insert_iterator
        *pImageData = pOutPut[i][1];
    }
    //释放内存
    fftw_free (pOutPut);
    fftw_destroy_plan( fftplan );
    return true;
}

size_t SA::SADsp::getFFTRealDataCount(size_t fftSize)
{
    return (unsigned int)floor((double)(fftSize/2));
}
