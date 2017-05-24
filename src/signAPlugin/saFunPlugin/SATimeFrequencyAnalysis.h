#ifndef SATIMEFREQUENCYANALYSIS_H
#define SATIMEFREQUENCYANALYSIS_H

#include <SAData.h>
#include <QVector>
#include <QWidget>
#include <qwt_plot_curve.h>
#include <memory>
#include <czyMath_DSP.h>
namespace Ui {
class SATimeFrequencyAnalysis;
}

class MainWindow;
class QwtPlotZoneItem;
class QButtonGroup;
class SATimeFrequencyAnalysis : public QWidget
{
    Q_OBJECT
public:
    explicit SATimeFrequencyAnalysis(QWidget *parent = 0);
    ~SATimeFrequencyAnalysis();
    bool setWaveData(SAAbstractDatas* data);
private slots:
    //滚动条滑动触发
    void onSliderMoved(int value);
    //按钮 - 导入
    void onPushButtonImport();
    //按钮 - 刷新
    //void onPushbuttonUpdata();
    //采样间隔刷新
    void onLineEditIntervalFinished();
    //按钮 - 导出
    void onPushbuttonExportdata();
    //按钮 - 左移波形
    void onPushButtonLeftMove();
    //按钮 - 右移波形
    void onPushButtonRightMove();
    //按钮 - 定位展开
    void onPushButtonLocate();
    //radio按钮 - 选择幅度设置
    void onButtonGroupMagSetToggled(int id, bool checked);
    //radio按钮 - 功率谱估计方法
    void onButtonGroupPSDSetToggled(int id, bool checked);
    //radio按钮 - 傅里叶变换选择
    void onRadioButtonFFT_Toggled(bool toggled);
    //radio按钮 - 功率谱分析选择
    void onRadioButtonPSD_Toggled(bool toggled);
    //窗口长度
    void onSpinBoxIntervalLengthEditingFinished();
    //spin开始索引的值改变
    void onSpinBoxStartIndexEditingFinished();
    //spin结束索引的值改变
    void onSpinBoxEndIndexEditingFinished();
private:
    //初始化UI
    void initUI();
    //初始化图形设置
    void initChart();
    //更新采样间隔，true为获取客户端数据到内存，false为把内存数据上载到客户端
    void updateSampleInterval(bool load = true);
    //检查采样间隔
    bool checkSampleInterval();
    //更新数据
    void updateData();
    //更新截断长度 - 在截断长度改变后触发
    void updateIntervalLength();
    //根据滚动条刷新
    void updateSlider();
    //更新滚动条范围
    void updateSliderRang();
    //更新信息 - 在滚动后触发
    void updateTextInfo();
    //更新频谱图，在滚动条滚动，采样间隔，等图标相关改变触发
    void updateSpectrumChart();
    //更加当前的索引计算进度条的值
    int calcSliderValueByIndex(int index, int dataSize, int intervalSize) const;
    int calcIndexBySliderValue(int sliderValue, int dataSize, int intervalSize) const;
    //设置开始索引
    void setStartIndex(int startIndex);
    //设置开始索引
    void setEndIndex(int endIndex);
private:
    //fft分析
    void dealFFT();
    //功率谱分析
    void dealPSD();
    //获取截断的波形数据，会返回采样率
    double getWaveData(QVector<double>& wave);
private:
    enum CalcType{
        FFT,PSD
    };
    Ui::SATimeFrequencyAnalysis *ui;
    std::unique_ptr<QwtPlotCurve> m_wave;
    std::unique_ptr<QwtPlotCurve> m_spectrum;
    double m_interval;
    QwtPlotZoneItem* m_plotZone;
    double m_xIntervalStart;///< 记录区域的开始x轴数据
    double m_xIntervalEnd;///< 记录区域的结束x轴数据
    int m_xIntervalStartIndex;///< 记录区域的开始x轴数据索引
    int m_xIntervalEndIndex;///< 记录区域的结束x轴数据索引
    int m_rangLength;///< 记录截断长度
    QButtonGroup* m_groupAnasysSet;///< 分析模式选择
    QButtonGroup* m_groupMagSet;///< 幅值设置
    QButtonGroup* m_groupPSDSet;///< psd分析模式
    czy::Math::DSP::SpectrumType m_magType;///< 频谱模式
    czy::Math::DSP::PowerDensityWay m_psdType;///< 功率谱模式
    CalcType m_spectrumType;///< 计算方式-功率谱-fft
    bool m_isDetrend;///< 是否去均值
    czy::Math::DSP::WindowType m_signalWindow;///< 信号的窗
};

#endif // SATIMEFREQUENCYANALYSIS_H
