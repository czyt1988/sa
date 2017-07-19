#ifndef SAVGROUPBOXPROPERTYITEM_H
#define SAVGROUPBOXPROPERTYITEM_H

#include "SAGroupBoxPropertyItem.h"
class QVBoxLayout;
///
/// \ingroup CommonUI
/// \brief 可伸缩groupbox，伸缩状态时，
/// groupbox的CheckBox可以设置为树形控件的箭头模式 \sa SAVGroupBoxPropertyItem::setTreeMode
///
class SAVGroupBoxPropertyItem : public SAGroupBoxPropertyItem
{
    Q_OBJECT
public:
    SAVGroupBoxPropertyItem(QWidget* par = nullptr);
    SAVGroupBoxPropertyItem(const QString& title,QWidget* par = nullptr);
    ~SAVGroupBoxPropertyItem();
    void addWidget(QWidget* w, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void addLayout(QLayout *layout, int stretch = 0);
private:
    class UI;
    SAVGroupBoxPropertyItem::UI* ui;
};

#endif // SAVGROUPBOXPROPERTYITEM_H
