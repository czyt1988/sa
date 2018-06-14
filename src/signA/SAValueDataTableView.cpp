#include "SAValueDataTableView.h"
#include <QKeyEvent>
#include <QHeaderView>
SAValueDataTableView::SAValueDataTableView(QWidget *par):SATableView(par)
{
    setCellHeight(19);
}

void SAValueDataTableView::setCellHeight(int height)
{
    //表格默认高度
    QHeaderView* plotLayerVerticalHeader = verticalHeader();
    if(plotLayerVerticalHeader)
    {
        plotLayerVerticalHeader->setDefaultSectionSize(height);
    }
}




