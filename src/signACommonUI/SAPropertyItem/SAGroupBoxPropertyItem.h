#ifndef SAGROUPBOXPROPERTYITEM_H
#define SAGROUPBOXPROPERTYITEM_H
#include <QGroupBox>
#include "ctkCollapsibleGroupBox.h"
class SAGroupBoxPropertyItem : public ctkCollapsibleGroupBox
{
    Q_OBJECT
public:
    enum IndicatorMode
    {
        TreeMode
        ,CheckBoxMode
    };

    SAGroupBoxPropertyItem(QWidget* par = nullptr);
    SAGroupBoxPropertyItem(const QString& title,QWidget* par = nullptr);
    ~SAGroupBoxPropertyItem();
    void setTreeMode(bool b = true);
};

#endif // SAGROUPBOXPROPERTYITEM_H
