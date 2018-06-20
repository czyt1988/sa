#include "SAFigureContainer.h"
#include <QResizeEvent>
#include <QDebug>
#include <QChildEvent>
SAFigureContainer::SAFigureContainer(QWidget *parent):QWidget(parent)
{

}

SAFigureContainer::~SAFigureContainer()
{
    //qDebug() <<"SAFigureContainer destroy";
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
    //connect(widget,&QObject::destroyed,this,&SAFigureContainer::onWidgetDestroy);
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

QRectF SAFigureContainer::getWidgetPos(QWidget *w) const
{
    return m_widgetPos.value(w);
}


void SAFigureContainer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QRect subWidgetSize;
    for(auto i=m_widgetPos.begin();i!=m_widgetPos.end();++i)
    {
        calcWidgetSize(i.value(),subWidgetSize);
        QWidget* w = i.key();
        w->setGeometry(subWidgetSize);
    }
    //  QWidget:resizeEvent(event);
}


bool SAFigureContainer::event(QEvent *e)
{
    if(e)
    {
        if(QEvent::ChildRemoved == e->type())
        {
            QChildEvent* ce = static_cast<QChildEvent*>(e);
            QObject* obj = ce->child();
            if(nullptr != obj && obj->isWidgetType())
            {
                QWidget* w = (QWidget*)obj;
                m_widgetPos.remove(w);
            }
        }
    }
    return QWidget::event(e);
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
