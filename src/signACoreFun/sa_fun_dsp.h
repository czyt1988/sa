#ifndef SAFUNDSP_H
#define SAFUNDSP_H
#include "SACoreFunGlobal.h"
#include "czyMath.h"
#include "czyMath_DSP.h"
#include "sa_fun_core.h"
class SAAbstractDatas;
class SAVariantDatas;
class SAVectorDouble;

namespace saFun {

//去直流 to_dc(wave)
SA_CORE_FUN__EXPORT
std::shared_ptr<SAVectorDouble> detrendDirect(SAAbstractDatas* wave);

//频谱分析 spectrum(wave,fs,fftSize,ampType)->[fre,amp]
SA_CORE_FUN__EXPORT
std::tuple<std::shared_ptr<SAVectorDouble>,std::shared_ptr<SAVectorDouble> >spectrum(SAAbstractDatas* wave
                                  ,double fs
                                  ,size_t fftSize
                                  ,czy::Math::DSP::SpectrumType ampType);
//功率谱分析 powerSpectrum(wave,fs,fftSize,pdw,samplingInterval)->[fre,amp]
SA_CORE_FUN__EXPORT
std::tuple<std::shared_ptr<SAVectorDouble>,std::shared_ptr<SAVectorDouble> > powerSpectrum(SAAbstractDatas* wave
                                  ,double fs
                                  ,size_t fftSize
                                  ,int pdw
                                  ,double samplingInterval);
//设置窗函数
SA_CORE_FUN__EXPORT
std::shared_ptr<SAVectorDouble> setWindow(SAAbstractDatas* wave,czy::Math::DSP::WindowType window);
//窗函数名
SA_CORE_FUN__EXPORT
QString windowName(czy::Math::DSP::WindowType window);
}
#endif // SADSP_H
