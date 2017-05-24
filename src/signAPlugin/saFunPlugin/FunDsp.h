#ifndef FUN_DSP_H
#define FUN_DSP_H
#include <memory>
class SATimeFrequencyAnalysis;

class FunDsp {
public:
    //去趋势
    static void detrendDirect();
    //信号设置窗
    static void setWindowToWave();
    //频谱分析
    static void spectrum();
    //功率谱分析
    static void powerSpectrum();
    //时频分析工具箱
    void tmeFrequency();
private:
    std::unique_ptr<SATimeFrequencyAnalysis> timeFrequencyWidget;

};

#endif // FUN_DSP_H
