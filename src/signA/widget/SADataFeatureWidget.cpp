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
#include "SADataFeatureTreeModel.h"
#include "SADataFeatureItem.h"
#define _DEBUG_PRINT
#define _DEBUG_OUTPUT




SADataFeatureWidget::_DataInfo::_DataInfo(QwtPlotItem* plotitem, QMdiSubWindow *midwidget, SAChart2D *chartptr):
    item(plotitem)
    ,midwidget(midwidget)
    ,chart(chartptr)
{

}

bool SADataFeatureWidget::_DataInfo::operator <(const SADataFeatureWidget::_DataInfo &other)
{
    return ((int)(this->item) + (int)(this->chart) + (int)(this->midwidget)) < ((int)other.item + (int)other.chart + (int)other.midwidget);
}




SADataFeatureWidget::SADataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SADataFeatureWidget)
  ,m_lastActiveSubWindow(nullptr)
{
    ui->setupUi(this);
    connect(ui->treeView,&QTreeView::clicked,this,&SADataFeatureWidget::onTreeViewClicked);
    connect(ui->toolButton_clearDataFeature,&QToolButton::clicked,this,&SADataFeatureWidget::onToolButtonClearDataFeatureClicked);
    //消息转发
    connect(&m_client,&SADataClient::messageInfo,this,&SADataFeatureWidget::showMessageInfo);
    connect(&m_client,&SADataClient::heartbeatCheckerTimerout,this,&SADataFeatureWidget::onHeartbeatCheckerTimerout);
    connect(&m_client,&SADataClient::rec2DPointsDescribe,this,&SADataFeatureWidget::onRec2DPointsDescribe);
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
    //记录最后一次激活的窗口
    m_lastActiveSubWindow = arg1;
    setWindowTitle(tr("data feature[%1]").arg(arg1->windowTitle()));
    //查找此窗口是否有对应的模型
    auto im = m_mdiToModel.find(arg1);
    if(im != m_mdiToModel.end())
    {
        //说明已经有model。把model设置进tree里
        ui->treeView->setModel(im.value());
    }
    else
    {
        //说明没有对应的内容，把数据下发给数据处理进程进行处理
        SAFigureWindow* figure = getFigureFromSubWindow(arg1);//记录当前的绘图窗口
        if(figure)
        {
            //说明是绘图窗口，进入这个函数，说明是首次建立的窗口，此时就建立空模型
            //先建立空模型
            SADataFeatureTreeModel* m = new SADataFeatureTreeModel();
            //记录子窗口和模型对应关系
            m_mdiToModel[arg1] = m;
            //请求远程计算，计算完结果返回后会自动填充进模型中从而在界面上显示
            calcFigureFeature(arg1,figure,m);
        }
    }

}


///
/// \brief 计算绘图窗口的dataFeature
/// \param figure 绘图窗口指针，不允许null
///
void SADataFeatureWidget::calcFigureFeature(QMdiSubWindow *subwnd,SAFigureWindow *figure,SADataFeatureTreeModel* model)
{
    Q_CHECK_PTR(subwnd);
    Q_CHECK_PTR(figure);
    Q_CHECK_PTR(model);
    //先清空model
    model->clear();
    //用于进行请求的key标识，这里将记录key和_DataInfo的关系，以便reply能找到对应是哪个item的内容
    static int s_key = 0;
    ++s_key;
    //获取所有的chart逐一调用接口进行计算
    QList<SAChart2D*> charts = figure->get2DPlots();
    for(SAChart2D* c : charts)
    {
        QwtPlotItemList itemList = c->itemList();
        for(auto i : itemList)
        {
            calcPlotItemFeature(subwnd,c,model,i,s_key);
        }
    }
}

/**
 * @brief 计算每个曲线的信息
 * @param subwnd 子窗口
 * @param chart 对应的sa2dchart
 * @param model 对应的model
 * @param plotitem 对应的plotitem
 * @param key 赋予的请求序号
 */
void SADataFeatureWidget::calcPlotItemFeature(QMdiSubWindow *subwnd,SAChart2D *chart,SADataFeatureTreeModel* model,QwtPlotItem *plotitem,int key)
{
    Q_CHECK_PTR(chart);
    _DataInfo d = _DataInfo(plotitem,subwnd,chart);
    switch(plotitem->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        QVector<QPointF> xys;
        SAChart::getXYDatas(xys,static_cast<const QwtPlotCurve*>(plotitem));
        m_keyToDatainfo[key] = d;
        //建立初步的DataFeatureTreeModel先把类型设置进去
        model.
        if(!m_client.request2DPointsDescribe(xys,key))
        {
            //如果发送请求失败，把记录清除
            m_keyToDatainfo.remove(key);
        }
        break;
    }
    default:
        break;
    }
}

///
/// \brief 子窗口关闭槽
/// \param arg1
///
void SADataFeatureWidget::mdiSubWindowClosed(QMdiSubWindow *arg1)
{

    auto modelIte = m_mdiToModel.find(arg1);
    if(modelIte != m_mdiToModel.end())
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
    m_mdiToModel.remove(arg1);
}
///
/// \brief 从子窗口获取绘图窗口
/// \param sub
/// \return
///
SAFigureWindow *SADataFeatureWidget::getFigureFromSubWindow(QMdiSubWindow *sub)
{
    if(nullptr == sub)
    {
        return nullptr;
    }
    return qobject_cast<SAFigureWindow*>(sub->widget());
}

///
/// \brief 数据特性树点击
/// \param index
///
void SADataFeatureWidget::onTreeViewClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    if(nullptr == m_lastActiveSubWindow)
        return;
    SAFigureWindow* figure = getFigureFromSubWindow(m_lastActiveSubWindow);//记录当前的绘图窗口
    if(nullptr == figure)
    {
        saPrint() << "can not find FigureWindow";
        return;
    }
    onToolButtonClearDataFeatureClicked();//先清除标记
    QSet<SAChart2D*> chartPlots = figure->get2DPlots().toSet();

    QItemSelectionModel* selModel = ui->treeView->selectionModel();
    SADataFeatureTreeModel* curFeatureModel = static_cast<SADataFeatureTreeModel*>(ui->treeView->model());
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
        SAChart2D* c = SADataFeatureTreeModel::getChartPtrFromItem(topParent);
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
void SADataFeatureWidget::onToolButtonClearDataFeatureClicked()
{
    if(nullptr == m_lastActiveSubWindow)
        return;
    SAFigureWindow* figure = getFigureFromSubWindow(m_lastActiveSubWindow);//记录当前的绘图窗口
    if(nullptr == figure)
        return;
    QList<SAChart2D*> charts= figure->get2DPlots();
    std::for_each(charts.begin(),charts.end(),[](SAChart2D* p){
        p->removeAllPlotMarker();
    });
}

void SADataFeatureWidget::onChartHide()
{

}

void SADataFeatureWidget::onChartDestroy()
{

}

void SADataFeatureWidget::onFigureDestroy()
{

}

void SADataFeatureWidget::onHeartbeatCheckerTimerout()
{
    emit showMessageInfo(tr("data process serve heartbreat time out"),SA::WarningMessage);
}

/**
 * @brief 获取服务器反馈
 * @param header
 * @param res
 */
void SADataFeatureWidget::onRec2DPointsDescribe(const SAProtocolHeader &header, SAXMLProtocolParserPtr res)
{
    Q_UNUSED(header);

}

/**
 * @brief 对MdiSubWindow进行绑定
 * @param w
 */
void SADataFeatureWidget::bindMdiSubWindow(QMdiSubWindow *w)
{
    if(nullptr == w)
    {
        return;
    }
    SAFigureWindow* fig = getFigureFromSubWindow(w);
    if(nullptr == fig)
    {
        return;
    }
    //进行信号绑定
    connect(fig,&SAFigureWindow::destroyed,this,&SADataFeatureWidget::onFigureDestroy);

}
/**
 * @brief 对已经绑定的MdiSubWindow进行解绑
 * @param w
 */
void SADataFeatureWidget::unbindMdiSubWindow(QMdiSubWindow *w)
{

}















