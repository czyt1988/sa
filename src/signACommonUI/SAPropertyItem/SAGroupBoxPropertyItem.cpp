#include "SAGroupBoxPropertyItem.h"

SAGroupBoxPropertyItem::SAGroupBoxPropertyItem(QWidget* par):QGroupBox(par)
{
    setCheckable(true);
    setFlat(true);
    connect(this,&QGroupBox::toggled,this,&SAGroupBoxPropertyItem::setExpanded);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
}

SAGroupBoxPropertyItem::~SAGroupBoxPropertyItem()
{

}


void SAGroupBoxPropertyItem::setExpanded(bool b)
{
    foreach (QObject *o, children()){
        if (o->isWidgetType())
            ((QWidget *)o)->setVisible(b);
    }
    setFlat(!b);
}

void SAGroupBoxPropertyItem::setCollapsed(bool b)
{
    foreach (QObject *o, children()){
        if (o->isWidgetType())
            ((QWidget *)o)->setVisible(b);
    }
    setFlat(b);
}
