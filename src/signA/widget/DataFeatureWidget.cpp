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
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include "SALocalServerDefine.h"
#include "qwt_plot_curve.h"

#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#include <QDebug>
#endif

DataFeatureWidget::DataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFeatureWidget)
  ,m_lastActiveSubWindow(nullptr)
  ,m_dataProcessSocket(nullptr)
  ,m_dataProcPro(nullptr)
  ,m_isReadedHeader(false)
  ,m_isStartRecData(false)
  ,m_dataLen(0)
{
    ui->setupUi(this);
    initLocalServer();
}

DataFeatureWidget::~DataFeatureWidget()
{
    m_dataProcPro->close();
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
#ifdef _DEBUG_OUTPUT
            QElapsedTimer t;
            t.start();
#endif
            callCalcFigureWindowFeature(figure);
#ifdef _DEBUG_OUTPUT
            qDebug() << "callCalcFigureWindowFeature time cost:" << t.elapsed();
#endif
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
    if(nullptr == m_dataProcessSocket)
    {
        return;
    }
    SALocalServeBaseHeader header;
    header.setKey(1);
    header.setSendedPid(qApp->applicationPid());
    header.setType(SALocalServeBaseHeader::VectorDoubleDataProc);
    QList<SAChart2D*> charts = figure->get2DPlots();
    for(auto i=charts.begin();i!=charts.end();++i)
    {
        QwtPlotItemList itemList = (*i)->itemList();
        for(auto j=itemList.begin();j!=itemList.end();++j)
        {
            if(QwtPlotItem::Rtti_PlotCurve == (*j)->rtti())
            {
                SALocalServeFigureItemProcessHeader subHeader;
                subHeader.setWndPtr((quintptr)m_lastActiveSubWindow);
                subHeader.setItem((quintptr)(*j));
                subHeader.setDataType(SALocalServeFigureItemProcessHeader::CurveData);

                QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(*j);
                const size_t size = cur->dataSize();
                QVector<QPointF> ys;
                for(size_t c = 0;c<size;++c)
                {
                    ys.append(cur->sample(c));
                }


                if(!m_dataProcessSocket->isValid())
                {
                    QDataStream io(m_dataProcessSocket);
                    header.write(io);
                    subHeader.write(io);
                    io << ys;
                    m_dataProcessSocket->waitForBytesWritten();
                }
            }
        }

    }
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
    qDebug() << "onLocalServeNewConnection";
    if(m_dataProcessSocket)
    {
        delete m_dataProcessSocket;
        m_dataProcessSocket = nullptr;
    }
    m_dataProcessSocket = m_localServer->nextPendingConnection();
    if(nullptr == m_dataProcessSocket)
    {
        return;
    }
    connect(m_dataProcessSocket,&QLocalSocket::readyRead,this,&DataFeatureWidget::onProcessDataReadyRead);
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
        saError("signADataProc has been crash,crash count:%d",s_dataProcCrashCount);
        emit showMessageInfo(tr("signADataProc has been crash")
                             ,SA::WarningMessage);
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
///
/// \brief 接收到数据处理进程的内容
///
void DataFeatureWidget::onProcessDataReadyRead()
{
    if(!m_dataProcessSocket->isValid())
        return;
    if(!m_isReadedHeader)
        m_recData += m_dataProcessSocket->readAll();
    dealRecDatas();

    #if 0
    SALocalServeBaseHeader header;
    header.read(io);
    emit showMessageInfo(tr("connect success:fullname%1,name:%2,key:%3,send pid:%4,type:%5")
                         .arg(m_dataProcessSocket->fullServerName()).arg(m_dataProcessSocket->serverName())
                         .arg(header.getKey()).arg(header.getSendedPid()).arg(header.getType())
                         ,SA::NormalMessage);
    if(header.getType() == SALocalServeBaseHeader::ShakeHand)
    {
        //握手协议

    }
    #endif
}

///
/// \brief 处理读取的数据
///
void DataFeatureWidget::dealRecDatas()
{
    if(!m_isReadedHeader)
    {
        //说明还没接收主包头，开始进行主包头数据解析
        dealMainHeaderData();
        return;
    }

}

void DataFeatureWidget::dealMainHeaderData()
{
    size_t mainHeaderSize = sizeof(SALocalServeBaseHeader);
    if(m_recData.size() < mainHeaderSize)
    {
        //说明包头未接收完，继续等下一个
        return;
    }
    else if(m_recData.size() == mainHeaderSize)
    {
        //说明包头接收完
        m_recData.clear();
        SALocalServeBaseHeader mainHeader;
        QDataStream io(m_recData);
        io >> mainHeader;

        m_isReadedHeader = true;
        m_isStartRecData = true;
        m_dataLen = mainHeader.getDataSize();//接收数据的大小
    }

}
