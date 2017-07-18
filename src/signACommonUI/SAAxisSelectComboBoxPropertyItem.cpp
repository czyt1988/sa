#include "SAAxisSelectComboBoxPropertyItem.h"
#include "qwt_plot.h"
#include <QComboBox>
SAAxisSelectComboBoxPropertyItem::SAAxisSelectComboBoxPropertyItem(SelectMode mode, QWidget *par)
    :SAComboBoxPropertyItem(par)
{
    if(XAxisOnly == mode)
    {
        addItem(tr("X Bottom"),(int)QwtPlot::xBottom);
        addItem(tr("X Top"),(int)QwtPlot::xTop);
    }
    else if(YAxisOnly == mode)
    {
        addItem(tr("Y Left"),(int)QwtPlot::yLeft);
        addItem(tr("Y Right"),(int)QwtPlot::yRight);
    }
    else
    {
        addItem(tr("X Bottom"),(int)QwtPlot::xBottom);
        addItem(tr("X Top"),(int)QwtPlot::xTop);
        addItem(tr("Y Left"),(int)QwtPlot::yLeft);
        addItem(tr("Y Right"),(int)QwtPlot::yRight);
    }
    connect(getComboBox(),static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
            ,this,&SAAxisSelectComboBoxPropertyItem::onCurrentIndexChanged);
}

void SAAxisSelectComboBoxPropertyItem::setCurrentSelectAxis(int axisID)
{
    getComboBox()->setCurrentIndex(getComboBox()->findData(axisID));
}

void SAAxisSelectComboBoxPropertyItem::onCurrentIndexChanged(int index)
{
    if(index >=0 )
    {
        emit axisSelectChanged(getComboBox()->currentData().toInt());
    }
}

