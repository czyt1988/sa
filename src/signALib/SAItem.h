#ifndef SAITEM_H
#define SAITEM_H
#include "SALibGlobal.h"
#include <QStandardItem>


class SALIB_EXPORT SAItem : public QStandardItem
{
public:
    SAItem();
    SAItem(const QString & text);
    SAItem(const QIcon & icon, const QString & text);
    virtual ~SAItem();
    void setName(const QString& name);
    QString getName() const;
    virtual QString getTypeName() const = 0;
    virtual int getType() const = 0;
    int getID() const;
protected:
    void setID(int id);
};
#endif // SAITEM_H
