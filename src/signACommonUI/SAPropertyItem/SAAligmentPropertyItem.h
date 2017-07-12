#ifndef SAALIGMENTPROPERTYITEM_H
#define SAALIGMENTPROPERTYITEM_H
#include "SAPropertyItemContainer.h"
class SAAligmentPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    SAAligmentPropertyItem(QWidget* par);
    ~SAAligmentPropertyItem();
signals:
    void stateChanged(Qt::Alignment al);
private slots:
    void onButtonBottomToggled(bool checked);
    void onButtonTopToggled(bool checked);
    void onButtonLeftToggled(bool checked);
    void onButtonRightToggled(bool checked);
private:
    class UI;
    SAAligmentPropertyItem::UI* ui;
};

#endif // SAALIGMENTPROPERTYITEM_H
