#include "DataFeatureWidget.h"
#include "ui_DataFeatureWidget.h"
#include <QMainWindow>
#include <QMdiSubWindow>

//#include <SAChartWidget.h>
#include <DataFeatureTreeModel.h>
#include <DataFeatureItem.h>
#include <SAPlotMarker.h>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QLocalServer>
#include <QLocalSocket>

#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SALog.h"

#include "SALocalServeFigureItemProcessHeader.h"
#include "SALocalServerDefine.h"
#include "SALocalServeReader.h"
#include "SALocalServeWriter.h"

#include "qwt_plot_curve.h"

#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#include <QDebug>
QElapsedTimer s_vector_send_time_elaspade = QElapsedTimer();
#endif


//====================================================================

DataFeatureWidget::DataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFeatureWidget)
  ,m_lastActiveSubWindow(nullptr)
  ,m_dataProcessSocket(nullptr)
  ,m_dataProcPro(nullptr)
  ,m_dataReader(nullptr)
  ,m_dataWriter(nullptr)
{
    ui->setupUi(this);
    initLocalServer();
}

DataFeatureWidget::~DataFeatureWidget()
{
    disconnect(m_dataProcPro,static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
            ,this,&DataFeatureWidget::onProcessDataProcFinish);
    m_dataProcPro->kill();
    delete ui;
}
///
/// \brief 子窗口激活槽
/// \param arg1
///
void DataFeatureWidget::mdiSubWindowActived(QMdiSubWindow *arg1)
{
#if 0
    if(!arg1)
        return;
    m_plotWidget = getChartWidgetFromSubWindow(arg1);

    if(m_plotWidget)
    {
        getTreeModel()->setDataFeatureItems(m_plotWidget->getDataFeatureItemList());
        ui->treeView->expandAll();
    }
    else
        getTreeModel()->clear();
#endif
    if(nullptr == arg1)
    {
        return;
    }
    m_lastActiveSubWindow = arg1;

    auto modelIte = m_subWindowToDataInfo.find(arg1);
    if(modelIte != m_subWindowToDataInfo.end())
    {
        if(nullptr != modelIte.value())
            ui->treeView->setModel(modelIte.value());
    }
    else
    {
        //说明没有对应的内容，把数据下发给数据处理进程进行处理
        SAFigureWindow* figure = getChartWidgetFromSubWindow(m_lastActiveSubWindow);//记录当前的绘图窗口
        if(figure)
        {
            callCalcFigureWindowFeature(figure);
        }
    }
}
///
/// \brief 子窗口关闭槽
/// \param arg1
///
void DataFeatureWidget::mdiSubWindowClosed(QMdiSubWindow *arg1)
{

    m_subWindowToDataInfo.remove(arg1);
    auto modelIte = m_subWindowToDataInfo.find(arg1);
    if(modelIte != m_subWindowToDataInfo.end())
    {
        //子窗口关闭，把对应的对应model删除，如果正在显示这个model，先设置为null
        QAbstractItemModel* model = modelIte.value();
        if(ui->treeView->model() == model)
        {
            ui->treeView->setModel(nullptr);
        }
        if(nullptr == model)
            delete model;
        m_subWindowToDataInfo.remove(arg1);
    }
}
///
/// \brief 从子窗口获取绘图窗口
/// \param sub
/// \return
///
SAFigureWindow *DataFeatureWidget::getChartWidgetFromSubWindow(QMdiSubWindow *sub)
{
    return qobject_cast<SAFigureWindow*>(sub->widget());
}
///
/// \brief 计算绘图窗口的dataFeature
/// \param figure 绘图窗口指针，不允许null
///
void DataFeatureWidget::callCalcFigureWindowFeature(SAFigureWindow *figure)
{
#ifdef _DEBUG_OUTPUT
    QElapsedTimer t;
    t.start();
#endif
    if(nullptr == m_dataProcessSocket)
    {
        return;
    }
    QList<SAChart2D*> charts = figure->get2DPlots();
    for(auto i=charts.begin();i!=charts.end();++i)
    {
        QwtPlotItemList itemList = (*i)->itemList();
        for(auto j=itemList.begin();j!=itemList.end();++j)
        {
            if(QwtPlotItem::Rtti_PlotCurve == (*j)->rtti())
            {
                QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(*j);
                const size_t size = cur->dataSize();
                QVector<QPointF> datas;
                datas.reserve(cur->data()->size());
                for(size_t c = 0;c<size;++c)
                {
                    datas.append(cur->sample(c));
                }
                if(m_dataWriter)
                {
#ifdef _DEBUG_OUTPUT
                    QElapsedTimer t2;
                    t2.start();
                    s_vector_send_time_elaspade.start();
#endif
                    m_dataWriter->sendDoubleVectorData((qintptr)figure,(qintptr)cur,datas);
#ifdef _DEBUG_OUTPUT
                    qDebug() << "sendDoubleVectorData time cost:" << t2.elapsed();
#endif
                }
            }
        }

    }
#ifdef _DEBUG_OUTPUT
    qDebug() << "callCalcFigureWindowFeature time cost:" << t.elapsed();
#endif
}

void DataFeatureWidget::onReceivedShakeHand(const SALocalServeBaseHeader &mainHeader)
{
    Q_UNUSED(mainHeader);
#ifdef _DEBUG_OUTPUT
    if(m_dataWriter)
    {
        qDebug() << "start 1000000 points test";
        s_vector_send_time_elaspade.restart();
        m_dataWriter->sendString("test");//发送一个测试
    }
#endif
    emit showMessageInfo(tr("data process connect sucess!"),SA::NormalMessage);
}

void DataFeatureWidget::onReceivedString(const QString &xmlString)
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "receive data cost:"<<s_vector_send_time_elaspade.elapsed()<<"receive str:"<<xmlString;
#endif
}

void DataFeatureWidget::onReceivedVectorDoubleData(const SALocalServeFigureItemProcessHeader &header, QVector<QPointF> &ys)
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "test time cost: " << s_vector_send_time_elaspade.elapsed()
             << "\n ys.size:" <<ys.size()
                ;

#endif
}



///
/// \brief 数据特性树点击
/// \param index
///
void DataFeatureWidget::on_treeView_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    if(nullptr == m_lastActiveSubWindow)
        return;
    SAFigureWindow* figure = getChartWidgetFromSubWindow(m_lastActiveSubWindow);//记录当前的绘图窗口
    if(nullptr == figure)
    {
        return;
    }
    on_toolButton_clearDataFeature_clicked();//先清除标记
    QItemSelectionModel* selModel = ui->treeView->selectionModel();
    if(!selModel)
        return;
    QModelIndexList indexList = selModel->selectedIndexes();
    for(int i=0;i<indexList.size();++i)
    {

        AbstractDataFeatureItem* item = static_cast<AbstractDataFeatureItem*>(indexList[i].internalPointer());
        if(nullptr == item)
        {
            return;
        }
        AbstractDataFeatureItem* topParent = item->topParent();
        if(AbstractDataFeatureItem::TopPlotItem != topParent->rtti())
        {
            return;
        }
        DataFeaturePlotInfoItem* plotItem = static_cast<DataFeaturePlotInfoItem*>(topParent);
        if(plotItem)
        {
            SAChart2D* chart = qobject_cast<SAChart2D*>(plotItem->chartWidget());
            if(!chart)
                return;
            int itemRtti = item->rtti();
            if(AbstractDataFeatureItem::PointItem == itemRtti)
            {
                DataFeaturePointItem* pointItem = static_cast<DataFeaturePointItem*>(item);
                QPointF point = pointItem->getPointData();
                chart->markPoint(point,pointItem->text());
                chart->replot();
            }
            else if(AbstractDataFeatureItem::ValueItem == itemRtti)
            {
                DataFeatureValueItem* valueItem = static_cast<DataFeatureValueItem*>(item);
                DataFeatureDescribeItem* desItem = valueItem->getDescribeItem();
                QString name;
                if(desItem)
                {
                    name = desItem->text();
                }
                double data = valueItem->getValueData();
                chart->markYValue(data,name+valueItem->text());
                chart->replot();
            }
        }

    }


}
///
/// \brief 清除标记按钮
///
void DataFeatureWidget::on_toolButton_clearDataFeature_clicked()
{
    if(nullptr == m_lastActiveSubWindow)
        return;
    SAFigureWindow* figure = getChartWidgetFromSubWindow(m_lastActiveSubWindow);//记录当前的绘图窗口
    if(nullptr == figure)
        return;
    QList<SAChart2D*> charts= figure->get2DPlots();
    std::for_each(charts.begin(),charts.end(),[](SAChart2D* p){
        p->removeAllMarker();
    });

}

void DataFeatureWidget::onShowErrorMessage(const QString &info)
{
    emit showMessageInfo(info,SA::ErrorMessage);
}
///
/// \brief 初始化本地服务器
///
void DataFeatureWidget::initLocalServer()
{
    m_localServer.reset(new QLocalServer);
    connect(m_localServer.data(),&QLocalServer::newConnection
            ,this,&DataFeatureWidget::onLocalServeNewConnection);
    if(!m_localServer->listen(SA_LOCAL_SERVER_DATA_PROC_NAME))
    {
       showMessageInfo(tr("listern loacl server error"),SA::ErrorMessage);
    }
    m_dataProcPro = new QProcess(this);
    connect(m_dataProcPro,static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished)
            ,this,&DataFeatureWidget::onProcessDataProcFinish);
    m_dataReader = new SALocalServeReader(this);
    m_dataWriter = new SALocalServeWriter(this);
    connect(m_dataReader,&SALocalServeReader::receivedShakeHand
            ,this,&DataFeatureWidget::onReceivedShakeHand);
    connect(m_dataReader,&SALocalServeReader::receivedString
            ,this,&DataFeatureWidget::onReceivedString);
    connect(m_dataReader,&SALocalServeReader::receivedVectorDoubleData
            ,this,&DataFeatureWidget::onReceivedVectorDoubleData);
    connect(m_dataReader,&SALocalServeReader::errorOccure
            ,this,&DataFeatureWidget::onShowErrorMessage);
    startDataProc();
}
///
/// \brief 启动数据处理进程
///
void DataFeatureWidget::startDataProc()
{
    if(m_dataProcessSocket)
    {
        delete m_dataProcessSocket;
        m_dataProcessSocket = nullptr;
    }
    QString path = qApp->applicationDirPath()+"/signADataProc.exe";
    QStringList args = {QString::number(qApp->applicationPid())};
    m_dataProcPro->start(path,args);
}
///
/// \brief 本地服务连接的槽
///
void DataFeatureWidget::onLocalServeNewConnection()
{
    if(m_dataProcessSocket)
    {
        delete m_dataProcessSocket;
        m_dataProcessSocket = nullptr;
    }
    m_dataProcessSocket = m_localServer->nextPendingConnection();
    if(nullptr == m_dataProcessSocket)
    {
        saPrint() << "can not exec m_localServer->nextPendingConnection();";
        return;
    }
    m_dataReader->setSocket(m_dataProcessSocket);
    m_dataWriter->setSocket(m_dataProcessSocket);
}
///
/// \brief 数据处理的线程终结
/// \param exitCode 退出代码
/// \param exitStatus
///
void DataFeatureWidget::onProcessDataProcFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(QProcess::CrashExit == exitStatus)
    {
        static int s_dataProcCrashCount = 0;
        ++s_dataProcCrashCount;
        saError("signADataProc has been crash,crash count:%d,exit code:%d",s_dataProcCrashCount,exitCode);
        emit showMessageInfo(tr("signADataProc has been crash")
                             ,SA::WarningMessage);
        qDebug() << "s_dataProcCrashCount:"<<s_dataProcCrashCount;
        startDataProc();
    }
    else if(QProcess::NormalExit == exitStatus)
    {
        QString strInfo = tr("signADataProc has been exit");
        saWarning(strInfo);
        emit showMessageInfo(strInfo,SA::WarningMessage);
        startDataProc();
        emit showMessageInfo(tr("restart signADataProc"),SA::WarningMessage);
    }
}



