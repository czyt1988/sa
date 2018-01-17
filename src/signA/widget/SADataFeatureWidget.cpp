#include "SADataFeatureWidget.h"
#include "ui_SADataFeatureWidget.h"
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QTimer>
#include <SAPlotMarker.h>
#include <QItemSelectionModel>
#include <QItemSelection>
#include "SAChart.h"
#include "SAPlotMarker.h"
#include "SAMarkSymbol.h"
#ifdef USE_THREAD_CALC_FEATURE

#else
    #include <QLocalServer>
    #include <QLocalSocket>
    #include "SALocalServeFigureItemProcessHeader.h"
    #include "SALocalServerDefine.h"
    #include "SALocalServeReader.h"
    #include "SALocalServeWriter.h"
    #include "SAXMLReadHelper.h"
#endif

#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SALog.h"

#include "qwt_plot_curve.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "SAMdiSubWindow.h"
#include "DataFeatureTreeModel.h"
#include "SADataFeatureItem.h"

//#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
    #include <QElapsedTimer>
    #include <QDebug>
        #ifdef USE_IPC_CALC_FEATURE
            static bool s_send_speed_test = false;
            static QElapsedTimer s_vector_send_time_elaspade = QElapsedTimer();
            #define __SEND_1M_POINTS_TEST__
        #endif
#endif


//====================================================================

SADataFeatureWidget::SADataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SADataFeatureWidget)
  ,m_lastActiveSubWindow(nullptr)
#ifdef USE_THREAD_CALC_FEATURE

#else
  ,m_dataProcessSocket(nullptr)
  ,m_dataReader(nullptr)
  ,m_dataWriter(nullptr)
  ,m_connectRetryCount(50)
#endif
{
    ui->setupUi(this);
#ifdef USE_THREAD_CALC_FEATURE

#else
    initLocalServer();
#endif
}

SADataFeatureWidget::~SADataFeatureWidget()
{
    delete ui;
}
///
/// \brief 子窗口激活槽
/// \param arg1
///
void SADataFeatureWidget::mdiSubWindowActived(QMdiSubWindow *arg1)
{
    if(nullptr == arg1)
    {
        return;
    }
    m_lastActiveSubWindow = arg1;

    setWindowTitle(tr("data feature[%1]").arg(arg1->windowTitle()));
    QAbstractItemModel* model = m_subWindowToDataInfo.value(arg1,nullptr);
#ifdef USE_THREAD_CALC_FEATURE

#else
    if(model)
    {
        checkModelItem(model,arg1);
        ui->treeView->setModel(model);
        ui->treeView->expandToDepth(1);
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
#endif
}
///
/// \brief 子窗口关闭槽
/// \param arg1
///
void SADataFeatureWidget::mdiSubWindowClosed(QMdiSubWindow *arg1)
{
    auto modelIte = m_subWindowToDataInfo.find(arg1);
    if(modelIte != m_subWindowToDataInfo.end())
    {
        //子窗口关闭，把对应的对应model删除，如果正在显示这个model，先设置为null
        QAbstractItemModel* model = modelIte.value();
        if(ui->treeView->model() == model)
        {
            ui->treeView->setModel(nullptr);
        }
        if(model)
        {
            delete model;
        }
    }
    m_subWindowToDataInfo.remove(arg1);
}
///
/// \brief 从子窗口获取绘图窗口
/// \param sub
/// \return
///
SAFigureWindow *SADataFeatureWidget::getChartWidgetFromSubWindow(QMdiSubWindow *sub)
{
    if(nullptr == sub)
    {
        return nullptr;
    }
    return qobject_cast<SAFigureWindow*>(sub->widget());
}
///
/// \brief 计算绘图窗口的dataFeature
/// \param figure 绘图窗口指针，不允许null
///
void SADataFeatureWidget::callCalcFigureWindowFeature(SAFigureWindow *figure)
{
#ifdef USE_THREAD_CALC_FEATURE//使用多线程

#else //使用多进程
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
            calcPlotItemFeature(*j,m_lastActiveSubWindow,*i);
        }
    }
#endif
}

#ifdef USE_IPC_CALC_FEATURE//使用多进程
///
/// \brief 计算一个plot item
/// \param plotitem
/// \param arg1
/// \param arg2
///
void SADataFeatureWidget::calcPlotItemFeature(const QwtPlotItem *plotitem, const QMdiSubWindow *arg1, const SAChart2D *arg2)
{
    if( const QwtSeriesStore<QPointF>* cur = dynamic_cast<const QwtSeriesStore<QPointF>*>(plotitem)
            /*QwtPlotItem::Rtti_PlotCurve == plotitem->rtti()*/
            )
    {
        //const QwtPlotCurve* cur = static_cast<const QwtPlotCurve*>(plotitem);
        const size_t size = cur->dataSize();
        QVector<QPointF> datas;
        datas.reserve(size);
        for(size_t c = 0;c<size;++c)
        {
            datas.append(cur->sample(c));
        }
        if(m_dataWriter)
        {
            qDebug() << "send item:" << plotitem->title().text() << " to calc";
            m_dataWriter->sendDoubleVectorData((qintptr)arg1,(qintptr)arg2,(qintptr)plotitem,datas);
        }
    }
}
#endif

void SADataFeatureWidget::checkModelItem(QAbstractItemModel *baseModel, QMdiSubWindow *subWndPtr)
{
    DataFeatureTreeModel* featureModel = qobject_cast<DataFeatureTreeModel*>(baseModel);
    if(nullptr == featureModel)
    {
        return;
    }
    SAFigureWindow * fig = SAUIInterface::getFigureWidgetFromMdiSubWindow(subWndPtr);
    if(nullptr == fig)
    {
        return;
    }
    QList<SAChart2D *> plots = fig->get2DPlots();
    QList<SADataFeatureItem* >  chartItems = featureModel->getRootItems();
    //检测有没有chart被删除了
    std::for_each(chartItems.begin(),chartItems.end(),[&plots,&featureModel](SADataFeatureItem* c){
        SAChart2D* p = DataFeatureTreeModel::getChartPtrFromItem(c);
        if(p)
        {
            if(!plots.contains(p))
            {
                if(featureModel->takeRootItem(c))
                {
                    if(c)
                    {
                        delete c;
                    }
                }
            }
        }

    });
    //检测item有没有新加或删除
    for(int i=0;i<plots.size();++i)
    {
        QwtPlotItemList itemLists = plots[i]->itemList();
        SADataFeatureItem* ci = featureModel->findChartItem(plots[i]);
        if(nullptr == ci)
        {
            continue;
        }
        QSet<QwtPlotItem*> itemSet = featureModel->getItemSetFromItem(ci);
        for(int j=0;j<itemLists.size();++j)
        {
            if(!itemSet.contains(itemLists[j]))
            {
#ifdef USE_IPC_CALC_FEATURE//使用多进程
                calcPlotItemFeature(itemLists[j],subWndPtr,plots[i]);
#endif
            }
        }
    }


}



#ifdef USE_IPC_CALC_FEATURE//使用多进程
void SADataFeatureWidget::onReceivedShakeHand(const SALocalServeBaseHeader &mainHeader)
{
    Q_UNUSED(mainHeader);
#ifdef __SEND_1M_POINTS_TEST__
    if(m_dataWriter)
    {
        qDebug() << "start 1000000 points test";
        s_send_speed_test = true;
        s_vector_send_time_elaspade.restart();
        m_dataWriter->sendString("__test__1m");//发送一个测试
    }
#endif
    emit showMessageInfo(tr("data process connect sucess!"),SA::NormalMessage);
}
#endif

#ifdef USE_IPC_CALC_FEATURE//使用多进程
void SADataFeatureWidget::onReceivedString(const QString &xmlString)
{
    SAXMLReadHelper xmlHelper(xmlString);
    if(xmlHelper.isValid())
    {
        //说明这个字符串是一个点数组处理的结果
        if(SAXMLReadHelper::TypeVectorPointFProcessResult == xmlHelper.getProtocolType())
        {
            quintptr w,chart,plotItemPtr;
            std::unique_ptr<SADataFeatureItem> item(new SADataFeatureItem);
            if(xmlHelper.getVectorPointFProcessResult(w,chart,plotItemPtr,item.get()))
            {
                QMdiSubWindow* subWind = (QMdiSubWindow*)w;
                SAChart2D* chartWnd = (SAChart2D*)chart;
                QwtPlotItem* itemPtr = (QwtPlotItem*)plotItemPtr;
                QList<QMdiSubWindow*> subWindList = saUI->getSubWindowList();
                if(!subWindList.contains(subWind))
                {
                    saDebug(tr("sa can not find subWind,subWind ptr:%1").arg((quintptr)subWind));
                    return;
                }
                SAFigureWindow* figure = getChartWidgetFromSubWindow(subWind);
                QList<SAChart2D*> plots = figure->get2DPlots();

                if(!plots.contains(chartWnd))
                {
                    saDebug(tr("can not find figure in cur sub window:%1").arg((quintptr)subWind));
                    return;
                }
                //查找model如果没有查找到新建一个model
                DataFeatureTreeModel* model= qobject_cast<DataFeatureTreeModel*>(m_subWindowToDataInfo.value(subWind,nullptr));
                if(nullptr == model)
                {
                    model = new DataFeatureTreeModel(this);
                    m_subWindowToDataInfo[subWind] = model;
                    ui->treeView->setModel(model);
                }
                //设置item的名字
                item->setName(itemPtr->title().text());
                //设置item的颜色
                QColor clr = SAChart2D::getItemColor( itemPtr);
                if(clr.isValid())
                {
                    clr.setAlpha(100);
                    item->setBackgroundColor(clr);
                }
                model->setPlotItem(chartWnd,itemPtr,item.release());
                ui->treeView->expandToDepth(1);
            }
        }
    }

}
#endif

#ifdef USE_IPC_CALC_FEATURE//使用多进程
///
/// \brief 接收到到点数组
/// \param header
/// \param ys
///
void SADataFeatureWidget::onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader &header, QVector<QPointF> &ys)
{
#ifdef _DEBUG_OUTPUT
    Q_UNUSED(header);
    int costTime = s_vector_send_time_elaspade.restart();
    if(s_send_speed_test)
    {
        s_send_speed_test = false;
        saDebug(QString("test time cost:%1 \n ys.size:%2").arg(costTime).arg(ys.size()));
        int byteSize = ys.size() * 2*sizeof(qreal);

        saDebug(QString("send speed:%1 byte/ms(%2 MB/s)")
                .arg((byteSize)/costTime)
                .arg((byteSize/(1024.0*1024)) / (costTime/1000.0)));
    }
#endif
    Q_UNUSED(header);
    Q_UNUSED(ys);

}

void SADataFeatureWidget::onLocalSocketDisconnect()
{
    saPrint() << "signADataProc local socket disconnect!";
    QTimer::singleShot(100,this,&SADataFeatureWidget::tryToStartDataProc);
}

void SADataFeatureWidget::tryToStartDataProc()
{
    saPrint() << "Try To Start Data Proc";
    QString path = qApp->applicationDirPath()+"/signADataProc.exe";
    QStringList args = {QString::number(qApp->applicationPid())};
    QProcess::startDetached(path,args);//signADataProc是一个单例进程，多个软件不会打开多个
    m_connectRetryCount = 50;
    saPrint() << "Start Try To Connect Server";
    tryToConnectServer();
}
#endif


///
/// \brief 数据特性树点击
/// \param index
///
void SADataFeatureWidget::on_treeView_clicked(const QModelIndex &index)
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
    QSet<SAChart2D*> chartPlots = figure->get2DPlots().toSet();

    QItemSelectionModel* selModel = ui->treeView->selectionModel();
    DataFeatureTreeModel* curFeatureModel = static_cast<DataFeatureTreeModel*>(ui->treeView->model());
    if(!selModel || !curFeatureModel)
        return;
    QModelIndexList indexList = selModel->selectedIndexes();

    for(int i=0;i<indexList.size();++i)
    {
        if(1 != indexList[i].column())
        {
            continue;
        }
        SADataFeatureItem* item = static_cast<SADataFeatureItem*>(indexList[i].internalPointer());
        if(nullptr == item)
        {
            continue;
        }
        SADataFeatureItem* topParent = item->topParent();
        //根据topParent找到对应的SAChart2D;
        SAChart2D* c = DataFeatureTreeModel::getChartPtrFromItem(topParent);
        if(!chartPlots.contains(c))
        {
            continue;
        }
        QVariant var = item->getValue();
        switch(var.type())
        {
        case QVariant::PointF:
        case QVariant::Point:
        {
            QPointF point = var.toPointF();
            SAPointMarker* pointMark = new SAPointMarker(point);
            SATriangleMarkSymbol* tra = new SATriangleMarkSymbol();
            pointMark->setSymbol( tra );
            pointMark->setLabel(tr("%1(%2,%3)").arg(item->getName()).arg(point.x()).arg(point.y()));
            pointMark->setLabelAlignment(Qt::AlignTop|Qt::AlignHCenter);
            pointMark->setSpacing(10);//设置文字和mark的间隔
            c->addPlotMarker(pointMark);
            qDebug() << "add Point";
            c->replot();
        }break;
        case QVariant::Double:
        {
            double data = var.toDouble();
            SAYValueMarker* valueMark = new SAYValueMarker(data);
            valueMark->setXValue(c->axisXmax());
            valueMark->setLinePen(Qt::black,1);
            valueMark->setLabel(tr("%1(%2)").arg(item->getName().arg(data)));
            valueMark->setLabelAlignment(Qt::AlignTop|Qt::AlignRight);
            valueMark->setSpacing(1);//设置文字和mark的间隔
            c->addPlotMarker(valueMark);
            c->replot();
        }break;
        default:
            return;
        }
    }
}
///
/// \brief 清除标记按钮
///
void SADataFeatureWidget::on_toolButton_clearDataFeature_clicked()
{
    if(nullptr == m_lastActiveSubWindow)
        return;
    SAFigureWindow* figure = getChartWidgetFromSubWindow(m_lastActiveSubWindow);//记录当前的绘图窗口
    if(nullptr == figure)
        return;
    QList<SAChart2D*> charts= figure->get2DPlots();
    std::for_each(charts.begin(),charts.end(),[](SAChart2D* p){
        p->removeAllPlotMarker();
    });
}

void SADataFeatureWidget::onShowErrorMessage(const QString &info)
{
    emit showMessageInfo(info,SA::ErrorMessage);
}
#ifdef USE_IPC_CALC_FEATURE//使用多进程
///
/// \brief 初始化本地服务器
///
void SADataFeatureWidget::initLocalServer()
{
    m_dataReader = new SALocalServeReader(this);
    m_dataWriter = new SALocalServeWriter(this);
    connect(m_dataReader,&SALocalServeReader::receivedShakeHand
            ,this,&SADataFeatureWidget::onReceivedShakeHand);
    connect(m_dataReader,&SALocalServeReader::receivedString
            ,this,&SADataFeatureWidget::onReceivedString);
    connect(m_dataReader,&SALocalServeReader::receivedVectorPointFData
            ,this,&SADataFeatureWidget::onReceivedVectorPointFData);
    connect(m_dataReader,&SALocalServeReader::errorOccure
            ,this,&SADataFeatureWidget::onShowErrorMessage);
    connectToServer();
}
#endif

#ifdef USE_IPC_CALC_FEATURE//使用多进程
///
/// \brief 连接服务器，此函数在进程返回准备完成后调用
///
void SADataFeatureWidget::connectToServer()
{
    if(m_dataProcessSocket)
    {
        delete m_dataProcessSocket;m_dataProcessSocket=nullptr;
    }
    m_dataProcessSocket = new QLocalSocket(this);
    connect(m_dataProcessSocket,&QLocalSocket::disconnected
            ,this,&SADataFeatureWidget::onLocalSocketDisconnect);
    m_connectRetryCount = 50;
    tryToConnectServer();
}
#endif

#ifdef USE_IPC_CALC_FEATURE//使用多进程
///
/// \brief 尝试连接服务器,此函数在connectToServer调用
///
void SADataFeatureWidget::tryToConnectServer()
{
#ifdef _DEBUG_OUTPUT
    QElapsedTimer t;
    t.start();
    saPrint() << "start connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME)";
#endif
    m_dataProcessSocket->connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME);

#ifdef _DEBUG_OUTPUT
    saPrint() << "start connectToServer cost:"<<t.elapsed();
    t.restart();
#endif
    if(m_connectRetryCount <= 0)
    {
        QString str = tr("can not connect dataProc Serve!%1").arg(m_dataProcessSocket->errorString());
        emit showMessageInfo(str,SA::ErrorMessage);
        saDebug(str);
        return;
    }
    if(!m_dataProcessSocket->waitForConnected(100))
    {

        QTimer::singleShot(100,this,&SADataFeatureWidget::tryToConnectServer);
        --m_connectRetryCount;
        return;
    }

    saPrint() << "connect to dataProc serve success!";
    m_dataReader->setSocket(m_dataProcessSocket);
    m_dataWriter->setSocket(m_dataProcessSocket);
    m_dataWriter->sendShakeHand();

}
#endif








