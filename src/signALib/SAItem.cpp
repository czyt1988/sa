#include "SAItem.h"



SAItem::SAItem():QStandardItem()
{
    setID(int(this));
}

SAItem::SAItem(const QString &text):QStandardItem(text)
{
    setID(int(this));
}

SAItem::SAItem(const QIcon &icon, const QString &text):QStandardItem(icon,text)
{
    setID(int(this));
}

SAItem::~SAItem()
{

}

void SAItem::setName(const QString &name)
{
    QStandardItem::setText (name);
}

QString SAItem::getName() const
{
    return QStandardItem::text ();
}

int SAItem::getID() const
{
    QVariant var = data(SA_ROLE_DATA_ID);
    if(!var.isValid ())
        return 0;
    return var.toInt ();
}

void SAItem::setID(int id)
{
    setData (id,SA_ROLE_DATA_ID);
}
