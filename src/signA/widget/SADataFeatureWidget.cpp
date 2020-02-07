#include "SADataFeatureWidget.h"
#include "ui_SADataFeatureWidget.h"
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QTimer>
#include <SAPlotMarker.h>
#include <QItemSelectionModel>
#include <QItemSelection>
#include "SADataClient.h"
#include "SAChart.h"
#include "SAPlotMarker.h"
#include "SAMarkSymbol.h"
#include "SADataClient.h"
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



SADataFeatureWidget::SADataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SADataFeatureWidget)
  ,m_lastActiveSubWindow(nullptr)
  ,m_wndPtrKey(1)
{
    ui->setupUi(this);
    //消息转发
    connect(&m_client,&SADataClient::messageInfo,this,&SADataFeatureWidget::showMessageInfo);
    connect(&m_client,&SADataClient::heartbeatCheckerTimerout,this,&SADataFeatureWidget::onHeartbeatCheckerTimerout);
    m_client.tryConnectToServe();
}

SADataFeatureWidget::~SADataFeatureWidget()
{
    delete ui;
}

/**
 * @brief 子窗口激活槽
 * @param arg1
 */
void SADataFeatureWidget::mdiSubWindowActived(QMdiSubWindow *arg1)
{
    if(nullptr == arg1)
    {
        return;
    }
    m_lastActiveSubWindow = arg1;

    setWindowTitle(tr("data feature[%1]").arg(arg1->windowTitle()));
    QAbstractItemModel* model = m_subWindowToDataInfo.value(arg1,nullptr);
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
    switch(plotitem->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
        QVector<QPointF> serise;
        SAChart::getPlotCurveSample(const_cast<QwtPlotItem *>(plotitem),serise);
//        m_client.


//        if(m_socketOpt)
//        {
//            qDebug() << "send item:" << plotitem->title().text() << " to calc";
//            TmpStru ts(const_cast<QwtPlotItem *>(plotitem),const_cast<QMdiSubWindow *>(arg1),const_cast<SAChart2D *>(arg2));
//            ++m_wndPtrKey;
//            m_key2wndPtr[m_wndPtrKey] = ts;
//            m_socketOpt->send2DPointFs(serise,m_wndPtrKey);
//        }
    }

//    if( const QwtSeriesStore<QPointF>* cur = dynamic_cast<const QwtSeriesStore<QPointF>*>(plotitem)
//            /*QwtPlotItem::Rtti_PlotCurve == plotitem->rtti()*/
//            )
//    {
//        const size_t size = cur->dataSize();
//        QVector<QPointF> datas;
//        datas.reserve(size);
//        for(size_t c = 0;c<size;++c)
//        {
//            datas.append(cur->sample(c));
//        }
//        if(m_socketOpt)
//        {
//            qDebug() << "send item:" << plotitem->title().text() << " to calc";
//            TmpStru ts(const_cast<QwtPlotItem *>(plotitem),const_cast<QMdiSubWindow *>(arg1),const_cast<SAChart2D *>(arg2));
//            ++m_wndPtrKey;
//            m_key2wndPtr[m_wndPtrKey] = ts;
//            m_socketOpt->send2DPointFs(datas,m_wndPtrKey);
//        }
//    }
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




/**
 * @brief 定时心跳检测时间到达触发槽
 */
void SADataFeatureWidget::onHeartbeatCheckerTimerout()
{
    emit showMessageInfo(tr("connect lost !"),SA::NormalMessage);
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














