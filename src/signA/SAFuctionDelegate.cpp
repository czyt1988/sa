#include "SAFuctionDelegate.h"
#include <mainwindow.h>
#include <SAChart2D.h>
#include <memory>

#include <QMessageBox>
#include <SAPropertySetDialog.h>
#include <limits>
//----------------

#include <SAFunctionFilter.h>
#include <SAFunctionStatistics.h>

#include <QtVariantPropertyManager>
//
#include "SAValueManager.h"
SAFuctionDelegate::SAFuctionDelegate(MainWindow* wnd)
    :SAMainWindowDelegate(wnd)
{

}

SAFuctionDelegate::~SAFuctionDelegate()
{

}


///
/// \brief 傅里叶变化
/// \param data 原始波形
/// \param args 其它参数，包括采样率和采样点
/// \param draw 是否进行绘制
/// \return
///
//bool SAFuctionDelegate::FFT(SAAbstractDatas* data, bool draw)
//{
//    int size = data->getSize ();
//    if(size<=0)
//    {
//        QMessageBox::information (getMainWindow (),tr("information")
//                                  ,tr("Please select valid data!"));//请选择有效数据！
//        return false;
//    }
//    SAPropertySetDialog dlg(getMainWindow ());
//    QList<QtVariantProperty*> prop = dlg.setDescribe (QList<QPair<QString,int>>()
//                     <<QPair<QString,int>(tr("fft length:"),QVariant::Int)//傅里叶变换长度
//                     <<QPair<QString,int>(tr("sampling frequency(Hz):"),QVariant::Double)//采样频率
//                     <<QPair<QString,int>(tr("is detrend:"),QVariant::Bool)//去均值预处理
//                     <<QPair<QString,int>(tr("amplitude/magnitude:"),QtVariantPropertyManager::enumTypeId())//幅值处理方式
//                     );

//    QStringList enumNames;
//    enumNames<<tr("magnitude")//幅度谱(magnitude)
//            <<tr("magnitude dB")//幅度谱dB(magnitude dB)
//              <<tr("Amplitude")//幅值谱(Amplitude)
//            <<tr("Amplitude dB");//幅值谱dB(Amplitude dB)
//    prop.back ()->setAttribute(QLatin1String("enumNames"), enumNames);
//    dlg.setDefaultData (QVariantList()
//                        <<size//采样点数
//                        <<double(size)//采样频率
//                        <<true//去均值预处理
//                        <<2//幅值处理方式
//                        );
//    if(QDialog::Accepted != dlg.exec ())
//    {
//        return false;
//    }
//    QVariantList args = dlg.getDatas ();
//    std::unique_ptr<SAFuctionFFT> fun(new SAFuctionFFT);
//    fun->addAddtionalInput("samnum",args[0]);//记录采样点数
//    fun->addAddtionalInput("samrate",args[1]);//记录采样率
//    fun->addAddtionalInput("isdetrend",args[2]);//记录采样率
//    fun->addAddtionalInput("spectrumtype",args[3]);//记录频谱的幅值处理方法
//    fun->addAddtionalInput("mode","fft");//标记进行频谱分析
//    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
//    if(res.size ()==0)//说明计算没结果
//    {
//        return false;
//    }
//    //res[0]->setName (QStringLiteral("fft(%1)").arg(data->getName ()));
//    fun.release ();
//    if(draw)
//    {
//        getMainWindow ()->getDrawDelegate ()->draw (res[0]);
//    }
//    return true;
//}
///
/// \brief SAFuctionDelegate::makePowerSpectrum
/// \param data
/// \param draw
/// \return
///
//bool SAFuctionDelegate::PSD(SAAbstractDatas* data, bool draw)
//{
//    int size = data->getSize ();
//    if(size<=0)
//    {
//        QMessageBox::information (getMainWindow (),tr("information")
//                                  ,tr("Please select valid data!"));
//        return false;
//    }
//    SAPropertySetDialog dlg(getMainWindow ());
//    QList<QtVariantProperty*> prop = dlg.setDescribe (QList<QPair<QString,int>>()
//                     <<QPair<QString,int>(tr("Sampling numbers:"),QVariant::Int)//采样点数
//                     <<QPair<QString,int>(tr("sampling frequency(Hz):"),QVariant::Double)//采样频率
//                     <<QPair<QString,int>(tr("is detrend:"),QVariant::Bool)//去均值预处理
//                     <<QPair<QString,int>(tr("PSD estimate way:"),QtVariantPropertyManager::enumTypeId())//功率密度估计方式
//                     <<QPair<QString,int>(tr("Sampling time interval:"),QVariant::Double)//采样时间间隔
//                     );
//    QStringList enumNames;
//    enumNames<<tr("MSA")//MSA(平均振幅平方法)
//            <<tr("SSA")//SSA(和振幅平方法)
//              <<tr("TISA");//TISA(时间积分振幅平方法)
//    prop[3]->setAttribute(QLatin1String("enumNames"), enumNames);
//    prop[3]->setToolTip (tr("TISA (time integral amplitude squared) needs to set the sampling interval!"));//TISA(时间积分振幅平方法)需要设定采样间隔！
//    prop[4]->setToolTip (tr("Only when it is set to TISA (time integral amplitude squared) to define"));//仅当TISA(时间积分振幅平方法)设定时需要指定！
//    double defaultInteral = getInterval(data);
//    dlg.setDefaultData (QVariantList()
//                        <<size//采样点数
//                        <<double(size)//采样频率
//                        <<true//去均值预处理
//                        <<0//平均振幅平方法
//                        <<defaultInteral//默认的采样间隔
//                        );
//    if(QDialog::Accepted != dlg.exec ())
//    {
//        return false;
//    }
//    QVariantList args = dlg.getDatas ();
//    std::unique_ptr<SAFuctionFFT> fun(new SAFuctionFFT);
//    fun->addAddtionalInput("samnum",args[0]);//记录采样点数
//    fun->addAddtionalInput("samrate",args[1]);//记录采样率
//    fun->addAddtionalInput("isdetrend",args[2]);//记录采样率
//    fun->addAddtionalInput("powerdensityway",args[3]);//记录功率谱估计的方法
//    fun->addAddtionalInput("interval",args[4]);//时间间隔
//    fun->addAddtionalInput("mode","psd");//标记进行功率谱分析
//    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());

//    if(res.size ()==0)//说明计算没结果
//    {
//        return false;
//    }
//    fun.release ();
//    if(draw)
//    {
//        getMainWindow ()->getDrawDelegate ()->draw (res[0]);
//    }
//    return true;
//}

bool SAFuctionDelegate::Detrend(SAAbstractDatas* data, bool draw)
{
    int size = data->getSize ();
    if(size<=0)
    {
        QMessageBox::information (getMainWindow (),tr("information")
                                  ,tr("Please select valid data!"));
        return false;
    }
    std::unique_ptr<SAFunctionFilter> fun(new SAFunctionFilter);
    fun->addAddtionalInput("mode","detrend");//标记进行去均值分析
    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
    if(res.size ()==0)//说明计算没结果
    {
        return false;
    }
    fun.release ();
    if(draw)
    {
        getMainWindow ()->getDrawDelegate ()->draw (res[0]);
    }
    return true;
}

bool SAFuctionDelegate::SmoothPoint(SAAbstractDatas* data, int point, int power, bool draw)
{
    int size = data->getSize ();
    if(size<=0)
    {
        QMessageBox::information (getMainWindow (),tr("information")
                                  ,tr("Please select valid data!"));
        return false;
    }
    std::unique_ptr<SAFunctionFilter> fun(new SAFunctionFilter);
    fun->addAddtionalInput("mode","pointsmooth");//标记进行功率谱分析
    fun->addAddtionalInput("points",point);
    fun->addAddtionalInput("power",power);
    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
    if(res.size ()==0)//说明计算没结果
    {
        return false;
    }
    fun.release ();
    if(draw)
    {
        getMainWindow ()->getDrawDelegate ()->draw (res[0]);
    }
    return true;
}

bool SAFuctionDelegate::SigmaDenoising(SAAbstractDatas* data, bool draw)
{
//    int size = data->getSize ();
//    if(size<=0)
//    {
//        QMessageBox::information (getMainWindow (),tr("information")
//                                  ,tr("Please select valid data!"));
//        return false;
//    }
//    SAPropertySetDialog dlg;
//    dlg.setDescribe (QList<QPair<QString,int>>()
//                     <<QPair<QString,int>(tr("sigma:"),QVariant::Double)
//                     );
//    dlg.setDefaultData (QVariantList()<<double(3.0));
//    if(QDialog::Accepted != dlg.exec ())
//    {
//        return false;
//    }
//    QVariantList args = dlg.getDatas ();
//    if(args.size ()!=1)
//    {
//        QMessageBox::information (getMainWindow (),tr("information")
//                                  ,tr("Parameter input error"));
//        return false;
//    }
//    std::unique_ptr<SAFunctionFilter> fun(new SAFunctionFilter);
//    fun->addAddtionalInput("mode","sigma");//标记进行sigma去噪
//    fun->addAddtionalInput("sigma",args[0]);//sigma值
//    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
//    if(res.size ()==0)//说明计算没结果
//    {
//        return false;
//    }
//    fun.release ();
//    if(draw)
//    {
//        getMainWindow ()->getDrawDelegate ()->draw (res[0]);
//    }
    return true;
}

bool SAFuctionDelegate::Statistics(SAAbstractDatas* data)
{
    int size = data->getSize ();
    if(size<=0)
    {
        QMessageBox::information (getMainWindow (),tr("information")
                                  ,tr("Please select valid data!"));
        return false;
    }
    std::unique_ptr<SAFunctionStatistics> fun(new SAFunctionStatistics);
    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
    if(res.size ()==0)//说明计算没结果
    {
        return false;
    }
    fun.release ();
    return true;
}

bool SAFuctionDelegate::Hist(SAAbstractDatas* data, bool draw)
{
//    int size = data->getSize ();
//    if(size<=0)
//    {
//        QMessageBox::information (getMainWindow (),tr("information")
//                                  ,tr("Please select valid data!"));
//        return false;
//    }
//    SAPropertySetDialog dlg;
//    dlg.setDescribe (QList<QPair<QString,int>>()
//                     <<QPair<QString,int>(tr("Number of sections:"),QVariant::Int)
//                     );
//    dlg.setDefaultData (QVariantList()<<100);
//    if(QDialog::Accepted != dlg.exec ())
//    {
//        return false;
//    }
//    QVariantList args = dlg.getDatas ();
//    if(args.size () != 1)
//    {
//        QMessageBox::information (getMainWindow (),tr("information")
//                                  ,tr("Error in obtaining parameters"));//获取参数出错
//        return false;
//    }
//    std::unique_ptr<SAFunctionStatistics> fun(new SAFunctionStatistics);
//    fun->addAddtionalInput("mode","hist");//标记进行功率谱分析
//    fun->addAddtionalInput("histcount",args[0]);
//    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
//    if(res.size ()==0)//说明计算没结果
//    {
//        return false;
//    }
//    fun.release ();
//    if(draw)
//    {
//        getMainWindow ()->getDrawDelegate ()->drawBar (res[0]);
//    }
    return true;
}
#if 0
bool SAFuctionDelegate::Diff(SAAbstractDatas* data, unsigned diffCount, bool draw)
{
    int size = data->getSize ();
    if(size<=0)
    {
        QMessageBox::information (getMainWindow (),tr("information")
                                  ,tr("Please select valid data!"));
        return false;
    }
    std::unique_ptr<SAFunctionNum> fun(new SAFunctionNum);
    fun->addAddtionalInput("mode","diff");//标记进行功率谱分析
    fun->addAddtionalInput("diffcount",diffCount);
    QList<SAAbstractDatas*> res = dealMake(data,fun.get ());
    if(res.size ()==0)//说明计算没结果
    {
        return false;
    }
    fun.release ();
    if(draw)
    {
        getMainWindow ()->getDrawDelegate ()->draw (res[0]);
    }
    return true;
}
#endif
///获取数据的两个间距，如果没有返回nan
double SAFuctionDelegate::getInterval(SAAbstractDatas* data) const
{
    if(data->getSize () < 2)
        return std::numeric_limits<double>::quiet_NaN ();
    QVariant var0 = data->getAt (0);
    QVariant var1 = data->getAt (1);
    bool isOK;
    double d0,d1;
    d0 = var0.toDouble (&isOK);
    if(!isOK)
    {
        QPointF p = var0.toPointF ();
        if(p.isNull ())
            return std::numeric_limits<double>::quiet_NaN ();
        d0 = p.x ();
    }
    d1 = var1.toDouble (&isOK);
    if(!isOK)
    {
        QPointF p = var1.toPointF ();
        if(p.isNull ())
            return std::numeric_limits<double>::quiet_NaN ();
        d1 = p.x ();
    }
    return (d1 - d0);
}
///
/// \brief 负责函数的调用以及把生成的数据在文件管理器的加入
/// \param data 输入数据
/// \param fun 输入函数
/// \return 若成功返回结果集,同时会把结算的结果数据自动装入变量管理器中
///
QList<SAAbstractDatas*> SAFuctionDelegate::dealMake(SAAbstractDatas* data,SAAbstractFuctionItem* fun)
{
    QList<SAAbstractDatas*> res;
    if(fun->calc (QList<SAAbstractDatas*>()<<data))
    {
        res = fun->getResultDatas ();
        if(res.size ()==0)//说明计算没结果
        {
            QMessageBox::information (getMainWindow (),tr("information")
                                      ,tr("Error in calculation"));
            return QList<SAAbstractDatas*>();
        }
        if(res[0]->getSize ()==0)//说明计算没结果
        {
            QMessageBox::information (getMainWindow (),tr("information")
                                      ,tr("No calculation results"));//无计算结果
            return QList<SAAbstractDatas*>();
        }
        saValueManager->addDatas (res);
    }
    else
    {
        QMessageBox::information (getMainWindow (),tr("information")
                                  ,tr("Error in calculation:%1")
                                  .arg(fun->getLassError()));
    }
    return res;
}
