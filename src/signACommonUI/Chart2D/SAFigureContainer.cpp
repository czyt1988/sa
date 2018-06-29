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
    widget->installEventFilter(this);
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

bool SAFigureContainer::isWidgetInContainer(const QWidget *w)
{
    return m_widgetPos.contains(const_cast<QWidget*>(w));
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

bool SAFigureContainer::eventFilter(QObject *watched, QEvent *event)
{
    if(nullptr == event)
    {
        return QWidget::eventFilter(watched,event);
    }
    if(QEvent::Resize == event->type())
    {
        if(watched && watched->isWidgetType())
        {
            QWidget* w = qobject_cast<QWidget*>(watched);
            if(w && isWidgetInContainer(w))
            {
                QResizeEvent* e = static_cast<QResizeEvent*>(event);
                QRectF& data = m_widgetPos[w];
                data.setWidth(e->size().width()/(double)width());
                data.setHeight(e->size().height()/(double)height());
            }
        }
    }
    else if(QEvent::Move == event->type())
    {
        if(watched && watched->isWidgetType())
        {
            QWidget* w = qobject_cast<QWidget*>(watched);
            if(w && isWidgetInContainer(w))
            {
                QMoveEvent* e = static_cast<QMoveEvent*>(event);
                QRectF& data = m_widgetPos[w];
                data.setX(e->pos().x()/(double)width());
                data.setY(e->pos().y()/(double)height());
            }
        }
    }
    return QWidget::eventFilter(watched,event);
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
