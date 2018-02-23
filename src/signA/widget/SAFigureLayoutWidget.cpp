#include "SAFigureLayoutWidget.h"
#include "ui_SAFigureLayoutWidget.h"
#include "SAPlotLayerModel.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include <QColorDialog>
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

SAPlotLayerModel *SAFigureLayoutWidget::getPlotLayerModel() const
{
    return m_layoutModel;
}

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

void SAFigureLayoutWidget::onTableViewLayerPressed(const QModelIndex &index)
{
    if(!index.isValid ())
        return;

    QColor rgb = index.data (Qt::BackgroundColorRole).value<QColor>();

    SAPlotLayerModel* model=getPlotLayerModel();
    QwtPlotItem* item = model->getPlotItemFromIndex (index);
    if (1==index.column())
    {
        model->setData (index,!item->isVisible ());
    }
    else if(index.column() == 2)
    {//颜色
        QColorDialog clrDlg;
        clrDlg.setCurrentColor(rgb);
        if(clrDlg.exec() == QDialog::Accepted)
        {
            QColor newClr = clrDlg.selectedColor();
            model->setData (index,newClr,Qt::BackgroundColorRole);
        }
    }
    //
    SAChart2D* plot = m_figure->current2DPlot();
    if(!plot)
    {
       return;
    }
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


