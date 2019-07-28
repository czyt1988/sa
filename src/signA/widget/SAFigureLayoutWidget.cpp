#include "SAFigureLayoutWidget.h"
#include "ui_SAFigureLayoutWidget.h"
#include "SAPlotLayerModel.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include <QColorDialog>
#include <QComboBox>
#include "SAChart.h"
SAFigureLayoutWidget::SAFigureLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAFigureLayoutWidget)
  ,m_figure(nullptr)
{
    ui->setupUi(this);
    m_layoutModel = new SAPlotLayerModel(ui->tableView);
    QHeaderView* plotLayerVerticalHeader = ui->tableView->verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(19);
    }
    ui->tableView->setModel(m_layoutModel);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    auto hh = ui->tableView->horizontalHeader();
    hh->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    hh->setStretchLastSection(true);
    connect(ui->tableView,&QTableView::pressed,this,&SAFigureLayoutWidget::onTableViewLayerPressed);
    connect(ui->toolButtonDelete,&QToolButton::clicked,this,&SAFigureLayoutWidget::onToolButtonDeleteClicked);
    connect(ui->comboBoxCurrentChart,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
            ,this,&SAFigureLayoutWidget::onComboBoxCurrentChartCurrentIndexChanged);
}

SAFigureLayoutWidget::~SAFigureLayoutWidget()
{
    delete ui;
}

SAPlotLayerModel *SAFigureLayoutWidget::getLayoutModel() const
{
    return m_layoutModel;
}

void SAFigureLayoutWidget::setFigure(SAFigureWindow *fig)
{
    if(m_figure)
    {
        disconnect(m_figure,&SAFigureWindow::chartAdded
                   ,this,&SAFigureLayoutWidget::onChartAdded);
        disconnect(m_figure,&SAFigureWindow::chartRemoved
                   ,this,&SAFigureLayoutWidget::onChartRemoved);
        disconnect(m_figure,&SAFigureWindow::currentWidgetChanged
                ,this,&SAFigureLayoutWidget::onCurrentWidgetChanged);
    }
    m_figure = fig;
    if(m_figure)
    {
        connect(m_figure,&SAFigureWindow::chartAdded
                   ,this,&SAFigureLayoutWidget::onChartAdded);
        connect(m_figure,&SAFigureWindow::chartRemoved
                   ,this,&SAFigureLayoutWidget::onChartRemoved);
        connect(m_figure,&SAFigureWindow::currentWidgetChanged
                ,this,&SAFigureLayoutWidget::onCurrentWidgetChanged);
    }
    updateCurrentChart();

}

SAFigureWindow *SAFigureLayoutWidget::currentFigure() const
{
    return m_figure;
}


///
/// \brief 更新图层
///
void SAFigureLayoutWidget::updateLayout()
{
    if(nullptr == m_figure)
    {
        m_layoutModel->setPlot(nullptr);
        return;
    }
    SAChart2D* plot = m_figure->current2DPlot();
    if(plot)
    {
        m_layoutModel->setPlot(plot);
        QItemSelectionModel* selectModel = ui->tableView->selectionModel();
        QList<QwtPlotItem*> selItems = plot->getCurrentSelectItems();
        if(selectModel)
        {
           QModelIndexList indexSel = m_layoutModel->getIndexFromPlotItems(selItems);
           selectModel->reset();
           for(int i=0;i<indexSel.size();++i)
           {
               selectModel->select(indexSel[i],QItemSelectionModel::Select|QItemSelectionModel::Rows);
           }
        }
    }
}

///
/// \brief 更新当前选中的图表
///
void SAFigureLayoutWidget::updateCurrentChart()
{
    if(nullptr == m_figure)
    {
        m_layoutModel->setPlot(nullptr);
        return;
    }
    const QList<SAChart2D *>& plots = m_figure->get2DPlots();
    ui->comboBoxCurrentChart->clear();
    int selIndex = -1;
    for(int i=0;i<plots.size();++i)
    {
        QString title = plots[i]->title().text();
        if(title.isEmpty())
        {
            title = tr("untitle[%1]").arg(i+1);
        }
        ui->comboBoxCurrentChart->addItem(title);
        if(m_figure->current2DPlot()==plots[i])
        {
            selIndex = i;
        }
    }
    ui->comboBoxCurrentChart->setCurrentIndex(selIndex);
}
///
/// \brief 表格点击
/// \param index
///
void SAFigureLayoutWidget::onTableViewLayerPressed(const QModelIndex &index)
{
    if(!index.isValid ())
        return;

    QColor rgb = index.data (Qt::BackgroundColorRole).value<QColor>();

    SAPlotLayerModel* model=getLayoutModel();
    QwtPlotItem* item = model->getPlotItemFromIndex (index);
    SAChart2D* plot = qobject_cast<SAChart2D*>(model->getPlot());
    if(!plot)
    {
       return;
    }
    if (1==index.column())
    {//可见性
        model->setData (index,!item->isVisible ());
        emit itemVisibleChanged(plot,item,item->isVisible());
    }
    else if(index.column() == 2)
    {//颜色
        QColorDialog clrDlg;
        clrDlg.setCurrentColor(rgb);
        if(clrDlg.exec() == QDialog::Accepted)
        {
            QColor newClr = clrDlg.selectedColor();
            model->setData (index,newClr,Qt::BackgroundColorRole);
            emit itemColorChanged(plot,item,newClr);
        }
    }
    //

    QItemSelectionModel* selMode = ui->tableView->selectionModel();
    QSet<QwtPlotItem*> itemSets;
    if(selMode)
    {
        QModelIndexList selIndex = selMode->selectedRows();
        for(int i=0;i<selIndex.size();++i)
        {
            itemSets.insert( model->getPlotItemFromIndex (selIndex[i]));
        }
        plot->setCurrentSelectItems(itemSets.toList());
    }
}

void SAFigureLayoutWidget::onToolButtonDeleteClicked(bool on)
{
    Q_UNUSED(on);
    QItemSelectionModel* selectModel = ui->tableView->selectionModel();
    SAChart2D* plot = qobject_cast<SAChart2D*>(m_layoutModel->getPlot());
    if(!selectModel || !plot)
    {
        return;
    }
    QModelIndexList indexs = selectModel->selectedIndexes();
    for(int i=0;i<indexs.size();++i)
    {
        QwtPlotItem* item = m_layoutModel->getPlotItemFromIndex(indexs[i]);
        if(SAChart::checkIsPlotChartItem(item))
        {
            plot->removeItem(item);
            emit itemRemoved(plot,item);
        }
    }
    m_layoutModel->updateModel();
}


///
/// \brief 当前绘图选择改变
/// \param index
///
void SAFigureLayoutWidget::onComboBoxCurrentChartCurrentIndexChanged(int index)
{
    if(nullptr == m_figure || index < 0)
    {
        return;
    }
    const QList<SAChart2D *>& plots = m_figure->get2DPlots();
    if(index >= plots.size())
    {
        return;
    }
    m_figure->setCurrent2DPlot(plots[index]);
    updateLayout();
}

///
/// \brief 添加图表
/// \param plot
///
void SAFigureLayoutWidget::onChartAdded(QwtPlot *plot)
{
    Q_UNUSED(plot);
    updateCurrentChart();
}

///
/// \brief 删除图表
/// \param plot
///
void SAFigureLayoutWidget::onChartRemoved(QwtPlot *plot)
{
    Q_UNUSED(plot);
    updateCurrentChart();
}

///
/// \brief figure 当前选中的图形发生了改变
/// \param w
///
void SAFigureLayoutWidget::onCurrentWidgetChanged(QWidget *w)
{
    if(nullptr == m_figure)
    {
        return;
    }
    const QList<SAChart2D *>& plots = m_figure->get2DPlots();
    for(int i=0;i<plots.size();++i)
    {
        if(plots[i] == w)
        {
            ui->comboBoxCurrentChart->setCurrentIndex(i);
        }
    }
}

///
/// \brief 图表标题改变
/// \param chart
/// \param title
///
void SAFigureLayoutWidget::setChartTitle(QwtPlot *chart, const QString &title)
{
    if(nullptr == m_figure)
    {
        return;
    }
    QList<SAChart2D *> charts = m_figure->get2DPlots();
    for(int i=0;i<charts.size();++i)
    {
        if(charts[i] == chart)
        {
            if(title.isEmpty())
            {
                ui->comboBoxCurrentChart->setItemText(i,tr("untitle[%1]").arg(i+1));
            }
            else
            {
                ui->comboBoxCurrentChart->setItemText(i,title);
            }
            return;
        }
    }
}
