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
#include "SAFigureWindow.h"
#include "SAChart2D.h"
DataFeatureWidget::DataFeatureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFeatureWidget)
  ,m_plotWidget(nullptr)
{
    ui->setupUi(this);
    DataFeatureTreeModel* treeModel = new DataFeatureTreeModel(this);
    ui->treeView->setModel(treeModel);
}

DataFeatureWidget::~DataFeatureWidget()
{
    delete ui;
}
///
/// \brief 子窗口激活槽
/// \param arg1
///
void DataFeatureWidget::mdiSubWindowActived(QMdiSubWindow *arg1)
{
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
}
///
/// \brief 子窗口关闭槽
/// \param arg1
///
void DataFeatureWidget::mdiSubWindowClosed(QMdiSubWindow *arg1)
{
    getTreeModel()->clear();
}
///
/// \brief DataFeatureWidget::getChartWidgetFromSubWindow
/// \param sub
/// \return
///
SAFigureWindow *DataFeatureWidget::getChartWidgetFromSubWindow(QMdiSubWindow *sub)
{
    return qobject_cast<SAFigureWindow*>(sub->widget());
}

DataFeatureTreeModel *DataFeatureWidget::getTreeModel()
{
    DataFeatureTreeModel * model = static_cast<DataFeatureTreeModel*>(ui->treeView->model());
    if (model == nullptr)
    {
        model = new DataFeatureTreeModel(this);
        ui->treeView->setModel(model);
    }
    return model;
}

void DataFeatureWidget::on_treeView_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    if(nullptr == m_plotWidget)
    {
        getTreeModel()->clear();
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
    if(nullptr == m_plotWidget)
    {
        getTreeModel()->clear();
        return;
    }
    QList<SAChart2D*> charts= m_plotWidget->get2DPlots();
    std::for_each(charts.begin(),charts.end(),[](SAChart2D* p){
        p->removeAllMarker();
    });

}
