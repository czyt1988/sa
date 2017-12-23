#include "sa_fun_dsp.h"

#include <QVector>
#include <iterator>
#include "SAValueManager.h"
#include "SAAbstractDatas.h"
#include "SAVectorDouble.h"
#include "SAVariantDatas.h"
#include <algorithm>

#define TR(str)\
    QApplication::translate("sa_fun_dsp", str, 0)

std::shared_ptr<SAVectorDouble> _setWindow(QVector<double>& y, czy::Math::DSP::WindowType window);
std::shared_ptr<SAVectorPointF> _setWindow(const SAVectorPointF *wave, czy::Math::DSP::WindowType window);
std::shared_ptr<SAVectorPointF> _detrendDirect(const SAVectorPointF *wave);
std::shared_ptr<SAVectorDouble> _detrendDirect(QVector<double>& wave);

std::shared_ptr<SAVectorPointF> _detrendDirect(const SAVectorPointF* wave)
{
    QVector<double> x,y;
    wave->getYs(y);
    wave->getXs(x);
    czy::Math::DSP::detrend(y.begin(),y.end());
    return SAValueManager::makeData<SAVectorPointF>(wave->getName() + "detrendDirect",x,y);
}
std::shared_ptr<SAVectorDouble> _detrendDirect(QVector<double>& wave)
{
    czy::Math::DSP::detrend(wave.begin(),wave.end());
    return SAValueManager::makeData<SAVectorDouble>(wave);
}
///
/// \brief saFun::makeVectorPointF
/// \param x
/// \param y
/// \param xys
///
void saFun::makeVectorPointF(const QVector<double> &x, const QVector<double> &y, QVector<QPointF> &xys)
{
    const int minSize = qMin(x.size(),y.size());
    xys.resize(minSize);
    for(int i=0;i<minSize;++i)
    {
        xys[i] = QPointF(x[i],y[i]);
    }
}
///
/// \brief 去直流
/// \param wave 波形
/// \return 去直流后波形
///
std::shared_ptr<SAAbstractDatas> saFun::detrendDirect(const SAAbstractDatas *wave)
{
    QVector<double> waveData;
    if(SA::VectorPoint == wave->getType())
    {
        std::shared_ptr<SAVectorPointF> res = _detrendDirect(static_cast<const SAVectorPointF*>(wave));
        return SAValueManager::castPointToBase(res);
    }
    else if(SAAbstractDatas::converToDoubleVector(wave,waveData))
    {
        std::shared_ptr<SAVectorDouble> res = _detrendDirect(waveData);
        if(res)
        {
            res->setName(wave->getName() + "detrendDirect");
        }
        return SAValueManager::castPointToBase(res);
    }
    setErrorString(TR("can not conver data to double vector!"));
    return nullptr;
    czy::Math::DSP::detrend(waveData.begin(),waveData.end());

}
///
/// \brief 去直流
/// \param y 波形
///
void saFun::detrendDirect(QVector<double> &y)
{
    czy::Math::DSP::detrend(y.begin(),y.end());
}

///
/// \brief 频谱分析
/// \param wave 波形
/// \param fs 采样率
/// \param fftSize fft长度
/// \param ampType 频谱幅值标示方法，type==Magnitude使用幅度谱，type==Amplitude使用幅值谱
/// \param fre 频率结果
/// \param mag 幅值结果
/// \return output[频率，幅值]
///
std::tuple<std::shared_ptr<SAVectorDouble>,std::shared_ptr<SAVectorDouble> >
saFun::spectrum(const SAAbstractDatas *wave
                                  , double fs
                                  , size_t fftSize
                                  , czy::Math::DSP::SpectrumType ampType)
{
    QVector<double> waveArr;
    if(SA::VectorPoint == wave->getType())
    {
        const SAVectorPointF* pf = static_cast<const SAVectorPointF*>(wave);
        pf->getYs(waveArr);
    }
    else if(!SAAbstractDatas::converToDoubleVector(wave,waveArr))
    {
        setErrorString(TR("can not conver data to double vector!"));
        return std::make_tuple(nullptr,nullptr);
    }
    if(waveArr.size() <= 0)
    {
        setErrorString(TR("wave size is too short!"));
        return std::make_tuple(nullptr,nullptr);
    }
    auto fre = SAValueManager::makeData<SAVectorDouble>();
    auto mag = SAValueManager::makeData<SAVectorDouble>();
    fre->setName(QString("%1-fre").arg(wave->getName()));
    mag->setName(QString("%1-mag").arg(wave->getName()));

    std::back_insert_iterator<SAVectorDouble> freIte(*(fre.get()));
    std::back_insert_iterator<SAVectorDouble> magIte(*(mag.get()));
    int len = czy::Math::DSP::spectrum(waveArr.begin(),waveArr.end()
                                   ,freIte,magIte
                                       ,fs,fftSize
                                       ,ampType);

    if(len <= 0)
    {
        return std::make_tuple(nullptr,nullptr);
    }
    return std::make_tuple(fre,mag);
}


void saFun::spectrum(const QVector<double> &input, double fs, size_t fftSize, czy::Math::DSP::SpectrumType ampType, QVector<double> &out_fre, QVector<double> &out_mag)
{
    out_fre.reserve(fftSize/2);
    out_mag.reserve(fftSize/2);
    std::back_insert_iterator<QVector<double> > freIte(out_fre);
    std::back_insert_iterator<QVector<double> > magIte(out_mag);
    int len = czy::Math::DSP::spectrum(input.cbegin(),input.cend()
                                   ,freIte,magIte
                                       ,fs,fftSize
                                       ,ampType);
}
///
/// \brief 功率谱分析
/// \param wave 波形
/// \param fs 采样率
/// \param fftSize fft长度
/// \param pdw PowerDensityWay功率谱估计的方法，功率谱估计提供了3种估计方法
/// czy::Math::DSP::PowerDensityWay
/// \param samplingInterval
/// \return output[频率，幅值]
///
std::tuple<std::shared_ptr<SAVectorDouble>, std::shared_ptr<SAVectorDouble> >
saFun::powerSpectrum(const SAAbstractDatas *wave
                                  , double fs
                                  , size_t fftSize
                                  , int pdw
                                  , double samplingInterval)
{
    QVector<double> waveArr;
    if(SA::VectorPoint == wave->getType())
    {
        const SAVectorPointF* pf = static_cast<const SAVectorPointF*>(wave);
        pf->getYs(waveArr);
    }
    else if(!SAAbstractDatas::converToDoubleVector(wave,waveArr))
    {
        setErrorString(TR("can not conver data to double vector!"));
        return std::make_tuple(nullptr,nullptr);
    }
    if(waveArr.size() <= 0)
    {
        setErrorString(TR("wave size is too short!"));
        return std::make_tuple(nullptr,nullptr);
    }
    std::shared_ptr<SAVectorDouble> fre = SAValueManager::makeData<SAVectorDouble>();
    std::shared_ptr<SAVectorDouble> mag = SAValueManager::makeData<SAVectorDouble>();
    fre->setName(QString("%1-fre").arg(wave->getName()));
    mag->setName(QString("%1-mag").arg(wave->getName()));
    std::back_insert_iterator<SAVectorDouble> freIte(*(fre.get()));
    std::back_insert_iterator<SAVectorDouble> magIte(*(mag.get()));
    czy::Math::DSP::PowerDensityWay pdwTmp = static_cast<czy::Math::DSP::PowerDensityWay>(pdw);
    int len = czy::Math::DSP::powerSpectrum(waveArr.begin(),waveArr.end()
                                   ,freIte,magIte
                                       ,fs,fftSize
                                       ,pdwTmp
                                        ,samplingInterval);
    if(len <= 0)
    {
        return std::make_tuple(nullptr,nullptr);
    }
    return std::make_tuple(fre,mag);
}

std::shared_ptr<SAVectorDouble> _setWindow(QVector<double>& y, czy::Math::DSP::WindowType window)
{
    czy::Math::DSP::windowed (y.begin (),y.end (),window);
    return SAValueManager::makeData<SAVectorDouble>(y);
}
std::shared_ptr<SAVectorPointF> _setWindow(const SAVectorPointF* wave, czy::Math::DSP::WindowType window)
{
    QVector<double> x,y;
    wave->getYs(y);
    wave->getXs(x);
    czy::Math::DSP::windowed (y.begin (),y.end (),window);
    return SAValueManager::makeData<SAVectorPointF>(wave->getName() + "window",x,y);
}
///
/// \brief 设置窗函数
/// \param wave 波形
/// \param window 窗类型
/// \return 设置窗后的波形
///
std::shared_ptr<SAAbstractDatas> saFun::setWindow(const SAAbstractDatas *wave, czy::Math::DSP::WindowType window)
{
    QVector<double> waveArr;
    if(SA::VectorPoint == wave->getType())
    {
        std::shared_ptr<SAVectorPointF> res = _setWindow(static_cast<const SAVectorPointF*>(wave),window);
        return SAValueManager::castPointToBase(res);
    }
    else if(SAAbstractDatas::converToDoubleVector(wave,waveArr))
    {
        if(waveArr.size() <= 0)
        {
            setErrorString(TR("wave size is too short!"));
            return nullptr;
        }
        std::shared_ptr<SAVectorDouble> res = _setWindow(waveArr,window);
        if(res)
        {
            res->setName(wave->getName() + "window");
        }
        return SAValueManager::castPointToBase(res);
    }
    setErrorString(TR("can not conver data to double vector!"));
    return nullptr;
}

///
/// \brief saFun::setWindow
/// \param input
/// \param window
///
void saFun::setWindow(QVector<double> &input, czy::Math::DSP::WindowType window)
{
    czy::Math::DSP::windowed (input.begin (),input.end (),window);
}

///
/// \brief 窗函数名
/// \param window 窗类型
/// \return
///
QString saFun::windowName(czy::Math::DSP::WindowType window)
{
    switch(window)
    {
    case czy::Math::DSP::WindowRect:return TR("Rect Window");
    case czy::Math::DSP::WindowHanning:return TR("Hanning Window");
    case czy::Math::DSP::WindowHamming:return TR("Hamming Window");
    case czy::Math::DSP::WindowBlackman:return TR("Blackman Window");
    case czy::Math::DSP::WindowBartlett:return TR("Bartlett Window");
    }
    return TR("Rect Window");
}





