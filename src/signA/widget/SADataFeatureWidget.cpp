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
    #include "SALocalServerDefine.h"
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
#define _DEBUG_PRINT
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
    #include "SALog.h"
    #include <QElapsedTimer>
        #ifdef USE_IPC_CALC_FEATURE
            static bool s_send_speed_test = false;
            static QElapsedTimer s_vector_send_time_elaspade = QElapsedTimer();
            #define __SEND_1M_POINTS_TEST__
        #endif
#endif
/// \def 连接不上的计数
#define CONNECT_RETRY_COUNT (50)
/// \def 心跳丢失次数记为连接丢失
#define HEART_BREAK_COUNT_AS_DISCONNECT (10)
//====================================================================





SADataFeatureWidget::SADataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SADataFeatureWidget)
  ,m_lastActiveSubWindow(nullptr)
#ifdef USE_THREAD_CALC_FEATURE

#else
  ,m_dataProcessSocket(nullptr)
  ,m_socketOpt(nullptr)
  ,m_connectRetryCount(CONNECT_RETRY_COUNT)
  ,m_wndPtrKey(1)
  ,m_lossHeartbeatCount(0)
#endif
{
    ui->setupUi(this);
#ifdef USE_THREAD_CALC_FEATURE

#else
    connect(&m_heartbeatChecker,&QTimer::timeout,this,&SADataFeatureWidget::onHeartbeatCheckerTimerout);
    setErrorCodeToString();
    connectToServer();
#endif
}

SADataFeatureWidget::~SADataFeatureWidget()
{
    delete ui;
}

#ifdef USE_IPC_CALC_FEATURE//使用多进程

/**
 * @brief 连接服务器，此函数在进程返回准备完成后调用
 */
void SADataFeatureWidget::connectToServer()
{
    if(m_dataProcessSocket)
    {
        delete m_dataProcessSocket;
        m_dataProcessSocket=nullptr;
    }
    m_dataProcessSocket = new QLocalSocket(this);
    connect(m_dataProcessSocket,&QLocalSocket::disconnected
            ,this,&SADataFeatureWidget::onLocalSocketDisconnect);
    m_connectRetryCount = CONNECT_RETRY_COUNT;
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
    qDebug() << tr("start connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME)");
#endif
    m_dataProcessSocket->connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME);

#ifdef _DEBUG_OUTPUT
    qDebug() << tr("start connectToServer cost:")<<t.elapsed();
    t.restart();
#endif
    if(m_connectRetryCount <= 0)
    {
        QString str = tr("can not connect dataProc Serve!%1").arg(m_dataProcessSocket->errorString());
        emit showMessageInfo(str,SA::ErrorMessage);
        saDebug(str);
        return;
    }
    if(!m_dataProcessSocket->waitForConnected(1000))
    {
        QTimer::singleShot(100,this,&SADataFeatureWidget::tryToConnectServer);
        --m_connectRetryCount;
        return;
    }

    qInfo() << tr("connect to dataProc serve success!");
    //建立socket解析操作类
    if(m_socketOpt)
    {
        delete m_socketOpt;
    }
    m_socketOpt = new SALocalServeSocketOpt(m_dataProcessSocket,this);
    connect(m_socketOpt,&SALocalServeSocketOpt::recHeartbeat
            ,this,&SADataFeatureWidget::onRecHeartbeat);
    connect(m_socketOpt,&SALocalServeSocketOpt::recString
            ,this,&SADataFeatureWidget::onReceivedString);
    connect(m_socketOpt,&SALocalServeSocketOpt::rec2DPointFs
            ,this,&SADataFeatureWidget::onReceive2DPointFs);
    connect(m_socketOpt,&SALocalServeSocketOpt::errorOccure
            ,this,&SADataFeatureWidget::onErrorOccure);
    m_socketOpt->sendString("__test__1m");
}
#endif


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
        const size_t size = cur->dataSize();
        QVector<QPointF> datas;
        datas.reserve(size);
        for(size_t c = 0;c<size;++c)
        {
            datas.append(cur->sample(c));
        }
        if(m_socketOpt)
        {
            qDebug() << "send item:" << plotitem->title().text() << " to calc";
            TmpStru ts(const_cast<QwtPlotItem *>(plotitem),const_cast<QMdiSubWindow *>(arg1),const_cast<SAChart2D *>(arg2));
            ++m_wndPtrKey;
            m_key2wndPtr[m_wndPtrKey] = ts;
            m_socketOpt->send2DPointFs(datas,m_wndPtrKey);
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
void SADataFeatureWidget::onRecHeartbeat(uint key)
{
    Q_UNUSED(key);
    m_lastHeartbeatTime = QDateTime::currentDateTime();
}
#endif



#ifdef USE_IPC_CALC_FEATURE//使用多进程
void SADataFeatureWidget::onReceivedString(const QString& str,uint key)
{
    SAXMLReadHelper xmlHelper(str);
    if(xmlHelper.isValid())
    {
        //说明这个字符串是一个点数组处理的结果
        if(SA_XML::TypeVectorPointFProcessResult == xmlHelper.getProtocolType())
        {
#ifdef _DEBUG_OUTPUT
            saPrint()<<"SAXMLReadHelper::TypeVectorPointFProcessResult";
#endif
            std::unique_ptr<SADataFeatureItem> item(new SADataFeatureItem);
            TmpStru kw = m_key2wndPtr.value(key,TmpStru(nullptr,nullptr,nullptr));
            if(nullptr == kw.mdiSubWnd)
            {
                qDebug() << tr("receive calc result ,but can not find widget");
                return;
            }
            //确保子窗口未关闭
            QList<QMdiSubWindow*> subWindList = saUI->getSubWindowList();
            if(!subWindList.contains(kw.mdiSubWnd))
            {
                qDebug() << tr("receive calc result ,but can not find subWind");
                return;
            }
            //确保绘图窗口未关闭
            SAFigureWindow* figure = getChartWidgetFromSubWindow(kw.mdiSubWnd);
            QList<SAChart2D*> plots = figure->get2DPlots();
            if(!plots.contains(kw.chart2d))
            {
                qDebug() << tr("receive calc result , but can not find figure in cur sub window");
                return;
            }
            //确保plotitem存在
            if(!kw.chart2d->itemList().contains(kw.plotitem))
            {
                qDebug() << tr("receive calc result , but can not find plot item in chart");
                return;
            }

            //查找model如果没有查找到新建一个model

            DataFeatureTreeModel* model= qobject_cast<DataFeatureTreeModel*>(m_subWindowToDataInfo.value(kw.mdiSubWnd,nullptr));
            if(nullptr == model)
            {
                model = new DataFeatureTreeModel(this);
                m_subWindowToDataInfo[kw.mdiSubWnd] = model;
                ui->treeView->setModel(model);
            }
            //设置item的名字
            item->setName(kw.plotitem->title().text());

            //设置item的颜色
            QColor clr = SAChart2D::getItemColor( kw.plotitem);
            if(clr.isValid())
            {
                clr.setAlpha(100);
                item->setBackgroundColor(clr);
            }
            model->setPlotItem(kw.chart2d,kw.plotitem,item.release());
            ui->treeView->expandToDepth(1);
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
void SADataFeatureWidget::onReceive2DPointFs(const QVector<QPointF>& arrs,uint key)
{
#ifdef _DEBUG_OUTPUT
    int costTime = s_vector_send_time_elaspade.restart();
    if(s_send_speed_test)
    {
        s_send_speed_test = false;
        saDebug(QString("test time cost:%1 \n ys.size:%2").arg(costTime).arg(arrs.size()));
        int byteSize = arrs.size() * 2*sizeof(qreal);
        byteSize += 16;
        saDebug(QString("send speed:%1 byte/ms(%2 MB/s)")
                .arg((byteSize)/costTime)
                .arg((byteSize/(1024.0*1024)) / (costTime/1000.0)));
    }
#endif
    Q_UNUSED(arrs);
    Q_UNUSED(key);
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
    m_connectRetryCount = CONNECT_RETRY_COUNT;
    saPrint() << "Start Try To Connect Server";
    tryToConnectServer();
}



/**
 * @brief 定时心跳检测时间到达触发槽
 */
void SADataFeatureWidget::onHeartbeatCheckerTimerout()
{
    if (0 == m_lossHeartbeatCount)
    {
        emit showMessageInfo(tr("connect lost !"),SA::NormalMessage);
    }
    ++m_lossHeartbeatCount;
    if (m_lossHeartbeatCount > HEART_BREAK_COUNT_AS_DISCONNECT)
    {
        //心跳多次无法到达，重新连接
        reconnect();
        m_lossHeartbeatCount = 0;
    }
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
        saPrint() << "can not find FigureWindow";
        return;
    }
    on_toolButton_clearDataFeature_clicked();//先清除标记
    QSet<SAChart2D*> chartPlots = figure->get2DPlots().toSet();

    QItemSelectionModel* selModel = ui->treeView->selectionModel();
    DataFeatureTreeModel* curFeatureModel = static_cast<DataFeatureTreeModel*>(ui->treeView->model());
    if(!selModel || !curFeatureModel)
    {
        saPrint() << "can not find DataFeatureTreeModel";
        return;
    }
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
        if(nullptr == c)
        {
            saPrint() << "can not getChartPtrFromItem";
            continue;
        }
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

/**
 * @brief SADataFeatureWidget::onShowErrorMessage
 * @param errcode
 */
void SADataFeatureWidget::onErrorOccure(int errcode)
{
    QString str = QString("Error Occure,code:%1,msg:%2").arg(errcode).arg(m_errcodeToString.value(errcode));
    emit showMessageInfo(str,SA::ErrorMessage);
}




#ifdef USE_IPC_CALC_FEATURE//使用多进程
/**
 * @brief 重新连接
 */
void SADataFeatureWidget::reconnect()
{
    //重新连接-自动释放资源
    connectToServer();
}

/**
 * @brief 把错误码转为字符串
 */
void SADataFeatureWidget::setErrorCodeToString()
{
    m_errcodeToString[SALocalServe::Unknow] = tr("unknow");
    m_errcodeToString[SALocalServe::ReceiveDataError] = tr("Receive Data Error");
    m_errcodeToString[SALocalServe::ReceiveUnknowHeader] = tr("Receive Unknow Header");
}
#endif










