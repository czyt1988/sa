#include "SAFigureLayoutWidget.h"
#include "ui_SAFigureLayoutWidget.h"
#include "SAPlotLayerModel.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include <QColorDialog>
#include "SAChart.h"
SAFigureLayoutWidget::SAFigureLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAFigureLayoutWidget)
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
    m_figure = fig;
    updateLayout();
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


