#ifndef SAFUNDSP_H
#define SAFUNDSP_H
#include "SACoreFunGlobal.h"
#include "SADsp.h"
#include "sa_fun_core.h"
class SAAbstractDatas;
class SAVariantDatas;
class SAVectorDouble;

namespace saFun {

//去直流 to_dc(wave)
SA_CORE_FUN__EXPORT std::shared_ptr<SAAbstractDatas> detrendDirect(const SAAbstractDatas* wave);
SA_CORE_FUN__EXPORT void detrendDirect(QVector<double>& y);
SA_CORE_FUN__EXPORT void detrendDirect(QVector<QPointF>& points);

//频谱分析 spectrum(wave,fs,fftSize,ampType)->[fre,amp]
SA_CORE_FUN__EXPORT std::tuple<std::shared_ptr<SAVectorDouble>,std::shared_ptr<SAVectorDouble> >spectrum(const SAAbstractDatas* wave
                                  ,double fs
                                  ,size_t fftSize
                                  ,SA::SADsp::SpectrumType ampType);
SA_CORE_FUN__EXPORT void spectrum(const QVector<double>& input
              ,double fs
              ,size_t fftSize
              ,SA::SADsp::SpectrumType ampType
              ,QVector<double>& out_fre
              ,QVector<double>& out_mag);
//功率谱分析 powerSpectrum(wave,fs,fftSize,pdw,samplingInterval)->[fre,amp]
SA_CORE_FUN__EXPORT std::tuple<std::shared_ptr<SAVectorDouble>,std::shared_ptr<SAVectorDouble> > powerSpectrum(const SAAbstractDatas* wave
                                  ,double fs
                                  ,size_t fftSize
                                  ,int pdw
                                  ,double samplingInterval);
SA_CORE_FUN__EXPORT void powerSpectrum(const QVector<double>& input
              ,double fs
              ,size_t fftSize
              ,SA::SADsp::PowerDensityWay pdwType
              ,QVector<double>& out_fre
              ,QVector<double>& out_mag
              ,double ti = 0.1
                   );
//设置窗函数
SA_CORE_FUN__EXPORT std::shared_ptr<SAAbstractDatas> setWindow(const SAAbstractDatas *wave, SA::SADsp::WindowType window);
SA_CORE_FUN__EXPORT void setWindow(QVector<double>& input,SA::SADsp::WindowType window);
SA_CORE_FUN__EXPORT void setWindow(QVector<QPointF>& input,SA::SADsp::WindowType window);
//窗函数名
SA_CORE_FUN__EXPORT QString windowName(SA::SADsp::WindowType window);
}
#endif // SADSP_H
