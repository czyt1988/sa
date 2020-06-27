#include "FunDsp.h"
#include <QApplication>
#include <QVariant>
#include <QMdiSubWindow>
#include "sa_fun_dsp.h"
#include "SAPropertySetDialog.h"
#include <qtvariantproperty.h>
#include "SAUIInterface.h"
#include "SAAbstractDatas.h"
#include "SAVariantDatas.h"
#include "SAValueManager.h"
#include "SAFigureWindow.h"
#include "SATimeFrequencyAnalysis.h"
#include "SAGUIGlobalConfig.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAMdiSubWindow.h"
#include "SAFigureOptCommands.h"
#include "SAFigureReplaceDatasCommand.h"
#include "SAMath.h"
#include "ui_opt.h"
#include <QApplication>
#include "SADsp.h"
#define TR(str)\
    QApplication::translate("FunDSP", str, 0)

std::unique_ptr<SATimeFrequencyAnalysis> g_timeFrequencyWidget = nullptr;

void detrendDirectInChart(SAUIInterface* ui);
void detrendDirectInValue(SAUIInterface* ui);
//信号设置窗
void setWindowToWaveInValue(SAUIInterface* ui);
void setWindowToWaveInChart(SAUIInterface* ui);
//频谱分析
void spectrumInChart(SAUIInterface* ui);
void spectrumInValue(SAUIInterface* ui);
//功率谱分析
void powerSpectrumInValue(SAUIInterface* ui);
void powerSpectrumInChart(SAUIInterface* ui);


bool getSpectrumProperty(double *samFre
                                , int *fftsize
                                , SA::SADsp::SpectrumType *magType
                                , SA::SADsp::WindowType *window
                                , bool *isDetrend
                                , size_t dataSize
                                , SAUIInterface *ui
                         );
bool getPowerSpectrumProperty(double *samFre
                            , int *fftsize
                            , SA::SADsp::WindowType *window
                            , SA::SADsp::PowerDensityWay* pdw
                            , double* ti
                            , bool *isDetrend
                            , size_t dataSize
                            ,SAUIInterface* ui
                                 );
bool getWindowProperty(SA::SADsp::WindowType &windowType
                       ,bool& isDetrend
                       , SAUIInterface *ui
                       );
QString windowTypeToString(SA::SADsp::WindowType windowType);
QString magTypeToString(SA::SADsp::SpectrumType magType);
QString psdTypeToString(SA::SADsp::PowerDensityWay psd);

///
/// \brief 应用于数据的去趋势
/// \param ui
///
void detrendDirectInValue(SAUIInterface* ui)
{
    SAAbstractDatas* data = ui->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    std::shared_ptr<SAAbstractDatas> res = saFun::detrendDirect(data);
    if(nullptr == res)
    {
        ui->showErrorMessageInfo(saFun::getLastErrorString());
        return;
    }
    QString info = TR("direct detrend :%1").arg(data->getName());
    saValueManager->addData(res);
    ui->showMessageInfo(info,SA::NormalMessage);
}
///
/// \brief 应用于图表的去趋势
/// \param ui
///
void detrendDirectInChart(SAUIInterface* ui)
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(ui,curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        ui->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    QString info = TR("direct detrend :");
    SAFigureOptCommand* topCmd = new SAFigureOptCommand(chart,TR("detrend Direct"));
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            QVector<QPointF> newData;
            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            SAChart::getXYDatas(newData,cur);
            SA::sub_mean(newData.begin(),newData.end()
                                ,[](QPointF& p)->double&{return p.ry();});
            new SAFigureReplaceAllDatasCommand<QPointF,QwtPlotCurve,decltype(&SAChart::setPlotCurveSample)>
                    (chart
                   ,item
                   ,newData
                   ,TR("detrend Direct %1").arg(item->title().text())
                   ,&SAChart::setPlotCurveSample
                   ,&SAChart::getPlotCurveSample
                   ,topCmd
                   );
            break;
        }
        }







/*
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
        */
    }
    chart->appendCommand(topCmd);
    ui->showMessageInfo(info,SA::NormalMessage);
}

///
/// \brief 信号设置窗
///
void setWindowToWaveInValue(SAUIInterface* ui)
{
    SAAbstractDatas* data = ui->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(data->getDim() != 1)
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] type is not accept to set window").arg(data->getName()));
        return;
    }
    int dataSize = data->getSize(SA::Dim1);
    if(dataSize <= 1)
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] size is too short").arg(data->getName()));
        return;
    }

    bool isDetrend = false;
    SA::SADsp::WindowType window = SA::SADsp::WindowRect;
    if(!getWindowProperty(window,isDetrend,ui))
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
        ui->showErrorMessageInfo(saFun::getLastErrorString());
        ui->raiseMessageInfoDock();
        return;
    }
    saValueManager->addData(res);
    ui->showNormalMessageInfo(TR("data:[\"%1\"] set window(%2)] -> [\"%3\"]")
                                .arg(data->getName())
                                .arg(windowTypeToString(window))
                                .arg(res->getName()));
}
///
/// \brief 信号设置窗
///
void setWindowToWaveInChart(SAUIInterface* ui)
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(ui,curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        ui->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    bool isDetrend = false;
    SA::SADsp::WindowType window = SA::SADsp::WindowRect;
    if(!getWindowProperty(window,isDetrend,ui))
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
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {

            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            QVector<QPointF> newData;
            {
                QVector<double> xs,ys;
                SAChart::getXYDatas(&xs,&ys,cur);
                saFun::setWindow(ys,window);
                if(isDetrend)
                {
                    saFun::detrendDirect(ys);
                }
                saFun::setWindow(ys,window);
                saFun::makeVectorPointF(xs,ys,newData);
            }
            new SAFigureReplaceAllDatasCommand<QPointF,QwtPlotCurve,decltype(&SAChart::setPlotCurveSample)>
                    (chart
                   ,item
                   ,newData
                   ,TR("%1 set %2").arg(item->title().text()).arg(windowName)
                   ,&SAChart::setPlotCurveSample
                   ,&SAChart::getPlotCurveSample
                   ,topCmd
                   );
            break;
        }
        }
        /*
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
        */
    }
    chart->appendCommand(topCmd);
    ui->showMessageInfo(info,SA::NormalMessage);
}


void powerSpectrum(SAUIInterface* ui)
{
    SAUIInterface::LastFocusType ft = ui->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        powerSpectrumInChart(ui);
    }
    else
    {
        powerSpectrumInValue(ui);
    }
}

void spectrumInValue(SAUIInterface* ui)
{
    SAAbstractDatas* data = ui->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(data->getDim() != 1)
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] type is not accept to spectrum").arg(data->getName()));
        ui->raiseMessageInfoDock();
        return;
    }
    int dataSize = data->getSize(SA::Dim1);
    if(dataSize <= 1)
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] size is too short").arg(data->getName()));
        ui->raiseMessageInfoDock();
        return;
    }
    int fftsize = 100;
    double fs=100;
    bool isDetrend = false;
    SA::SADsp::SpectrumType magType = SA::SADsp::Amplitude;
    SA::SADsp::WindowType window = SA::SADsp::WindowRect;
    if(!getSpectrumProperty(&fs,&fftsize,&magType,&window,&isDetrend,dataSize,ui))
    {
        return;
    }


    std::shared_ptr<SAAbstractDatas> preTrendData = nullptr;
    if(isDetrend)//dlg.getData(3).toBool())
    {
        preTrendData = saFun::detrendDirect(preTrendData.get() ? preTrendData.get() : data);
        if(nullptr == preTrendData)
        {
            ui->showErrorMessageInfo(saFun::getLastErrorString());
            ui->raiseMessageInfoDock();
            return;
        }
    }
    if(SA::SADsp::WindowRect != window)//窗函数设置
    {
        preTrendData = saFun::setWindow(preTrendData.get() ? preTrendData.get() : data
                                                             ,window);
        if(nullptr == preTrendData)
        {
            ui->showErrorMessageInfo(saFun::getLastErrorString());
            ui->raiseMessageInfoDock();
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
        ui->showErrorMessageInfo(saFun::getLastErrorString());
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
    ui->showNormalMessageInfo(info);
}

void spectrumInChart(SAUIInterface* ui)
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(ui,curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        ui->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }

    double fs=100;
    bool isDetrend = false;
    SA::SADsp::SpectrumType magType = SA::SADsp::Amplitude;
    SA::SADsp::WindowType window = SA::SADsp::WindowRect;
    //绘图的参数里，没有fftsize
    if(!getSpectrumProperty(&fs,nullptr,&magType,&window,&isDetrend,0,ui))
    {
        return;
    }
    QStringList lineNameList,newLineNameList;
    ui->raiseMainDock();
    SAMdiSubWindow* sub = ui->createFigureWindow(QString("%1-fft").arg(ui->getCurrentActiveSubWindow()->windowTitle()));
    SAFigureWindow* w = ui->getFigureWidgetFromMdiSubWindow(sub);
    chart = w->create2DPlot();



    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QString title = item->title().text();
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            QVector<double> ys;
            {
                QVector<QPointF> wave;
                SAChart::getPlotCurveSample(item,wave);
                ys.reserve(wave.size());
                std::for_each(wave.begin(),wave.end(),[&ys](const QPointF& p){ys.push_back(p.y());});
            }
            if(isDetrend)
            {
                saFun::detrendDirect(ys);
            }
            QVector<double> mag,fre;
            saFun::setWindow(ys,window);
            int fftsize = SA::SADsp::nextPow2Value(ys.size());//获取最优的fft尺寸
            saFun::spectrum(ys,fs,fftsize,magType,fre,mag);
            QwtPlotCurve * c = chart->addCurve(fre,mag);
            if(c)
            {
                c->setTitle(QString("%1-fft").arg(title));
                newLineNameList.append(c->title().text());
            }
            lineNameList.append(title);
            break;
        }
        }

        /*
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
        int fftsize = czy::SADsp::nextPow2Value(ys.size());//获取最优的fft尺寸
        saFun::spectrum(ys,fs,fftsize,magType,fre,mag);
        QwtPlotCurve * c = chart->addCurve(fre,mag);
        if(c)
        {
            c->setTitle(QString("%1-fft").arg(title));
            newLineNameList.append(c->title().text());
        }
        lineNameList.append(title);
        */
    }
    QString info = TR("fft(data=[\"%1\"],fs=%2,magType=%3,window=%4) -> [figure=\"%5\"]")
            .arg(lineNameList.join(","))
            .arg(fs)
            .arg(magTypeToString(magType))
            .arg(windowTypeToString(window))
            .arg(newLineNameList.join(","))
            ;
    ui->showNormalMessageInfo(info);
    sub->show();
}
///
/// \brief 获取频谱分析的设置
/// \return
///
bool getSpectrumProperty(double* samFre
                         ,int* fftsize
                         , SA::SADsp::SpectrumType* magType
                         ,SA::SADsp::WindowType* window
                         ,bool* isDetrend
                         ,size_t dataSize
                         ,SAUIInterface* ui
                         )
{
    SAPropertySetDialog dlg(ui->getMainWindowPtr(),SAPropertySetDialog::GroupBoxType);
    dlg.appendGroup(TR("property set"));
    if(samFre)
    {
        dlg.appendDoubleProperty("fs",TR("sample frequency(Hz)")
                                 ,0,std::numeric_limits<double>::max()
                                 ,1024,TR("sample frequency"));
    }
    if(fftsize)
    {
        dlg.appendIntProperty("fftsize",TR("fft size")
                              ,0,std::numeric_limits<int>::max()
                              ,dataSize,TR("fft size"));
    }
    if(magType)
    {
        dlg.appendEnumProperty("amptype",TR("amplitude type"),{TR("Magnitude")
                                                           ,TR("MagnitudeDB")
                                                           ,TR("Amplitude")
                                                           ,TR("AmplitudeDB")}
                               ,2
                               ,TR("how to deal amplitude"));
    }
    if(window)
    {
        dlg.appendEnumProperty("windowtype",TR("window type"),{TR("Rect")
                                                       ,TR("Hanning")
                                                       ,TR("Hamming")
                                                       ,TR("Blackman")
                                                       ,TR("Bartlett")}
                               ,0
                               ,TR("set window to wave"));
    }
    if(isDetrend)
    {
        dlg.appendBoolProperty("detrend",TR("is detrend"),true,TR("if this is set true,the data will sub the mean value"));
    }

    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    if(magType)
    {
        *magType = SA::SADsp::Amplitude;
        switch(dlg.getDataByID<int>("amptype"))//dlg.getData(2).toInt())
        {
            case 0:*magType = SA::SADsp::Magnitude;break;
            case 1:*magType = SA::SADsp::MagnitudeDB;break;
            case 2:*magType = SA::SADsp::Amplitude;break;
            case 3:*magType = SA::SADsp::AmplitudeDB;break;
            default:*magType = SA::SADsp::Amplitude;break;
        }
    }
    if(window)
    {
        *window = SA::SADsp::WindowRect;
        switch(dlg.getDataByID<int>("windowtype"))//dlg.getData(2).toInt())
        {
            case 0:*window = SA::SADsp::WindowRect;break;
            case 1:*window = SA::SADsp::WindowHanning;break;
            case 2:*window = SA::SADsp::WindowHamming;break;
            case 3:*window = SA::SADsp::WindowBlackman;break;
            case 4:*window = SA::SADsp::WindowBartlett;break;
            default:*window = SA::SADsp::WindowRect;break;
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
void powerSpectrumInValue(SAUIInterface* ui)
{
    SAAbstractDatas* data = ui->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(data->getDim() != 1)
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] type is not accept to psd").arg(data->getName()));
        ui->raiseMessageInfoDock();
        return;
    }
    int dataSize = data->getSize(SA::Dim1);
    if(dataSize <= 1)
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] size is too short").arg(data->getName()));
        ui->raiseMessageInfoDock();
        return;
    }



    SA::SADsp::PowerDensityWay dspType = SA::SADsp::MSA;
    SA::SADsp::WindowType window = SA::SADsp::WindowRect;
    double fs=100;
    double ti=0.01;
    bool isDetrend = false;
    int fftsize = 100;
    if(!getPowerSpectrumProperty(&fs,&fftsize,&window,&dspType,&ti,&isDetrend,dataSize,ui))
    {
        return;
    }
    std::shared_ptr<SAAbstractDatas> preTrendData = nullptr;
    if(isDetrend)
    {
        preTrendData = saFun::detrendDirect(preTrendData.get() ? preTrendData.get() : data);
        if(nullptr == preTrendData)
        {
            ui->showErrorMessageInfo(saFun::getLastErrorString());
            ui->raiseMessageInfoDock();
            return;
        }
    }
    if(SA::SADsp::WindowRect != window)//窗函数设置
    {
        preTrendData = saFun::setWindow(preTrendData.get() ? preTrendData.get() : data
                                                             ,window);
        if(nullptr == preTrendData)
        {
            ui->showErrorMessageInfo(saFun::getLastErrorString());
            ui->raiseMessageInfoDock();
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
        ui->showErrorMessageInfo(saFun::getLastErrorString());
        ui->raiseMessageInfoDock();
        return;
    }
    saValueManager->addData(fre);
    saValueManager->addData(mag);
    QString info;
    if(SA::SADsp::TISA == dspType)
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
    ui->showNormalMessageInfo(info);
}

void powerSpectrumInChart(SAUIInterface* ui)
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(ui,curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        ui->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    SA::SADsp::PowerDensityWay dspType = SA::SADsp::MSA;
    SA::SADsp::WindowType window = SA::SADsp::WindowRect;
    double fs=100;
    double ti=0.01;
    bool isDetrend = false;
    if(!getPowerSpectrumProperty(&fs,nullptr,&window,&dspType,&ti,&isDetrend,0,ui))
    {
        return;
    }
    QStringList lineNameList,newLineNameList;
    ui->raiseMainDock();
    SAMdiSubWindow* sub = ui->createFigureWindow(QString("%1-fft").arg(ui->getCurrentActiveSubWindow()->windowTitle()));
    SAFigureWindow* w = ui->getFigureWidgetFromMdiSubWindow(sub);
    chart = w->create2DPlot();
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QString title = item->title().text();
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            QVector<double> ys;
            {
                QVector<QPointF> wave;
                SAChart::getPlotCurveSample(item,wave);
                ys.reserve(wave.size());
                std::for_each(wave.begin(),wave.end(),[&ys](const QPointF& p){ys.push_back(p.y());});
            }
            if(isDetrend)
            {
                saFun::detrendDirect(ys);
            }
            QVector<double> mag,fre;
            saFun::setWindow(ys,window);
            int fftsize = SA::SADsp::nextPow2Value(ys.size());//获取最优的fft尺寸
            saFun::powerSpectrum(ys,fs,fftsize,dspType,fre,mag,ti);
            QwtPlotCurve * c = chart->addCurve(fre,mag);
            if(c)
            {
                c->setTitle(QString("%1-psd").arg(title));
                newLineNameList.append(c->title().text());
            }
            lineNameList.append(title);
            break;
        }
        }
        /*
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
        int fftsize = czy::SADsp::nextPow2Value(ys.size());//获取最优的fft尺寸
        saFun::powerSpectrum(ys,fs,fftsize,dspType,fre,mag,ti);
        QwtPlotCurve * c = chart->addCurve(fre,mag);
        if(c)
        {
            c->setTitle(QString("%1-psd").arg(title));
            newLineNameList.append(c->title().text());
        }
        lineNameList.append(title);
        */
    }
    QString info;
    if(SA::SADsp::TISA == dspType)
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
    ui->showNormalMessageInfo(info);
    sub->show();
}

bool getPowerSpectrumProperty(double *samFre
                                      , int *fftsize
                                      , SA::SADsp::WindowType *window
                                      , SA::SADsp::PowerDensityWay *pdw
                                      , double *ti, bool *isDetrend
                                      , size_t dataSize
                                      , SAUIInterface* ui)
{
    SAPropertySetDialog dlg(ui->getMainWindowPtr(),SAPropertySetDialog::GroupBoxType);
    dlg.appendGroup(TR("property set"));
    if(samFre)
    {
        dlg.appendDoubleProperty("fs",TR("sample frequency(Hz)")
                                            ,0,std::numeric_limits<double>::max()
                                            ,1024
                                            ,TR("sample frequency(Hz)")
                                       );
    }
    if(fftsize)
    {
        dlg.appendIntProperty("fftsize",TR("fft size")
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
        dlg.appendBoolProperty("detrend"
                                     ,TR("is detrend")
                                     ,true
                                     ,TR("if this is set true,the data will sub the mean value")
                                     );
    }
    if(window)
    {
        dlg.appendEnumProperty("windowtype"
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
            case 0:*pdw = SA::SADsp::MSA;break;
            case 1:*pdw = SA::SADsp::SSA;break;
            case 2:*pdw = SA::SADsp::TISA;break;
            default:*pdw = SA::SADsp::MSA;break;
        }
    }
    if(window)
    {
        switch(dlg.getDataByID<int>("windowtype"))//dlg.getData(2).toInt())
        {
            case 0:*window = SA::SADsp::WindowRect;break;
            case 1:*window = SA::SADsp::WindowHanning;break;
            case 2:*window = SA::SADsp::WindowHamming;break;
            case 3:*window = SA::SADsp::WindowBlackman;break;
            case 4:*window = SA::SADsp::WindowBartlett;break;
            default:*window = SA::SADsp::WindowRect;break;
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
void tmeFrequency(SAUIInterface* ui)
{
    Q_UNUSED(ui);
    if(nullptr == g_timeFrequencyWidget)
    {
        g_timeFrequencyWidget.reset(new SATimeFrequencyAnalysis());
    }
    g_timeFrequencyWidget->show();
    g_timeFrequencyWidget->raise();
}


///
/// \brief 获取设置窗的属性
///
bool getWindowProperty(SA::SADsp::WindowType & windowType, bool &isDetrend, SAUIInterface *ui)
{
    SAPropertySetDialog dlg(ui->getMainWindowPtr(),SAPropertySetDialog::GroupBoxType);
    dlg.appendGroup(TR("property set"));
    dlg.appendEnumProperty("windowtype",TR("window type"),{TR("Rect")
                                                   ,TR("Hanning")
                                                   ,TR("Hamming")
                                                   ,TR("Blackman")
                                                   ,TR("Bartlett")}
                           ,0
                           ,TR("set window to wave"));
    dlg.appendBoolProperty("detrend",TR("is detrend"),true,TR("if this is set true,the data will sub the mean value"));
    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    windowType = SA::SADsp::WindowRect;
    switch(dlg.getDataByID<int>("windowtype"))//dlg.getData(2).toInt())
    {
        case 0:windowType = SA::SADsp::WindowRect;break;
        case 1:windowType = SA::SADsp::WindowHanning;break;
        case 2:windowType = SA::SADsp::WindowHamming;break;
        case 3:windowType = SA::SADsp::WindowBlackman;break;
        case 4:windowType = SA::SADsp::WindowBartlett;break;
        default:windowType = SA::SADsp::WindowRect;break;
    }
    isDetrend = dlg.getDataByID<bool>("detrend");
    return true;
}

QString windowTypeToString(SA::SADsp::WindowType windowType)
{
    return saFun::windowName(windowType);
}

QString magTypeToString(SA::SADsp::SpectrumType magType)
{
    switch(magType)//dlg.getData(2).toInt())
    {
    case SA::SADsp::Magnitude:return TR("Magnitude");
    case SA::SADsp::MagnitudeDB:return TR("MagnitudeDB");
    case SA::SADsp::Amplitude:return TR("Amplitude");
    case SA::SADsp::AmplitudeDB:return TR("AmplitudeDB");
    default:return TR("UnKnow");
    }
    return TR("UnKnow");
}

QString psdTypeToString(SA::SADsp::PowerDensityWay psd)
{
    switch(psd)//dlg.getData(2).toInt())
    {
    case SA::SADsp::MSA:return TR("MSA");
    case SA::SADsp::SSA:return TR("SSA");
    case SA::SADsp::TISA:return TR("TISA");
    default: return TR("UnKnow");
    }
    return TR("UnKnow");
}

void detrendDirect(SAUIInterface *ui)
{
    SAUIInterface::LastFocusType ft = ui->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        detrendDirectInChart(ui);
    }
    else
    {
        detrendDirectInValue(ui);
    }
}

void setWindow(SAUIInterface *ui)
{
    SAUIInterface::LastFocusType ft = ui->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        setWindowToWaveInChart(ui);
    }
    else
    {
        setWindowToWaveInValue(ui);
    }
}

void spectrum(SAUIInterface *ui)
{
    SAUIInterface::LastFocusType ft = ui->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        spectrumInChart(ui);
    }
    else
    {
        spectrumInValue(ui);
    }
}
