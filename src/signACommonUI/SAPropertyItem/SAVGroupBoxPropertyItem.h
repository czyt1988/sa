#ifndef SAVGROUPBOXPROPERTYITEM_H
#define SAVGROUPBOXPROPERTYITEM_H

#include "SAGroupBoxPropertyItem.h"
class QVBoxLayout;
class SAVGroupBoxPropertyItem : public SAGroupBoxPropertyItem
{
    Q_OBJECT
public:
    SAVGroupBoxPropertyItem(QWidget* par);
    ~SAVGroupBoxPropertyItem();
    void addWidget(QWidget* w, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void addLayout(QLayout *layout, int stretch = 0);
private:
    class UI;
    SAVGroupBoxPropertyItem::UI* ui;
};

#endif // SAVGROUPBOXPROPERTYITEM_H
