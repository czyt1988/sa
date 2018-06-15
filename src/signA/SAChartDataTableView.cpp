#include "SAChartDataTableView.h"
#include <QHeaderView>
SAChartDataTableView::SAChartDataTableView(QWidget *par):SATableView(par)
{
    setCellHeight(19);
}

void SAChartDataTableView::setCellHeight(int height)
{
    //表格默认高度
    QHeaderView* plotLayerVerticalHeader = verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(height);
    }
}
