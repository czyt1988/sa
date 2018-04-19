#include "FunDsp.h"
#include <QApplication>
#include <QVariant>
#include <QMdiSubWindow>
#include "sa_fun_dsp.h"
#include "SAPropertySetDialog.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "SAAbstractDatas.h"
#include "SAVariantDatas.h"
#include "SAValueManager.h"
#include "SAFigureWindow.h"
#include "SATimeFrequencyAnalysis.h"
#include "SAGUIGlobalConfig.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAFigureOptCommands.h"
#include "ui_opt.h"
#define TR(str)\
    QApplication::translate("FunDSP", str, 0)



///
/// \brief 去趋势
///
void FunDsp::detrendDirect()
{
    SAUIInterface::LastFocusType ft = saUI->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        detrendDirectInChart();
    }
    else
    {
        detrendDirectInValue();
    }
}
///
/// \brief 信号设置窗
///
void FunDsp::setWindow()
{
    SAUIInterface::LastFocusType ft = saUI->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        setWindowToWaveInChart();
    }
    else
    {
        setWindowToWaveInValue();
    }
}



void FunDsp::detrendDirectInValue()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    std::shared_ptr<SAAbstractDatas> res = saFun::detrendDirect(data);
    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        return;
    }
    QString info = TR("direct detrend :%1").arg(data->getName());
    saValueManager->addData(res);
    saUI->showMessageInfo(info,SA::NormalMessage);
}
///
/// \brief FunDsp::detrendDirectInChart
///
void FunDsp::detrendDirectInChart()
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        saUI->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    QString info = TR("direct detrend :");
    SAFigureOptCommand* topCmd = new SAFigureOptCommand(chart,TR("detrend Direct"));
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QwtSeriesStore<QPointF>* series = dynamic_cast<QwtSeriesStore<QPointF>*>(item);
        if(nullptr == series)
        {
            continue;
        }

        QVector<QPointF> newData;
        if(chart->isRegionVisible())
        {
            QVector<double> xs,ys;
            QVector<int> index;
            if(!chart->getXYDataInRange(&xs,&ys,&index,item))
            {
                continue;
            }

            if(!chart->getXYData(newData,item))
            {
                continue;
            }
            const int dataSize = newData.size();
            const int indexSize = index.size();
            const int ysSize = ys.size();
            if(indexSize <= 0)
            {
                continue;
            }
            saFun::detrendDirect(ys);
            for(int i=0;i<indexSize && i<ysSize;++i)
            {
                if(index[i] < dataSize)
                {
                    newData[index[i]].ry() = ys[i];
                }
            }
        }
        else
        {
            QVector<double> xs,ys;
            if(!chart->getXYData(&xs,&ys,item))
            {
                continue;
            }
            saFun::detrendDirect(ys);
            saFun::makeVectorPointF(xs,ys,newData);
        }
        new SAFigureChangeXYSeriesDataCommand(chart
                                             ,series
                                             ,TR("detrend Direct %1").arg(item->title().text())
                                             ,newData
                                             ,topCmd);
        info += item->title().text();
        info += " ";
    }
    chart->appendCommand(topCmd);
    saUI->showMessageInfo(info,SA::NormalMessage);
}

///
/// \brief 信号设置窗
///
void FunDsp::setWindowToWaveInValue()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(data->getDim() != 1)
    {
        saUI->showWarningMessageInfo(TR("data:[\"%1\"] type is not accept to set window").arg(data->getName()));
        return;
    }
    int dataSize = data->getSize(SA::Dim1);
    if(dataSize <= 1)
    {
        saUI->showWarningMessageInfo(TR("data:[\"%1\"] size is too short").arg(data->getName()));
        return;
    }

    bool isDetrend = false;
    czy::Math::DSP::WindowType window = czy::Math::DSP::WindowRect;
    if(!getWindowProperty(window,isDetrend))
    {
        return;
    }
    std::shared_ptr<SAAbstractDatas> res;
    if(isDetrend)
    {
        res = saFun::detrendDirect(data);
        res = saFun::setWindow(res.get(),window);
    }
    else
    {
        res = saFun::setWindow(data,window);
    }

    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    saValueManager->addData(res);
    saUI->showNormalMessageInfo(TR("data:[\"%1\"] set window(%2)] -> [\"%3\"]")
                                .arg(data->getName())
                                .arg(windowTypeToString(window))
                                .arg(res->getName()));
}
///
/// \brief 信号设置窗
///
void FunDsp::setWindowToWaveInChart()
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        saUI->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    bool isDetrend = false;
    czy::Math::DSP::WindowType window = czy::Math::DSP::WindowRect;
    if(!getWindowProperty(window,isDetrend))
    {
        return;
    }
    QString windowName = windowTypeToString(window);
    QString info = TR("Set %1 ").arg(windowName);
    SAFigureOptCommand* topCmd = new SAFigureOptCommand(chart,info);
    info += ":";
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QwtSeriesStore<QPointF>* series = dynamic_cast<QwtSeriesStore<QPointF>*>(item);
        if(nullptr == series)
        {
            continue;
        }
        QVector<double> xs,ys;
        SAChart::getXYDatas(&xs,&ys,series);
        if(isDetrend)
        {
            saFun::detrendDirect(ys);
        }
        saFun::setWindow(ys,window);
        QVector<QPointF> xys;
        saFun::makeVectorPointF(xs,ys,xys);
        new SAFigureChangeXYSeriesDataCommand(chart
                                             ,series
                                             ,TR("%1 set %2").arg(item->title().text()).arg(windowName)
                                             ,xys
                                             ,topCmd);
        info += item->title().text();
        info += " ";
    }
    chart->appendCommand(topCmd);
    saUI->showMessageInfo(info,SA::NormalMessage);
}

void FunDsp::spectrum()
{
    SAUIInterface::LastFocusType ft = saUI->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        spectrumInChart();
    }
    else
    {
        spectrumInValue();
    }
}

void FunDsp::powerSpectrum()
{
    SAUIInterface::LastFocusType ft = saUI->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        powerSpectrumInChart();
    }
    else
    {
        powerSpectrumInValue();
    }
}

void FunDsp::spectrumInValue()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(data->getDim() != 1)
    {
        saUI->showWarningMessageInfo(TR("data:[\"%1\"] type is not accept to spectrum").arg(data->getName()));
        saUI->raiseMessageInfoDock();
        return;
    }
    int dataSize = data->getSize(SA::Dim1);
    if(dataSize <= 1)
    {
        saUI->showWarningMessageInfo(TR("data:[\"%1\"] size is too short").arg(data->getName()));
        saUI->raiseMessageInfoDock();
        return;
    }
    int fftsize = 100;
    double fs=100;
    bool isDetrend = false;
    czy::Math::DSP::SpectrumType magType = czy::Math::DSP::Amplitude;
    czy::Math::DSP::WindowType window = czy::Math::DSP::WindowRect;
    if(!getSpectrumProperty(&fs,&fftsize,&magType,&window,&isDetrend,dataSize))
    {
        return;
    }


    std::shared_ptr<SAAbstractDatas> preTrendData = nullptr;
    if(isDetrend)//dlg.getData(3).toBool())
    {
        preTrendData = saFun::detrendDirect(preTrendData.get() ? preTrendData.get() : data);
        if(nullptr == preTrendData)
        {
            saUI->showErrorMessageInfo(saFun::getLastErrorString());
            saUI->raiseMessageInfoDock();
            return;
        }
    }
    if(czy::Math::DSP::WindowRect != window)//窗函数设置
    {
        preTrendData = saFun::setWindow(preTrendData.get() ? preTrendData.get() : data
                                                             ,window);
        if(nullptr == preTrendData)
        {
            saUI->showErrorMessageInfo(saFun::getLastErrorString());
            saUI->raiseMessageInfoDock();
            return;
        }
    }
    std::shared_ptr<SAVectorDouble> fre = nullptr;
    std::shared_ptr<SAVectorDouble> mag = nullptr;

    std::tie(fre,mag) = saFun::spectrum( preTrendData.get() ? preTrendData.get() : data
                                             ,fs//dlg.getData(0).toDouble()
                                             ,fftsize//dlg.getData(1).toInt()
                                             ,magType
                                             );
    if(nullptr == fre || nullptr == mag)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        return;
    }
    saValueManager->addData(fre);
    saValueManager->addData(mag);
    QString info = TR("fft(data=[\"%1\"], fftsize=%2,fs=%3,magType=%4,window=%5) -> [fre=\"%6\",mag=\"%7\"]")
            .arg(data->getName())
            .arg(fftsize)
            .arg(fs)
            .arg(magTypeToString(magType))
            .arg(windowTypeToString(window))
            .arg(fre->getName())
            .arg(mag->getName())
            ;
    saUI->showNormalMessageInfo(info);
}

void FunDsp::spectrumInChart()
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        saUI->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }

    double fs=100;
    bool isDetrend = false;
    czy::Math::DSP::SpectrumType magType = czy::Math::DSP::Amplitude;
    czy::Math::DSP::WindowType window = czy::Math::DSP::WindowRect;
    //绘图的参数里，没有fftsize
    if(!getSpectrumProperty(&fs,nullptr,&magType,&window,&isDetrend))
    {
        return;
    }
    QStringList lineNameList,newLineNameList;
    saUI->raiseMainDock();
    QMdiSubWindow* sub = saUI->createFigureWindow(QString("%1-fft").arg(saUI->getCurrentActiveSubWindow()->windowTitle()));
    SAFigureWindow* w = saUI->getFigureWidgetFromMdiSubWindow(sub);
    chart = w->create2DPlot();



    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QwtSeriesStore<QPointF>* series = dynamic_cast<QwtSeriesStore<QPointF>*>(item);
        if(nullptr == series)
        {
            continue;
        }
        QString title = item->title().text();
        QVector<double> ys;
        SAChart::getXYDatas(nullptr,&ys,series);
        if(isDetrend)
        {
            saFun::detrendDirect(ys);
        }
        QVector<double> mag,fre;
        saFun::setWindow(ys,window);
        int fftsize = czy::Math::DSP::nextPow2Value(ys.size());//获取最优的fft尺寸
        saFun::spectrum(ys,fs,fftsize,magType,fre,mag);
        QwtPlotCurve * c = chart->addCurve(fre,mag);
        if(c)
        {
            c->setTitle(QString("%1-fft").arg(title));
            newLineNameList.append(c->title().text());
        }
        lineNameList.append(title);
    }
    QString info = TR("fft(data=[\"%1\"],fs=%2,magType=%3,window=%4) -> [figure=\"%5\"]")
            .arg(lineNameList.join(","))
            .arg(fs)
            .arg(magTypeToString(magType))
            .arg(windowTypeToString(window))
            .arg(newLineNameList.join(","))
            ;
    saUI->showNormalMessageInfo(info);
    sub->show();
}
///
/// \brief 获取频谱分析的设置
/// \return
///
bool FunDsp::getSpectrumProperty(double* samFre
                                 ,int* fftsize
                                 , czy::Math::DSP::SpectrumType* magType
                                 ,czy::Math::DSP::WindowType* window
                                 ,bool* isDetrend
                                 ,size_t dataSize)
{
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    QtVariantProperty* tmp = nullptr;
    if(samFre)
    {
        tmp = dlg.appendDoubleProperty("fs",TR("sample frequency(Hz)")
                                 ,0,std::numeric_limits<double>::max()
                                 ,1024,TR("sample frequency"));
    }
    if(fftsize)
    {
        tmp = dlg.appendIntProperty("fftsize",TR("fft size")
                              ,0,std::numeric_limits<int>::max()
                              ,dataSize,TR("fft size"));
    }
    if(magType)
    {
        tmp = dlg.appendEnumProperty("amptype",TR("amplitude type"),{TR("Magnitude")
                                                           ,TR("MagnitudeDB")
                                                           ,TR("Amplitude")
                                                           ,TR("AmplitudeDB")}
                               ,2
                               ,TR("how to deal amplitude"));
    }
    if(window)
    {
        tmp = dlg.appendEnumProperty("windowtype",TR("window type"),{TR("Rect")
                                                       ,TR("Hanning")
                                                       ,TR("Hamming")
                                                       ,TR("Blackman")
                                                       ,TR("Bartlett")}
                               ,0
                               ,TR("set window to wave"));
    }
    if(isDetrend)
    {
        tmp = dlg.appendBoolProperty("detrend",TR("is detrend"),true,TR("if this is set true,the data will sub the mean value"));
    }

    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    if(magType)
    {
        *magType = czy::Math::DSP::Amplitude;
        switch(dlg.getDataByID<int>("amptype"))//dlg.getData(2).toInt())
        {
            case 0:*magType = czy::Math::DSP::Magnitude;break;
            case 1:*magType = czy::Math::DSP::MagnitudeDB;break;
            case 2:*magType = czy::Math::DSP::Amplitude;break;
            case 3:*magType = czy::Math::DSP::AmplitudeDB;break;
            default:*magType = czy::Math::DSP::Amplitude;break;
        }
    }
    if(window)
    {
        *window = czy::Math::DSP::WindowRect;
        switch(dlg.getDataByID<int>("windowtype"))//dlg.getData(2).toInt())
        {
            case 0:*window = czy::Math::DSP::WindowRect;break;
            case 1:*window = czy::Math::DSP::WindowHanning;break;
            case 2:*window = czy::Math::DSP::WindowHamming;break;
            case 3:*window = czy::Math::DSP::WindowBlackman;break;
            case 4:*window = czy::Math::DSP::WindowBartlett;break;
            default:*window = czy::Math::DSP::WindowRect;break;
        }
    }
    if(isDetrend)
        *isDetrend = dlg.getDataByID<bool>("detrend");
    if(samFre)
        *samFre = dlg.getDataByID<double>("fs");
    if(fftsize)
        *fftsize = dlg.getDataByID<int>("fftsize");
    return true;
}

///
/// \brief FunDsp::powerSpectrumInValue
///
void FunDsp::powerSpectrumInValue()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(data->getDim() != 1)
    {
        saUI->showWarningMessageInfo(TR("data:[\"%1\"] type is not accept to psd").arg(data->getName()));
        saUI->raiseMessageInfoDock();
        return;
    }
    int dataSize = data->getSize(SA::Dim1);
    if(dataSize <= 1)
    {
        saUI->showWarningMessageInfo(TR("data:[\"%1\"] size is too short").arg(data->getName()));
        saUI->raiseMessageInfoDock();
        return;
    }



    czy::Math::DSP::PowerDensityWay dspType = czy::Math::DSP::MSA;
    czy::Math::DSP::WindowType window = czy::Math::DSP::WindowRect;
    double fs=100;
    double ti=0.01;
    bool isDetrend = false;
    int fftsize = 100;
    if(!getPowerSpectrumProperty(&fs,&fftsize,&window,&dspType,&ti,&isDetrend,dataSize))
    {
        return;
    }
    std::shared_ptr<SAAbstractDatas> preTrendData = nullptr;
    if(isDetrend)
    {
        preTrendData = saFun::detrendDirect(preTrendData.get() ? preTrendData.get() : data);
        if(nullptr == preTrendData)
        {
            saUI->showErrorMessageInfo(saFun::getLastErrorString());
            saUI->raiseMessageInfoDock();
            return;
        }
    }
    if(czy::Math::DSP::WindowRect != window)//窗函数设置
    {
        preTrendData = saFun::setWindow(preTrendData.get() ? preTrendData.get() : data
                                                             ,window);
        if(nullptr == preTrendData)
        {
            saUI->showErrorMessageInfo(saFun::getLastErrorString());
            saUI->raiseMessageInfoDock();
            return;
        }
    }
    std::shared_ptr<SAVectorDouble> fre = nullptr;
    std::shared_ptr<SAVectorDouble> mag = nullptr;
    std::tie(fre,mag) = saFun::powerSpectrum(preTrendData.get() ? preTrendData.get() : data
                                             ,fs
                                             ,fftsize
                                             ,dspType
                                             ,ti);
    if(nullptr == fre || nullptr == mag)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    saValueManager->addData(fre);
    saValueManager->addData(mag);
    QString info;
    if(czy::Math::DSP::TISA == dspType)
    {
        info = TR("psd(data=[\"%1\"], fftsize=%2,fs=%3,window=%4,psdType=%5,ti=%6) -> [fre=\"%7\",mag=\"%8\"]")
                .arg(data->getName())
                .arg(fftsize)
                .arg(fs)
                .arg(windowTypeToString(window))
                .arg(psdTypeToString(dspType))
                .arg(ti)
                .arg(fre->getName())
                .arg(mag->getName())
                ;
    }
    else
    {
        info = TR("psd(data=[\"%1\"], fftsize=%2,fs=%3,window=%4,psdType=%5) -> [fre=\"%6\",mag=\"%7\"]")
                .arg(data->getName())
                .arg(fftsize)
                .arg(fs)
                .arg(windowTypeToString(window))
                .arg(psdTypeToString(dspType))
                .arg(fre->getName())
                .arg(mag->getName())
                ;
    }
    saUI->showNormalMessageInfo(info);
}

void FunDsp::powerSpectrumInChart()
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        saUI->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    czy::Math::DSP::PowerDensityWay dspType = czy::Math::DSP::MSA;
    czy::Math::DSP::WindowType window = czy::Math::DSP::WindowRect;
    double fs=100;
    double ti=0.01;
    bool isDetrend = false;
    if(!getPowerSpectrumProperty(&fs,nullptr,&window,&dspType,&ti,&isDetrend))
    {
        return;
    }
    QStringList lineNameList,newLineNameList;
    saUI->raiseMainDock();
    QMdiSubWindow* sub = saUI->createFigureWindow(QString("%1-fft").arg(saUI->getCurrentActiveSubWindow()->windowTitle()));
    SAFigureWindow* w = saUI->getFigureWidgetFromMdiSubWindow(sub);
    chart = w->create2DPlot();
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QwtSeriesStore<QPointF>* series = dynamic_cast<QwtSeriesStore<QPointF>*>(item);
        if(nullptr == series)
        {
            continue;
        }
        QString title = item->title().text();
        QVector<double> ys;
        SAChart::getXYDatas(nullptr,&ys,series);
        if(isDetrend)
        {
            saFun::detrendDirect(ys);
        }
        QVector<double> mag,fre;
        saFun::setWindow(ys,window);
        int fftsize = czy::Math::DSP::nextPow2Value(ys.size());//获取最优的fft尺寸
        saFun::powerSpectrum(ys,fs,fftsize,dspType,fre,mag,ti);
        QwtPlotCurve * c = chart->addCurve(fre,mag);
        if(c)
        {
            c->setTitle(QString("%1-psd").arg(title));
            newLineNameList.append(c->title().text());
        }
        lineNameList.append(title);
    }
    QString info;
    if(czy::Math::DSP::TISA == dspType)
    {
        info = TR("psd(data=[\"%1\"],fs=%2,window=%3,psdType=%4,ti=%5) -> [figure=\"%6\"]")
                .arg(lineNameList.join(","))
                .arg(fs)
                .arg(windowTypeToString(window))
                .arg(psdTypeToString(dspType))
                .arg(ti)
                .arg(newLineNameList.join(","))
                ;
    }
    else
    {
        info = TR("psd(data=[\"%1\"],fs=%2,window=%3,psdType=%4) -> [figure=\"%5\"]")
                .arg(lineNameList.join(","))
                .arg(fs)
                .arg(windowTypeToString(window))
                .arg(psdTypeToString(dspType))
                .arg(newLineNameList.join(","))
                ;
    }
    saUI->showNormalMessageInfo(info);
    sub->show();
}

bool FunDsp::getPowerSpectrumProperty(double *samFre
                                      , int *fftsize
                                      , czy::Math::DSP::WindowType *window
                                      , czy::Math::DSP::PowerDensityWay *pdw
                                      , double *ti, bool *isDetrend
                                      , size_t dataSize)
{

    QtVariantProperty* tmp = nullptr;
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    if(samFre)
    {
        tmp = dlg.appendDoubleProperty("fs",TR("sample frequency(Hz)")
                                            ,0,std::numeric_limits<double>::max()
                                            ,1024
                                            ,TR("sample frequency(Hz)")
                                       );
    }
    if(fftsize)
    {
        tmp = dlg.appendIntProperty("fftsize",TR("fft size")
                                    ,-1,std::numeric_limits<int>::max()
                                    ,dataSize
                                    ,TR("fft size,if you do not know how to set the fft size , please set 0,sa will auto set to you ")
                                    );
    }
    if(pdw)
    {
        QtVariantProperty * enumProp = nullptr;
        QtVariantProperty * timeInput = nullptr;
        enumProp = dlg.appendEnumProperty("pdw",TR("power density way"),{TR("MSA"),TR("SSA"),TR("TISA")}
                                          ,0
                                          ,TR("MSA:mean squared amplitude :(real^2+imag^2)/n^2 \r\n"
                                              "SSA:sum squared amplitude :(real^2+imag^2)/n \r\n"
                                              "TISA:time-integral squared amplitude :dT*(real^2+imag^2)/n \r\n")
                                          );

        timeInput = dlg.appendDoubleProperty("ti",TR("sampling interval(s)")
                                             ,1.0/1024.0
                                             ,std::numeric_limits<double>::max()
                                             ,1024,TR("only use TISA should set sampling interval")
                                             );
        timeInput->setEnabled(false);
        dlg.setPropertyChangEvent(enumProp,[timeInput](SAPropertySetDialog* dlg,QtProperty* prop,const QVariant& var){
            Q_UNUSED(dlg);
            Q_UNUSED(prop);
            bool isOK = false;
            int pdw = var.toInt(&isOK);
            if(!isOK)
            {
                return;
            }
            timeInput->setEnabled(2 == pdw);
        });
    }
    if(isDetrend)
    {
        tmp = dlg.appendBoolProperty("detrend"
                                     ,TR("is detrend")
                                     ,true
                                     ,TR("if this is set true,the data will sub the mean value")
                                     );
    }
    if(window)
    {
        tmp = dlg.appendEnumProperty("windowtype"
                                     ,TR("window type")
                                     ,{TR("Rect"),TR("Hanning"),TR("Hamming"),TR("Blackman"),TR("Bartlett")}
                                     ,0
                                     ,TR("set window to wave")
                                     );
    }


    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    if(pdw)
    {
        switch(dlg.getDataByID<int>("pdw"))//dlg.getData(2).toInt())
        {
            case 0:*pdw = czy::Math::DSP::MSA;break;
            case 1:*pdw = czy::Math::DSP::SSA;break;
            case 2:*pdw = czy::Math::DSP::TISA;break;
            default:*pdw = czy::Math::DSP::MSA;break;
        }
    }
    if(window)
    {
        switch(dlg.getDataByID<int>("windowtype"))//dlg.getData(2).toInt())
        {
            case 0:*window = czy::Math::DSP::WindowRect;break;
            case 1:*window = czy::Math::DSP::WindowHanning;break;
            case 2:*window = czy::Math::DSP::WindowHamming;break;
            case 3:*window = czy::Math::DSP::WindowBlackman;break;
            case 4:*window = czy::Math::DSP::WindowBartlett;break;
            default:*window = czy::Math::DSP::WindowRect;break;
        }
    }
    if(isDetrend)
        *isDetrend = dlg.getDataByID<bool>("detrend");
    if(samFre)
        *samFre = dlg.getDataByID<double>("fs");
    if(fftsize)
        *fftsize = dlg.getDataByID<int>("fftsize");
    if(ti)
        *ti = dlg.getDataByID<double>("ti");
    return true;
}

///
/// \brief 时频分析工具箱
///
void FunDsp::tmeFrequency()
{
    if(nullptr == timeFrequencyWidget)
    {
        timeFrequencyWidget.reset(new SATimeFrequencyAnalysis());
    }
    timeFrequencyWidget->show();
    timeFrequencyWidget->raise();
}


///
/// \brief 获取设置窗的属性
///
bool FunDsp::getWindowProperty(czy::Math::DSP::WindowType & windowType, bool &isDetrend)
{
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    QtVariantProperty* tmp = dlg.appendEnumProperty("windowtype",TR("window type"),{TR("Rect")
                                                   ,TR("Hanning")
                                                   ,TR("Hamming")
                                                   ,TR("Blackman")
                                                   ,TR("Bartlett")}
                           ,0
                           ,TR("set window to wave"));
    tmp = dlg.appendBoolProperty("detrend",TR("is detrend"),true,TR("if this is set true,the data will sub the mean value"));
    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    windowType = czy::Math::DSP::WindowRect;
    switch(dlg.getDataByID<int>("windowtype"))//dlg.getData(2).toInt())
    {
        case 0:windowType = czy::Math::DSP::WindowRect;break;
        case 1:windowType = czy::Math::DSP::WindowHanning;break;
        case 2:windowType = czy::Math::DSP::WindowHamming;break;
        case 3:windowType = czy::Math::DSP::WindowBlackman;break;
        case 4:windowType = czy::Math::DSP::WindowBartlett;break;
        default:windowType = czy::Math::DSP::WindowRect;break;
    }
    isDetrend = dlg.getDataByID<bool>("detrend");
    return true;
}

QString FunDsp::windowTypeToString(czy::Math::DSP::WindowType windowType)
{
    return saFun::windowName(windowType);
}

QString FunDsp::magTypeToString(czy::Math::DSP::SpectrumType magType)
{
    switch(magType)//dlg.getData(2).toInt())
    {
    case czy::Math::DSP::Magnitude:return "Magnitude";
    case czy::Math::DSP::MagnitudeDB:return "MagnitudeDB";
    case czy::Math::DSP::Amplitude:return "Amplitude";
    case czy::Math::DSP::AmplitudeDB:return "AmplitudeDB";
    default:return "UnKnow";
    }
    return "UnKnow";
}

QString FunDsp::psdTypeToString(czy::Math::DSP::PowerDensityWay psd)
{
    switch(psd)//dlg.getData(2).toInt())
    {
    case czy::Math::DSP::MSA:return QString("MSA");
    case czy::Math::DSP::SSA:return QString("SSA");
    case czy::Math::DSP::TISA:return QString("TISA");
    default: return TR("UnKnow");
    }
    return TR("UnKnow");
}
