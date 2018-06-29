#ifndef SAITEM_H
#define SAITEM_H
#include "SALibGlobal.h"
#include <QStandardItem>

class SAItemPrivate;
///
/// \ingroup SALib
/// \brief The SAItem class
///
class SALIB_EXPORT SAItem //: public QStandardItem
{
    SA_IMPL(SAItem)
public:
    SAItem();
    SAItem(const QString & text);
    SAItem(const QIcon & icon, const QString & text);
    virtual ~SAItem();

    void setName(const QString& name);
    QString getName() const;

    void setIcon(const QIcon& icon);
    const QIcon& getIcon() const;

    int getID() const;

    void setData(int roleID,const QVariant& var);
    bool isHaveData(int id) const;
    int getDataCount() const;


    const QVariant& getData(int id) const;
    QVariant& getData(int id);
    void getData(int index,int& id,QVariant& var) const;

    int childCount() const;
    SAItem *child(int row) const;
    void appendChild(SAItem* item);
    void insertChild(SAItem* item,int row);
    void clearChild();
    SAItem* takeChild(int row);
    SAItem* parent() const;
    //用于记录当前所处的层级，如果parent不为nullptr，这个将返回parent下次item对应的层级,如果没有parent，返回-1
    int fieldRow() const;

    virtual QString getTypeName() const = 0;
    virtual int getType() const = 0;
protected:
    void setID(int id);
};
#endif // SAITEM_H
