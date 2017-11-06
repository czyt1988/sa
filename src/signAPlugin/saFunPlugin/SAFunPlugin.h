#ifndef SATEXTDATAIMPORT_H
#define SATEXTDATAIMPORT_H
#include "SAPluginInterface.h"
#include <QObject>
#include <QtPlugin>
#include <QIcon>
#include <memory>
#include "SATimeFrequencyAnalysis.h"
class SAAbstractFunctionItem;
class SAAbstractDatas;
class QStringList;
class QAction;
class QMenu;
class SAPropertySetDialog;
class QtVariantProperty;
class FunDsp;
class FunDataPreprocessing;
class FunNum;
class FunFit;
class SAUIInterface;
///
/// \brief 数据导入的接口
///
class SAFunPlugin : public QObject
                        ,public SAAbstractFunctionPackagePlugin
{
    Q_OBJECT
public:
    SAFunPlugin();
    virtual ~SAFunPlugin();
    //插件类型
    virtual PluginType getType() const;
    //插件的名称
    virtual QString getPluginName() const;
    //返回插件版本
    virtual QString getVersion() const;
    //获取插件的描述
    virtual QString getDescription() const;

    //获取菜单图标
    virtual void setupUI(SAUIInterface* ui);

    //获取包名
    virtual QString getPackageName() const; 
    //获取功能action的目录列表
    virtual QStringList getFunctionActionCategory() const;
    //获取目录对应的actions 目录通过getFunctionActionCategory获取
    virtual QList<QAction*> getActionList(const QString& category) const;
    //
    void retranslateUI();
protected:
    bool event(QEvent *e);
private slots:
    //======统计学相关菜单==========================================
    //求和
    void on_sumAction();
    //求均值
    void on_meanAction();
    //求均值
    void on_diffAction();
    //求统计参数
    void on_statisticsAction();
    //频率统计
    void on_histAction();
private slots:
    //=====信号处理相关菜单========================================
    //去趋势
    void on_detrendDirect();
    //信号设置窗
    void on_setWindowToWave();
    //频谱分析 spectrum(wave,fs,fftSize,ampType)->[fre,amp]
    void on_spectrumAction();
    //功率谱分析
    void on_powerSpectrumAction();
    //时频分析工具箱
    void on_tmeFrequencyAction();
private slots:
    //=====数据预处理相关菜单========================================
    //sigma异常值判断
    void on_sigmaDetectAction();
    //m点n次滤波
    void on_pointSmoothAction();
private slots:
    //=====数据拟合相关菜单========================================
    void on_fittingFigureCurveAction();
private:
    void init();
    void setupDSPMenu();
    void setupStatisticsMenu();
    void setupDataPreprocessingMenu();
    void setupFittingMenu();
private:
    std::unique_ptr<QMenu> m_menuDSP;///<信号处理相关菜单
    std::unique_ptr<QMenu> m_menuStatistics;///<统计学相关菜单
    std::unique_ptr<QMenu> m_dataPreprocessing;///< 数据预处理
    std::unique_ptr<QMenu> m_dataFitting;///< 拟合相关菜单
    //=====数据预处理相关菜单========================================
    QAction* m_sigmaDetectAction;///< sigma异常值判断
    QAction* m_pointSmoothAction;///< m点n次滤波
    //=====信号处理相关菜单========================================
    QAction* m_detrendDirectAction;///< 去趋势(去直流)
    QAction* m_waveSetWindowAction;///< 信号设置窗
    QAction* m_spectrumAction;///< 频谱分析 spectrum(wave,fs,fftSize,ampType)->[fre,amp]
    QAction* m_powerSpectrumAction;///< 功率谱分析
    QAction* m_tmeFrequencyAction;///< 时频分析工具箱
    //======统计学相关菜单==========================================
    QAction* m_sumAction;///< 求和
    QAction* m_meanAction;///< 求均值
    QAction* m_diffAction;///< 求差分
    QAction* m_statisticsAction;///< 求统计参数
    QAction* m_histAction;///< 频率统计
    //======统计学相关菜单==========================================
    QAction* m_FittingFigureCurve;///< 拟合图表曲线
    //
    std::unique_ptr<FunDsp> m_funDSP;///< Dsp相关的函数封装
    std::unique_ptr<FunDataPreprocessing> m_funDataPreprocessing;///< DataPreprocessing相关的函数封装
    std::unique_ptr<FunNum> m_funNum;///< num相关的函数封装
    std::unique_ptr<FunFit> m_funFit;///< Fit相关的函数封装
    SAUIInterface* m_ui;
    //
    QHash<QString,QList<QAction*> > m_category2actionList;
};



#endif // SAABSTRACTDATAIMPORT

