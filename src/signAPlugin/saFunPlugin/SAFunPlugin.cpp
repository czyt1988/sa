#include "SAFunPlugin.h"
#include <QApplication>
//sa lib
#include "SAGlobals.h"
#include "SAAbstractFunctionItem.h"
#include "SAStandardFunctionItem.h"
#include "SAAbstractDatas.h"
#include "SAVariantDatas.h"
#include "SAValueManager.h"
//sa core function
#include "sa_fun_core.h"
#include "sa_fun_dsp.h"
#include "sa_fun_num.h"
#include "sa_fun_preproc.h"
//sa common ui
#include "SAUIInterface.h"
#include "SAUIReflection.h"
#include "SAPropertySetDialog.h"
#include "SAFigureWindow.h"

#include <QAction>
#include <QMenu>
#include <limits>

#include "Resource.h"
#include "czyMath_DSP.h"

#include "SATimeFrequencyAnalysis.h"
#include "FunDataPreprocessing.h"
#include "FunDsp.h"
#include "FunNum.h"
#include "FunFit.h"
#if defined(SA_COMMON_PLUGIN_LIB_MAKE)     // create a DLL library
    #ifndef SA_PLUGIN_EXPORT
    #define SA_PLUGIN_EXPORT Q_DECL_EXPORT
    #endif
#else                        // use a DLL library
    #ifndef SA_PLUGIN_EXPORT
    #define SA_PLUGIN_EXPORT Q_DECL_IMPORT
    #endif
#endif

#include  "SAPluginInterface.h"
//这是插件仅有的两个导出函数
extern "C" SA_PLUGIN_EXPORT SAAbstractPlugin* createPlugin();
extern "C" SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin* obj);
extern "C" SA_PLUGIN_EXPORT QString pluginName();
extern "C" SA_PLUGIN_EXPORT QString pluginVersion();
extern "C" SA_PLUGIN_EXPORT QString pluginDescription();
extern "C" SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType();




#define TR(str)\
    QApplication::translate("SAFunPlugin",str,0)

static const QString s_pluginName = TR("SA Default Function Plugin");
static const QString s_pluginDescription =  TR("SA Default Function library");
static const QString s_pluginVersion = TR("version 0.1");
static const SAAbstractPlugin::PluginType s_pluginType = SAAbstractPlugin::Function;
//类似 bool(QList<SAAbstractDatas *> &,QList<SAAbstractDatas *> &,QString&) 的函数导出
static const QString s_packageName = "sa";


SA_PLUGIN_EXPORT SAAbstractPlugin *createPlugin()
{
    return new SAFunPlugin();
}
SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin *obj)
{
    if(obj != nullptr)
    {
        delete obj;
    }
}
SA_PLUGIN_EXPORT QString pluginName()
{
    return s_pluginName;
}
SA_PLUGIN_EXPORT QString pluginVersion()
{
    return s_pluginVersion;
}
SA_PLUGIN_EXPORT QString pluginDescription()
{
    return s_pluginDescription;
}
SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType()
{
    return s_pluginType;
}

//类似 bool(QList<SAAbstractDatas *> &,QList<SAAbstractDatas *> &,QString&) 的函数导出
SA_PLUGIN_EXPORT QStringList functionNames()
{
    return s_functionNames;
}
//======================================================================


//======================================================================


///
/// \brief 是通过SA系统自带的OpenFile还是使用自身openFile
///
/// 如果返回true。这时sa将会调用bool openFile(const QStringList* filePaths),并传入用户选择的路径，
/// 如果需要多选，需要isMultiselect返回true
/// 如果返回false，sa将会调用bool openFile()
/// \return
///
SAFunPlugin::SAFunPlugin()
    :m_ui(nullptr)
    ,m_menuDSP(nullptr)
    ,m_spectrumAction(nullptr)
{
    m_funDSP.reset(new FunDsp);//std::make_unique<FunDsp>();
    m_funDataPreprocessing.reset(new FunDataPreprocessing);//std::make_unique<FunDataPreprocessing>();
    m_funNum.reset(new FunNum);
    m_funFit.reset(new FunFit);
    init();
}

SAFunPlugin::~SAFunPlugin()
{
}

///
/// \brief ui构建
///
void SAFunPlugin::setupUI(SAUIInterface* ui)
{
    m_ui = ui;
    //数据预处理
    setupDataPreprocessingMenu();
    //统计菜单
    setupStatisticsMenu();
    //信号处理相关菜单
    setupDSPMenu();
    //拟合菜单
    setupFittingMenu();
    //
    retranslateUI();
}
//#define FUNPTR_CAST(PTR) static_cast<bool(*)(QList<SAAbstractDatas*>&,QList<SAAbstractDatas*>&,QString*)>(PTR)

void SAFunPlugin::init()
{

}
///
/// \brief 信号处理相关菜单
///
void SAFunPlugin::setupDSPMenu()
{
    //数字信号处理
    m_menuDSP.reset(new QMenu("Signal processing",m_ui->getMainWindowPtr()));
    m_menuDSP->setObjectName(QStringLiteral("SignalProcessingMenu"));

    //去趋势
    m_detrendDirectAction = m_menuDSP->addAction(ICON_detrendDirect,"detrend(direct)");
    m_detrendDirectAction->setObjectName("detrend(direct)");
    connect(m_detrendDirectAction,&QAction::triggered
            ,this,&SAFunPlugin::on_detrendDirect);
    //信号设置窗
    m_waveSetWindowAction = m_menuDSP->addAction(QIcon(),"window");
    m_waveSetWindowAction->setObjectName("wave set window");
    connect(m_waveSetWindowAction,&QAction::triggered,this,&SAFunPlugin::on_setWindowToWave);

    //频谱分析
    m_spectrumAction = m_menuDSP->addAction(ICON_spectrum,tr("spectrum"));
    m_spectrumAction->setObjectName("spectrum");
    connect(m_spectrumAction,&QAction::triggered,this,&SAFunPlugin::on_spectrumAction);

    //功率谱分析
    m_powerSpectrumAction = m_menuDSP->addAction(ICON_powerSpectrum,tr("PSD(power Spectrum)"));
    m_powerSpectrumAction->setObjectName("PSD(power Spectrum)");
    connect(m_powerSpectrumAction,&QAction::triggered,this,&SAFunPlugin::on_powerSpectrumAction);

    //时频分析工具箱
    m_tmeFrequencyAction = m_menuDSP->addAction(tr("tme frequency toolbox"));
    m_tmeFrequencyAction->setObjectName("tme frequency toolbox");
    connect(m_tmeFrequencyAction,&QAction::triggered,this,&SAFunPlugin::on_tmeFrequencyAction);

    m_ui->addAnalysisPluginMenu(m_menuDSP.get());
    m_category2actionList[m_menuDSP->title()] = QList<QAction*>()
            << m_detrendDirectAction
            << m_waveSetWindowAction
            << m_spectrumAction
            << m_powerSpectrumAction
            << m_tmeFrequencyAction;
}
///
/// \brief 统计学相关菜单
///
void SAFunPlugin::setupStatisticsMenu()
{
    m_menuStatistics.reset(new QMenu(("Statistics"),m_ui->getMainWindowPtr()));
    m_menuStatistics->setObjectName(QStringLiteral("StatisticsMenu"));
    //求和
    m_sumAction = m_menuStatistics->addAction(tr("sum"));
    m_sumAction->setObjectName("sumAction");
    connect(m_sumAction,&QAction::triggered,this,&SAFunPlugin::on_sumAction);
    //求均值
    m_meanAction = m_menuStatistics->addAction(tr("mean"));
    m_meanAction->setObjectName("meanAction");
    connect(m_meanAction,&QAction::triggered,this,&SAFunPlugin::on_meanAction);
    //求差分
    m_diffAction = m_menuStatistics->addAction(tr("diff"));
    m_diffAction->setObjectName("diffAction");
    connect(m_diffAction,&QAction::triggered,this,&SAFunPlugin::on_diffAction);
    //求统计参数
    m_statisticsAction = m_menuStatistics->addAction(tr("statistics"));
    m_statisticsAction->setObjectName("statisticsAction");
    connect(m_statisticsAction,&QAction::triggered,this,&SAFunPlugin::on_statisticsAction);
    //频率统计
    m_histAction = m_menuStatistics->addAction(tr("hist"));
    m_histAction->setObjectName("histAction");
    connect(m_histAction,&QAction::triggered,this,&SAFunPlugin::on_histAction);


    m_ui->addAnalysisPluginMenu(m_menuStatistics.get());
    m_category2actionList[m_menuStatistics->title()] = QList<QAction*>()
            << m_sumAction
            << m_meanAction
            << m_diffAction
            << m_statisticsAction
            << m_histAction;
}
///
/// \brief 数据预处理
///
void SAFunPlugin::setupDataPreprocessingMenu()
{
    m_dataPreprocessing.reset(new QMenu(("data preprocessing"),m_ui->getMainWindowPtr()));
    m_dataPreprocessing->setObjectName(QStringLiteral("DataPreprocessingMenu"));
    //西格玛检测
    m_sigmaDetectAction = m_dataPreprocessing->addAction(tr("sigma detect"));
    m_sigmaDetectAction->setObjectName("sigmaDetectAction");
    connect(m_sigmaDetectAction,&QAction::triggered,this,&SAFunPlugin::on_sigmaDetectAction);

    //m点n次滤波
    m_pointSmoothAction = m_dataPreprocessing->addAction(tr("m points n pow smooth"));
    m_pointSmoothAction->setObjectName("mPointsNPowSmoothAction");
    connect(m_pointSmoothAction,&QAction::triggered,this,&SAFunPlugin::on_pointSmoothAction);

    m_ui->addAnalysisPluginMenu(m_dataPreprocessing.get());

    m_category2actionList[m_dataPreprocessing->title()] = QList<QAction*>()
            << m_sigmaDetectAction
            << m_pointSmoothAction
               ;

}
///
/// \brief 构建拟合菜单
///
void SAFunPlugin::setupFittingMenu()
{
    m_dataFitting.reset(new QMenu(("fitting"),m_ui->getMainWindowPtr()));
    m_dataFitting->setObjectName(QStringLiteral("DataFittingMenu"));
    //拟合图表曲线
    m_FittingFigureCurve = m_dataFitting->addAction(ICON_fit, tr("Fitting Curve in Fig"));
    m_FittingFigureCurve->setObjectName("FittingCurveInFigAction");
    connect(m_FittingFigureCurve,&QAction::triggered,this,&SAFunPlugin::on_fittingFigureCurveAction);

    m_ui->addAnalysisPluginMenu(m_dataFitting.get());
    m_category2actionList[m_dataFitting->title()] = QList<QAction*>()
            << m_FittingFigureCurve
               ;
}



///
/// \brief 返回插件类型
/// \return
///
SAAbstractPlugin::PluginType SAFunPlugin::getType() const
{
    return s_pluginType;
}
///
/// \brief 获取插件名字
/// \return
///
QString SAFunPlugin::getPluginName() const
{
    return s_pluginName;
}
///
/// \brief 获取插件版本
/// \return
///
QString SAFunPlugin::getVersion() const
{
    return s_pluginVersion;
}
///
/// \brief 获取描述信息
/// \return
///
QString SAFunPlugin::getDescription() const
{
    return s_pluginDescription;
}



///
/// \brief 获取包名
/// \return
///
QString SAFunPlugin::getPackageName() const
{
    return s_packageName;
}


///
/// \brief 获取功能action的目录列表
/// \return
///
QStringList SAFunPlugin::getFunctionActionCategory() const
{
    QStringList res;
    if(nullptr != m_menuDSP)
    {
        res << m_menuDSP->title();
    }
    if(nullptr != m_dataPreprocessing)
    {
        res << m_dataPreprocessing->title();
    }
    if(nullptr != m_menuStatistics)
    {
        res << m_menuStatistics->title();
    }
    if(nullptr != m_dataFitting)
    {
        res << m_dataFitting->title();
    }
    return res;
}
///
/// \brief 获取目录对应的actions 目录通过getFunctionActionCategory获取
/// \param category
/// \return
///
QList<QAction *> SAFunPlugin::getActionList(const QString &category) const
{

}

void SAFunPlugin::retranslateUI()
{
    //
    m_menuDSP->setTitle(tr("Signal processing"));
    m_detrendDirectAction->setText(tr("detrend(direct)"));
    m_detrendDirectAction->setToolTip(tr("direct detrend for signal"));
    m_waveSetWindowAction->setText(tr("set window"));
    m_waveSetWindowAction->setToolTip(tr("set the window to the wave"));
    m_spectrumAction->setText(tr("spectrum"));
    m_spectrumAction->setToolTip(tr("make spectrum for signal"));
    m_powerSpectrumAction->setText(tr("power spectrum"));
    m_powerSpectrumAction->setToolTip(tr("make power spectrum for signal"));
    m_tmeFrequencyAction->setText(tr("tme frequency"));
    m_tmeFrequencyAction->setToolTip(tr("tme frequency toolbox"));
    //
    m_menuStatistics->setTitle(tr("Statistics"));
    m_sumAction->setText(tr("sum"));
    m_sumAction->setToolTip(tr("sum datas"));
    m_meanAction->setText(tr("mean"));
    m_meanAction->setToolTip(tr("mean datas"));
    m_diffAction->setText(tr("diff"));
    m_diffAction->setToolTip(tr("diff datas"));
    m_statisticsAction->setText(tr("statistics"));
    m_statisticsAction->setToolTip(tr("statistics datas"));
    m_histAction->setText(tr("hist"));
    m_histAction->setToolTip(tr("hist datas"));
    //
    m_dataPreprocessing->setTitle(tr("data preprocessing"));
    m_sigmaDetectAction->setText(tr("sigma detect"));
    m_sigmaDetectAction->setToolTip(tr("detect the datas out of sigma rang"));
    m_pointSmoothAction->setText(tr("m points n pow smooth"));
    m_pointSmoothAction->setToolTip(tr("m points n pow smooth"));
    //
    m_dataFitting->setTitle(tr("fitting"));
    m_FittingFigureCurve->setText(tr("Fitting"));
    m_FittingFigureCurve->setToolTip(tr("Fitting the Curves in Current Figure"));

}

void SAFunPlugin::event(QEvent *e)
{
    if(e->type() == QEvent::LanguageChange)
    {
        //TODO
    }
}
///
/// \brief 求和
///
void SAFunPlugin::on_sumAction()
{
    m_funNum->sum();
}
///
/// \brief 求均值
///
void SAFunPlugin::on_meanAction()
{
    m_funNum->mean();
}
///
/// \brief 求差分
///
void SAFunPlugin::on_diffAction()
{
    m_funNum->diff();

}
///
/// \brief 求统计参数
///
void SAFunPlugin::on_statisticsAction()
{
    m_funNum->statistics();
}

void SAFunPlugin::on_histAction()
{
    m_funNum->hist();
}

///
/// \brief 频谱分析 m_spectrumAction响应
///
void SAFunPlugin::on_spectrumAction()
{
    m_funDSP->spectrum();
}
///
/// \brief 功率谱分析 - powerSpectrum
///
void SAFunPlugin::on_powerSpectrumAction()
{
    m_funDSP->powerSpectrum();
}

///
/// \brief 去趋势
///
void SAFunPlugin::on_detrendDirect()
{
    m_funDSP->detrendDirect();
}
///
/// \brief 信号设置窗
///
void SAFunPlugin::on_setWindowToWave()
{
    m_funDSP->setWindowToWave();
}
///
/// \brief 时频分析工具箱
///
void SAFunPlugin::on_tmeFrequencyAction()
{
    m_funDSP->tmeFrequency();
}
///
/// \brief sigma异常值判断
///
void SAFunPlugin::on_sigmaDetectAction()
{
    m_funDataPreprocessing->sigmaDetect();
}
///
/// \brief m点n次滤波
///
void SAFunPlugin::on_pointSmoothAction()
{
    m_funDataPreprocessing->pointSmooth();
}
///
/// \brief 拟合当前绘图曲线上的曲线
///
void SAFunPlugin::on_fittingFigureCurveAction()
{
    m_funFit->polyfitInChart();
}



