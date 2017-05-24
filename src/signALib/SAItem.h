#ifndef SAITEM_H
#define SAITEM_H
#include "SALibGlobal.h"
#include "SAGlobals.h"
#include <QStandardItem>


class SALIB_EXPORT SAItem : public QStandardItem
{
public:
    SAItem():QStandardItem()
    {
        setID(int(this));
    }


    SAItem(const QString & text):QStandardItem(text)
    {
        setID(int(this));
    }

    SAItem(const QIcon & icon, const QString & text)
        :QStandardItem(icon,text)
    {
        setID(int(this));
    }

    virtual ~SAItem(){}
    virtual void setName(const QString& name){QStandardItem::setText (name);}
    virtual QString getName() const{return QStandardItem::text ();}
    virtual QString getTypeName() const = 0;
    virtual int getID() const
    {
        QVariant var = data(SA_ROLE_DATA_ID);
        if(!var.isValid ())
            return 0;
        return var.toInt ();
    }

    virtual int getType() const = 0;
protected:
    void setID(int id)
    {setData (id,SA_ROLE_DATA_ID);}
};
#endif // SAITEM_H
