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



///
/// \brief 去直流
/// \param wave 波形
/// \return 去直流后波形
///
std::shared_ptr<SAVectorDouble> saFun::detrendDirect(SAAbstractDatas* wave)
{
    QVector<double> waveData;
    if(!wave->converToDoubleVector(waveData))
    {
        setErrorString(TR("can not conver data to double vector!"));
        return nullptr;
    }
    czy::Math::DSP::detrend(waveData.begin(),waveData.end());
    return SAValueManager::makeData<SAVectorDouble>(QString("%1_DC").arg(wave->getName()),waveData);
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
saFun::spectrum(SAAbstractDatas* wave
                                  , double fs
                                  , size_t fftSize
                                  , czy::Math::DSP::SpectrumType ampType)
{
    QVector<double> waveArr;
    if(!wave->converToDoubleVector(waveArr))
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
saFun::powerSpectrum(SAAbstractDatas* wave
                                  ,double fs
                                  ,size_t fftSize
                                  ,int pdw
                                  ,double samplingInterval)
{
    QVector<double> waveArr;
    if(!wave->converToDoubleVector(waveArr))
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
///
/// \brief 设置窗函数
/// \param wave 波形
/// \param window 窗类型
/// \return 设置窗后的波形
///
std::shared_ptr<SAVectorDouble> saFun::setWindow(SAAbstractDatas *wave, czy::Math::DSP::WindowType window)
{
    QVector<double> waveArr;
    if(!wave->converToDoubleVector(waveArr))
    {
        setErrorString(TR("can not conver data to double vector!"));
        return nullptr;
    }
    if(waveArr.size() <= 0)
    {
        setErrorString(TR("wave size is too short!"));
        return nullptr;
    }
    czy::Math::DSP::windowed (waveArr.begin (),waveArr.end (),window);
    return SAValueManager::makeData<SAVectorDouble>(wave->getName() + "window",waveArr);
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
