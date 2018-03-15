#include "SAChartDatasViewWidget.h"
#include "ui_SAChartDatasViewWidget.h"
#include "SAPlotItemTreeModel.h"
#include "SAPlotDataModel.h"
#include "SAChart2D.h"
#include "SAFigureWindow.h"
#include "SACellInputWidget.h"
#include "SAFigureOptCommands.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_multi_barchart.h"
#include "qwt_plot_spectrocurve.h"
#include "qwt_plot_tradingcurve.h"
SAChartDatasViewWidget::SAChartDatasViewWidget(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::SAChartDatasViewWidget)
    ,m_figure(nullptr)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,3);
    m_treeModel = new SAPlotItemTreeModel(this);
    ui->treeView->setModel(m_treeModel);
    m_tableModel = new SAPlotDataModel(this);
    ui->tableView->setModel(m_tableModel);
    ui->tableView->onCtrlVFun = [this](){
        this->onCtrlVInTableView();
    };

    QHeaderView* plotLayerVerticalHeader = ui->tableView->verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(19);
    }
    // - TreeView CurPlotItem slots(曲线条目树形窗口)
    connect(ui->treeView,&QTreeView::clicked,this,&SAChartDatasViewWidget::onTreeViewCurPlotItemClicked);
    // - 数据表双击 - 处理插入新数据
    connect(ui->tableView,&QTableView::doubleClicked,this,&SAChartDatasViewWidget::onTableViewDoubleClicked);
}

SAChartDatasViewWidget::~SAChartDatasViewWidget()
{
    delete ui;
}

QwtPlotItemTreeModel *SAChartDatasViewWidget::getTreeModel() const
{
    return m_treeModel;
}
///
/// \brief 设置一个fig，并显示figure的内容
/// \param fig
///
void SAChartDatasViewWidget::setFigure(SAFigureWindow *fig)
{
    if(m_figure)
    {
        if(m_figure == fig)
        {
            return;
        }
        disconnect(m_figure,&SAFigureWindow::chartAdded
                   ,this,&SAChartDatasViewWidget::onChartAdded);
        disconnect(m_figure,&SAFigureWindow::destroyed
                   ,this,&SAChartDatasViewWidget::onFigureDestroy);
    }
    m_figure = fig;
    m_tableModel->setFigure(fig);
    if(fig)
    {
        QList<SAChart2D*> plotWidgets = fig->get2DPlots();
        QList<QwtPlot*> qwtChart;
        std::for_each(plotWidgets.begin(),plotWidgets.end(),[&](SAChart2D* p){
            qwtChart.append(static_cast<QwtPlot*>(p));
        });
        m_treeModel->setPlots(qwtChart);
        ui->treeView->expandAll();
        connect(m_figure,&SAFigureWindow::chartAdded
                   ,this,&SAChartDatasViewWidget::onChartAdded);
        connect(m_figure,&SAFigureWindow::destroyed
                   ,this,&SAChartDatasViewWidget::onFigureDestroy);
    }
    else
    {
        m_treeModel->clear();
    }
    m_tableModel->clear();

}
///
/// \brief TreeView CurPlotItem slots(曲线条目树形窗口)
/// \param index
///
void SAChartDatasViewWidget::onTreeViewCurPlotItemClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QItemSelectionModel* sel = ui->treeView->selectionModel();
    QModelIndexList indexList = sel->selectedRows();
    QList<QwtPlotItem*> items;
    for(int i = 0;i<indexList.size ();++i)
    {
        if(!indexList[i].parent().isValid())
        {//说明点击的是父窗口，就是qwtplot,这时显示所有
            items.clear();
            int childIndex = 0;
            while (indexList[i].child(childIndex,0).isValid()) {
                items.append(m_treeModel->getQwtPlotItemFromIndex (
                                 indexList[i].child(childIndex,0)));
                ++childIndex;
            }
            break;
        }
        if(indexList[i].column () != 0)
        {
            indexList[i] = indexList[i].parent().child(indexList[i].row(),0);
        }
        items.append (m_treeModel->getQwtPlotItemFromIndex (indexList[i]));
    }

    m_tableModel->setPlotItems (items);
}
///
/// \brief SAChartDatasViewWidget::onTableViewDoubleClicked
/// \param index
///
void SAChartDatasViewWidget::onTableViewDoubleClicked(const QModelIndex &index)
{
    QwtPlotItem* item = m_tableModel->getItemFromCol(index.column());
    if(nullptr == item)
        return;
    int itemRowCount = m_tableModel->getItemRowCount(item);
    if(index.row() != itemRowCount)
        return;
    int startCol=-1,endCol=-1;
    m_tableModel->getItemColumnRange(item,&startCol,&endCol);
    if(-1 == startCol || -1 == endCol)
        return;
    SACellInputWidget w(this);
    initCellInputPosition(&w,item,index.row(),startCol,endCol);
    w.exec();
    if(!w.isAcceptInput())
        return;

    QList<double> datas;
    if(!w.getDoubleList(datas))
        return;

    //接受完数据，根据不同类型插入新数据
    SAChart2D* chart = m_figure->findChartFromItem(item);
    if(nullptr == chart)
        return;

    const int dim2 = endCol - startCol + 1;
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        if(2 != datas.size())
            return;
        QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);
        QPointF newData(datas[0],datas[1]);
        SAFigureAppendXYSeriesDataCommand* cmd
                = new SAFigureAppendXYSeriesDataCommand(
                    chart
                    ,p
                    ,tr("append curve %1 data").arg(p->title().text())
                    ,{newData}
                    );
        chart->appendCommand(cmd);
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        if(2 != datas.size())
            return;
        QwtPlotBarChart* p = static_cast<QwtPlotBarChart*>(item);
        QPointF newData(datas[0],datas[1]);
        SAFigureAppendXYSeriesDataCommand* cmd
                = new SAFigureAppendXYSeriesDataCommand(
                    chart
                    ,p
                    ,tr("append curve %1 data").arg(p->title().text())
                    ,{newData}
                    );
        chart->appendCommand(cmd);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        if(3 != datas.size())
            return;
        QwtPlotIntervalCurve* p = static_cast<QwtPlotIntervalCurve*>(item);
        QwtIntervalSample newData(datas[0],datas[1],datas[2]);
        SAFigureAppendIntervalSeriesDataCommand* cmd
                = new SAFigureAppendIntervalSeriesDataCommand(
                    chart
                    ,p
                    ,tr("append interval curve %1 data").arg(p->title().text())
                    ,{newData}
                    );
        chart->appendCommand(cmd);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        if(3 != datas.size())
            return;
        QwtPlotHistogram* p = static_cast<QwtPlotHistogram*>(item);
        QwtIntervalSample newData(datas[2],datas[0],datas[1]);
        SAFigureAppendIntervalSeriesDataCommand* cmd
                = new SAFigureAppendIntervalSeriesDataCommand(
                    chart
                    ,p
                    ,tr("append histogram %1 data").arg(p->title().text())
                    ,{newData}
                    );
        chart->appendCommand(cmd);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        if(3 != datas.size())
            return;
        QwtPlotSpectroCurve* p = static_cast<QwtPlotSpectroCurve*>(item);
        QwtPoint3D newData(datas[0],datas[1],datas[2]);
        SAFigureAppendXYZSeriesDataCommand* cmd
                = new SAFigureAppendXYZSeriesDataCommand(
                    chart
                    ,p
                    ,tr("append spectro %1 data").arg(p->title().text())
                    ,{newData}
                    );
        chart->appendCommand(cmd);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        if(5 != datas.size())
            return;
        QwtPlotTradingCurve* p = static_cast<QwtPlotTradingCurve*>(item);
        QwtOHLCSample newData(datas[0],datas[1],datas[2],datas[3],datas[4]);
        SAFigureAppendOHLCSeriesDataCommand* cmd
                = new SAFigureAppendOHLCSeriesDataCommand(
                    chart
                    ,p
                    ,tr("append trading %1 data").arg(p->title().text())
                    ,{newData}
                    );
        chart->appendCommand(cmd);
        break;
    }
    default:
        break;
    }
}

void SAChartDatasViewWidget::onChartAdded(QwtPlot *plot)
{
    m_treeModel->addPlot(plot);
}

void SAChartDatasViewWidget::onFigureDestroy(QObject *obj)
{
    setFigure(nullptr);
}

void SAChartDatasViewWidget::onCtrlVInTableView()
{

}

QPoint SAChartDatasViewWidget::tableHeaderPositionOffset() const
{
    QHeaderView* verticalHeader = ui->tableView->verticalHeader();
    QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
    return QPoint(verticalHeader->width(),horizontalHeader->height());
}

void SAChartDatasViewWidget::initCellInputPosition(SACellInputWidget *w,QwtPlotItem* item,int row,int colStart,int colEnd)
{
    QPoint offset = tableHeaderPositionOffset();
    const int dim2 = colEnd - colStart + 1;
    w->resizeCells(dim2);
    QPoint leftTop,rightBottom;
    leftTop.setX(ui->tableView->columnViewportPosition(colStart));
    leftTop.setY(ui->tableView->rowViewportPosition(row));
    rightBottom.setX(ui->tableView->columnViewportPosition(colEnd)+ui->tableView->columnWidth(colEnd));
    rightBottom.setY(leftTop.y()+ui->tableView->rowHeight(row));
    int tmp = ui->tableView->rowHeight(row);
    if(tmp < 20 || tmp > 30)
    {
        tmp = 20;
    }
    leftTop.ry() -= tmp;
    leftTop = ui->tableView->mapToGlobal(leftTop);
    rightBottom = ui->tableView->mapToGlobal(rightBottom);
    leftTop += offset;
    rightBottom += offset;
    int buttonAreaWidth = w->getButtonAreaWidth();
    rightBottom.rx() += buttonAreaWidth;
    w->setGeometry(QRect(leftTop,rightBottom));
    for(int i=0;i<dim2;++i)
    {
        w->setCellTitleText(i,m_tableModel->getItemDimDescribe(item,i));
        w->setCellWidth(i,ui->tableView->columnWidth(colStart+i));
    }
}
