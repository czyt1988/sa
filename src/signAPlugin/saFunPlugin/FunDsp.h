#ifndef FUN_DSP_H
#define FUN_DSP_H
#include <memory>
class SATimeFrequencyAnalysis;

class FunDsp {
public:
    //去趋势
    static void detrendDirectInValue();
    static void detrendDirectInChart();
    //信号设置窗
    static void setWindowToWaveInValue();
    //频谱分析
    static void spectrumInValue();
    //功率谱分析
    static void powerSpectrumInValue();
    //时频分析工具箱
    void tmeFrequency();
private:
    std::unique_ptr<SATimeFrequencyAnalysis> timeFrequencyWidget;

};

#endif // FUN_DSP_H
