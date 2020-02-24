//////////////////////////////////////////////////////////////////////////
///创建时间-2012-4-1
///创件人：尘中远
///库名：数字信号处理的数学方法库
///参考库：SP++、itpp-4.2
///引用库：fftw
///必须包含fftw3.h，libfftw3-3.lib，libfftw3-3.dll
//////////////////////////////////////////////////////////////////////////
//verson 1.1
//////////////////////////////////////////////////////////////////////////
#ifndef DSP_MATH_H
#define DSP_MATH_H
#include <stddef.h>
#include <czyMath.h>


namespace SA {
namespace Math {
class DSP
{
public:
    DSP(void);
    ~DSP(void);
public:
    //辅助函数
    //判断是否是2的基次
    //算法来着sp++3.0
    static bool isPower2( int n );
public:
    enum WindowType{
        WindowRect///< 矩形窗
        ,WindowHanning///< 汉宁窗
        ,WindowHamming///< 汉明窗
        ,WindowBlackman///< 巴克曼窗
        ,WindowBartlett///< 巴特利窗
    };

    ///
    /// \brief 获取当前数字的下一个2^n基数的n
    /// \param n
    /// \return
    ///
    template<typename INT_LIKE>
    static INT_LIKE nextPow2(INT_LIKE n)
    {
        return ceil(log2(n));
    }
    ///
    /// \brief 获取当前数字的下一个2^n数
    /// \param n
    /// \return
    ///
    template<typename INT_LIKE>
    static INT_LIKE nextPow2Value(INT_LIKE val)
    {
        INT_LIKE n = nextPow2(val);
        return pow(2,(int)n);
    }

    static void windowed(double *x, size_t n, WindowType windowflag);

    ///
    /// \brief 信号加窗
    /// \param _begin 信号的开始迭代器
    /// \param _end 信号的结束迭代器
    /// \param windowflag 窗
    /// \see windowed
    ///
    template<typename IT>
    static void windowed(IT _begin,IT _end,WindowType windowflag)
    {
        size_t size = std::distance(_begin,_end);
        windowed(_begin,size,windowflag);
    }

    //==========================================================================
    //傅里叶变换相关
    //////////////////////////////////////////////////////////////////////////
    //快速傅里叶变换//长度无要求
    static bool fft(double* pRealData,double* pImageData,int nNumCount);
    //逆傅里叶变换//长度无要求
    static bool ifft(double* pRealData,double* pImageData,int nNumCount);
    //实傅里叶变换，实数变换后为复数的重载
    static bool rfft(const double* pOrignData,double* pRealData,double* pImageData,int nNumCount,int nfftSize);
    //C++的方式调用rfft
    template<typename IT_INPUT,typename IT_OUTPUT>
    static bool rfft(IT_INPUT _begin,IT_INPUT _end,IT_OUTPUT _realBegin,IT_OUTPUT _imgBegin,int nfftSize)
    {
        size_t size = std::distance(_begin,_end);
        return rfft(_begin,_realBegin,_imgBegin,size,nfftSize);
    }

    ///
    /// \brief 去均值
    ///
    template<typename T>
    static void detrend(T _begin,T _end)
    {
        double m = SA::Math::mean(_begin,_end);
        for(T i=_begin;i!=_end;++i)
        {
            (*i) = (*i)-m;
        }
    }
    ///
    /// \brief 求幅值，复数取模操作
    /// \param real 实部
    /// \param imag 虚部
    /// \return 摸
    ///
    static double magnitude(double real,double imag)
    {
        return sqrt( (real * real)+( imag * imag ));
    }

    ///
    /// \brief 求序列的20log - 转换为db标示
    /// \param _begin 序列的开始迭代器
    /// \param _end 序列的结束迭代器
    ///
    template<typename IT>
    static void db20log10(IT _begin,IT _end)
    {
        for(auto i=_begin;i!=_end;++i)
        {
            if(*i<(1e-20))
                *i=(1e-20);//限定最小值
            *i = 20.0 * log(*i);
        }
    }

    static size_t getFFTRealDataCount(size_t fftSize);
public://频谱分析
    ///
    /// \brief 频谱的幅值标示方法，Magnitude代表幅度谱，此时计算的幅值是(real^2+imag^2)^0.5,Amplitude是真实幅值，计算的结果是
    /// 2*Magnitude/n
    ///
    /// 参考:http://www.originlab.com/doc/Origin-Help/FFT1-Algorithm
    ///
    enum SpectrumType{
        Magnitude///< 幅度谱
        ,MagnitudeDB///<分贝的幅度谱
        ,Amplitude///< 幅值谱
        ,AmplitudeDB///<分贝的幅值谱
    };
    ///
    /// \brief 频谱分析
    /// \param _begin 输入波形开始迭代器
    /// \param _end 输入波形结束迭代器
    /// \param _freBegin 频率结果开始迭代器，建议使用back_insert_iterator
    /// \param _magBegin 幅值结果开始迭代器，建议使用back_insert_iterator
    /// \param sampleRate 采样率
    /// \param fftSize fft长度,-1小于或等于0时，将会用波形长度的下个2次幂来作为fft的长度
    /// \param type 频谱幅值标示方法，type==Magnitude使用幅度谱，type==Amplitude使用幅值谱
    /// \return 频谱的长度,若傅里叶变化未能成功，返回-1；
    /// \see SpectrumType rfft
    ///
    template<typename DOUBLE_LIKE_PTR1,typename DOUBLE_LIKE_PTR2>
    static int spectrum(DOUBLE_LIKE_PTR1 pOrignData
                        , DOUBLE_LIKE_PTR2 pFre
                        , DOUBLE_LIKE_PTR2 pMag
                        , size_t size
                        , double sampleRate
                        , int nfftSize = 0
                        ,SpectrumType type = Amplitude)
    {
        if(nfftSize <= 0)
        {
            nfftSize = pow(2,nextPow2(size));
        }
        const size_t nRealData = getFFTRealDataCount(nfftSize);
        std::vector<double> real,imag;  
        real.resize (nfftSize,0);
        imag.resize (nfftSize,0);
        if(!rfft(pOrignData,real.data (),imag.data (),size,nfftSize))//pMag作为实部，pFre暂时作为虚部
            return -1;
        //获取幅值
        //if放在外头提高效率

        dealMagnitude(real.data (),imag.data (),pMag,nfftSize,type,size);
        //获取频率
        for (size_t i=0;i<nRealData;++i,++pFre)
        {
            *pFre = ((double(i)*sampleRate))/((double)nfftSize);
        }
        return nRealData;
    }

    ///
    /// \brief 频谱分析
    /// \param _begin 输入波形开始迭代器
    /// \param _end 输入波形结束迭代器
    /// \param _freBegin 频率结果开始迭代器，建议使用back_insert_iterator
    /// \param _magBegin 幅值结果开始迭代器，建议使用back_insert_iterator
    /// \param sampleRate 采样率
    /// \param fftSize fft长度
    /// \return 频谱的长度
    ///
    template<typename IT_INPUT,typename IT_OUTPUT>
    static int spectrum(IT_INPUT _begin,IT_INPUT _end,IT_OUTPUT _freBegin,IT_OUTPUT _magBegin
                        , double sampleRate,int fftSize,SpectrumType type = Amplitude)
    {
        size_t size = std::distance(_begin,_end);
        if(size<2)
            return -1;
        return spectrum(_begin,_freBegin,_magBegin,size,sampleRate,fftSize,type);
    }
    ///
    /// \brief 处理幅值
    /// \param real 实数部
    /// \param imag 虚数部
    /// \param pMag 幅值 - 输出
    /// \param dataSize real,imag,pMag对应的长度，
    /// \param type 幅值的类型
    /// \param realDataSize 实际数据的长度，只有type == Amplitude，type== AmplitudeDB时起作用
    ///
    template<typename DOUBLE_LIKE_PTR,typename DOUBLE_LIKE_PTR2>
    static void dealMagnitude(DOUBLE_LIKE_PTR real,DOUBLE_LIKE_PTR imag,DOUBLE_LIKE_PTR2 pMag,size_t dataSize
                              ,SpectrumType type = Amplitude
                              ,size_t waveDataSize = 0)
    {
        const size_t nRealData = getFFTRealDataCount(dataSize);
        if (Magnitude == type)//幅度谱
        {
            for (size_t i=0;i<nRealData;++i,++pMag)
            {
                *pMag =  magnitude(real[i],imag[i]);//支撑back_insert_iterator
            }
        }
        else if (MagnitudeDB == type)//分贝标示的幅度谱
        {
            for (size_t i=0;i<nRealData;++i,++pMag)
            {
                *pMag =  20*log(magnitude(real[i],imag[i]));//支撑back_insert_iterator
            }
        }
        else//幅值谱
        {
            if(0 == waveDataSize)
                waveDataSize = dataSize;
            double temp = 2.0 / double(waveDataSize);
            const size_t count = nRealData-1;
            if (Amplitude == type)
            {
                //幅值谱的头尾需要处理
                *pMag =  magnitude(real[0],imag[0]) / double(waveDataSize);//第一个幅值
                ++pMag;
                for (size_t i=1;i<count;++i,++pMag)
                {
                    *pMag =  magnitude(real[i],imag[i]) * temp;//转变为幅值谱
                }
                //最后一个幅值
                ++pMag;
                *pMag =  magnitude(real[count],imag[count]) / double(waveDataSize);//第一个幅值
            }
            else if(AmplitudeDB == type)//幅值谱
            {
                //幅值谱的头尾需要处理
                *pMag =  20.0*log(magnitude(real[0],imag[0]) / double(waveDataSize));//第一个幅值
                ++pMag;
                for (size_t i=1;i<count;++i,++pMag)
                {
                    *pMag =  20.0*log(magnitude(real[i],imag[i]) * temp);//转变为幅值谱
                }
                //最后一个幅值
                ++pMag;
                *pMag =  20.0*log(magnitude(real[count],imag[count]) / double(waveDataSize));//第一个幅值
            }
        }

    }





public://功率谱分析
    ///
    /// \brief 功率谱估计的方法，功率谱估计提供了3种估计方法
    ///
    /// 参考:http://www.originlab.com/doc/Origin-Help/FFT1-Algorithm
    ///
    enum PowerDensityWay{
        MSA///< mean squared amplitude 平均振幅平方法，计算方法为(real^2+imag^2)/n^2
        ,SSA///< sum squared amplitude 和振幅平方法,计算方法为(real^2+imag^2)/n
        ,TISA///< time-integral squared amplitude 时间积分振幅的平方dT*(real^2+imag^2)/n
    };


    template<typename DOUBLE_LIKE_PTR1,typename DOUBLE_LIKE_PTR2>
    static int powerSpectrum(DOUBLE_LIKE_PTR1 pOrignData
                        , DOUBLE_LIKE_PTR2 pFre
                        , DOUBLE_LIKE_PTR2 pMag
                        , size_t size
                        , double sampleRate
                        , int nfftSize = 0
                        ,PowerDensityWay type = MSA
                        ,double samplingInterval = 0.0
                        )
    {
        if(nfftSize <= 0)
        {
            nfftSize = pow(2,nextPow2(size));
        }
        const size_t nRealData = (unsigned int)floor((double)(nfftSize/2));
        std::vector<double> real,imag;
        real.resize (nfftSize,0);
        imag.resize (nfftSize,0);
        if(!rfft(pOrignData,real.data (),imag.data (),size,nfftSize))//pMag作为实部，pFre暂时作为虚部
            return -1;
        if(MSA == type)
        {//平均振幅平方法，计算方法为(real^2+imag^2)/n^2
            _dealpowerSpectrumMSA(pMag,real.begin (),real.end (),imag.begin ());
        }
        else if(SSA == type)
        {
            _dealpowerSpectrumSSA(pMag,real.begin (),real.end (),imag.begin ());
        }
        else if(TISA == type)
        {
            _dealpowerSpectrumTISA(pMag,real.begin (),real.end (),imag.begin (),samplingInterval);
        }
        //获取频率
        for (size_t i=0;i<nRealData;++i,++pFre)
        {
            *pFre = ((double(i)*sampleRate))/((double)nfftSize);
        }
        return nRealData;
    }
    ///
    /// \brief 功率谱分析
    /// \param _begin 输入波形开始迭代器
    /// \param _end 输入波形结束迭代器
    /// \param _freBegin 频率结果开始迭代器，建议使用back_insert_iterator
    /// \param _magBegin 幅值结果开始迭代器，建议使用back_insert_iterator
    /// \param sampleRate 采样率
    /// \param fftSize fft长度
    /// \param type 功率谱的估计方法，分为MSA，SSA，TISA，具体见PowerDensityWay枚举
    /// \param samplingInterval 采样间隔，此参数只有在type==TISA时会起作用，不定义type==TISA无用
    /// \return 频谱的长度
    /// \see PowerDensityWay
    ///
    template<typename IT_INPUT,typename IT_OUTPUT>
    static size_t powerSpectrum(IT_INPUT _begin,IT_INPUT _end,IT_OUTPUT _freBegin,IT_OUTPUT _magBegin
                                , double sampleRate,int fftSize
                                ,PowerDensityWay type = MSA,double samplingInterval = 0.0)
    {
        size_t size = std::distance(_begin,_end);
        if(size<2)
            return -1;
        return powerSpectrum(_begin,_freBegin,_magBegin,size,sampleRate,fftSize,type,samplingInterval);
    }
private:
    ///
    /// \brief 处理功率谱密度估计的平均增幅法
    /// \param pMag 幅值结果输出
    /// \param real_begin 实部输入
    /// \param real_end 实部结束
    /// \param imag_begin 虚部输入
    ///
    template<typename DOUBLE_LIKE_PTR,typename DATA_IT>
    static void _dealpowerSpectrumMSA(DOUBLE_LIKE_PTR pMag,DATA_IT real_begin,DATA_IT real_end,DATA_IT imag_begin)
    {
        double n = std::distance(real_begin,real_end);
        n = pow(n,2);//需要对n进行平方
        auto real = real_begin;
        auto imag = imag_begin;
        while(real!=real_end)
        {
            *pMag = (pow(*real,2)+pow(*imag,2))/n;
            ++real;
            ++imag;
            ++pMag;
        }
    }
    ///
    /// \brief 处理功率谱密度估计的和振幅平方法,计算方法为(real^2+imag^2)/n
    /// \param pMag 幅值结果输出
    /// \param real_begin 实部输入
    /// \param real_end 实部结束
    /// \param imag_begin 虚部输入
    ///
    template<typename DOUBLE_LIKE_PTR,typename DATA_IT>
    static void _dealpowerSpectrumSSA(DOUBLE_LIKE_PTR pMag,DATA_IT real_begin,DATA_IT real_end,DATA_IT imag_begin)
    {
        double n = std::distance(real_begin,real_end);
        //n = pow(n,2);//不需要对n进行平方与_dealpowerSpectrumMSA的区别
        auto real = real_begin;
        auto imag = imag_begin;
        while(real!=real_end)
        {
            *pMag = (pow(*real,2)+pow(*imag,2))/n;
            ++real;
            ++imag;
            ++pMag;
        }
    }
    ///
    /// \brief 处理功率谱密度估计的时间积分振幅的平方dT*(real^2+imag^2)/n
    /// \param pMag 幅值结果输出
    /// \param real_begin 实部输入
    /// \param real_end 实部结束
    /// \param imag_begin 虚部输入
    /// \param samplingInterval 采样间隔
    ///
    template<typename DOUBLE_LIKE_PTR,typename DATA_IT>
    static void _dealpowerSpectrumTISA(DOUBLE_LIKE_PTR pMag,DATA_IT real_begin,DATA_IT real_end,DATA_IT imag_begin,double samplingInterval)
    {
        double n = std::distance(real_begin,real_end);
        auto real = real_begin;
        auto imag = imag_begin;
        while(real!=real_end)
        {
            *pMag = (samplingInterval*(pow(*real,2)+pow(*imag,2)))/n;
            ++real;
            ++imag;
            ++pMag;
        }
    }


};
}
}


#endif

