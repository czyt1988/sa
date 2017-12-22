#ifndef FUN_DSP_H
#define FUN_DSP_H
#include <memory>
#include <QString>
#include "czyMath_DSP.h"
class SATimeFrequencyAnalysis;

class FunDsp {
public:
    //去趋势
    static void detrendDirect();
    //信号设置窗
    static void setWindow();
    //频谱分析
    static void spectrumInValue();
    //功率谱分析
    static void powerSpectrumInValue();
    //时频分析工具箱
    void tmeFrequency();
private:
    static void setWindowToWaveInValue();
    static void setWindowToWaveInChart();
    static bool getWindowProperty(czy::Math::DSP::WindowType &windowType);
    static QString windowTypeToString(czy::Math::DSP::WindowType windowType);
    static void detrendDirectInValue();
    static void detrendDirectInChart();
    std::unique_ptr<SATimeFrequencyAnalysis> timeFrequencyWidget;

};

#endif // FUN_DSP_H
