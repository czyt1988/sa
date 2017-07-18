#ifndef SAGROUPBOXPROPERTYITEM_H
#define SAGROUPBOXPROPERTYITEM_H
#include <QGroupBox>

class SAGroupBoxPropertyItem : public QGroupBox
{
    Q_OBJECT
public:
    SAGroupBoxPropertyItem(QWidget* par = nullptr);
    ~SAGroupBoxPropertyItem();
public slots:
    Q_SLOT void setExpanded(bool b);
    Q_SLOT void setCollapsed(bool b);
};

#endif // SAGROUPBOXPROPERTYITEM_H
