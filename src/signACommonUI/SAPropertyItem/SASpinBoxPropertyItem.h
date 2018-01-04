#ifndef SASPINBOXPROPERTYITEM_H
#define SASPINBOXPROPERTYITEM_H

#include "SAPropertyItemContainer.h"
class QSpinBox;
class SASpinBoxPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    explicit SASpinBoxPropertyItem(QWidget *parent = nullptr);
    ~SASpinBoxPropertyItem();
    //
    void setValue(int v);
    //
    int getValue() const;
    //
    QSpinBox* getSpinBox() const;
    //
    void setMinimum(int v);

    void setMaximum(int v);
    void setMinMax(int min,int max);
Q_SIGNALS:
    void valueChanged(int v);
private:
    class UI;
    SASpinBoxPropertyItem::UI *ui;
};

#endif // SACOLORSETPROPERTYITEM_H
