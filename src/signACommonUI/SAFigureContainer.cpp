#include "SAFigureContainer.h"
#include <QResizeEvent>
SAFigureContainer::SAFigureContainer(QWidget *parent):QWidget(parent)
{

}

void SAFigureContainer::addWidget(QWidget *widget, const QRectF &pos)
{
    if(widget->parentWidget() != this)
    {
        widget->setParent(this);
    }
    QRect widgetSize;
    calcWidgetSize(pos,widgetSize);
    widget->setGeometry(widgetSize);
    m_widgetPos[widget] = pos;
}

void SAFigureContainer::addWidget(QWidget *widget, float xPresent, float yPresent, float wPresent, float hPresent)
{
    addWidget(widget,QRectF(xPresent,yPresent,wPresent,hPresent));
}

QList<QWidget *> SAFigureContainer::getWidgetList() const
{
    QList<QWidget *> res;
    for(auto i=m_widgetPos.begin();i!=m_widgetPos.end();++i)
    {
        res.append(i.key());
    }
    return res;
}
#include <QDebug>
void SAFigureContainer::resizeEvent(QResizeEvent *event)
{
    qDebug() << "SAFigureContainer::resizeEvent(QResizeEvent *event)";
    QRect subWidgetSize;
    for(auto i=m_widgetPos.begin();i!=m_widgetPos.end();++i)
    {
        calcWidgetSize(i.value(),subWidgetSize);
        QWidget* w = i.key();
        qDebug() << "subwidget begin resize";
        qDebug() << "widget size "<<size() <<" plot size " <<subWidgetSize;
        w->setGeometry(subWidgetSize);

        qDebug() << "subwidget end resize";
    }
  //  QWidget:resizeEvent(event);
}

void SAFigureContainer::calcWidgetSize(const QRectF &present, QRect &newSize)
{
    QSize widgetSize = size();
    newSize.setRect(
    widgetSize.width() * present.left()
    ,widgetSize.height() * present.top()
    ,widgetSize.width() * present.width()
    ,widgetSize.height() * present.height());
}
