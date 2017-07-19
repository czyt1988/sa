#ifndef SAGROUPBOXPROPERTYITEM_H
#define SAGROUPBOXPROPERTYITEM_H
#include <QGroupBox>
#include "ctkCollapsibleGroupBox.h"
class SACollapsibleGroupBox : public ctkCollapsibleGroupBox
{
    Q_OBJECT
public:
    SACollapsibleGroupBox(QWidget* par = nullptr);
    SACollapsibleGroupBox(const QString& title,QWidget* par = nullptr);
    ~SACollapsibleGroupBox();
    void setTreeMode(bool b = true);
};

#endif // SAGROUPBOXPROPERTYITEM_H
