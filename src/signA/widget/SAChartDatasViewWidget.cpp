#include "SAChartDatasViewWidget.h"
#include "ui_SAChartDatasViewWidget.h"
#include "SAPlotItemTreeModel.h"
#include "SAPlotDataModel.h"
#include "SAChart2D.h"
#include "SAFigureWindow.h"
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
    m_tableModel->setQwtPlotItems (items);
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
