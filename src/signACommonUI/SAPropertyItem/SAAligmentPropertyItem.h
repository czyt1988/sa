#ifndef SAALIGMENTPROPERTYITEM_H
#define SAALIGMENTPROPERTYITEM_H
#include "SAPropertyItemContainer.h"
class SAAligmentPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    SAAligmentPropertyItem(QWidget* par = nullptr);
    ~SAAligmentPropertyItem();
    void setAlignment(Qt::Alignment al);
signals:
    void stateChanged(Qt::Alignment al);
private:
    class UI;
    SAAligmentPropertyItem::UI* ui;
};

#endif // SAALIGMENTPROPERTYITEM_H
