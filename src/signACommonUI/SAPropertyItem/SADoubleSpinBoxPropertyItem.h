#ifndef SADOUBLESPINBOXPROPERTYITEM_H
#define SADOUBLESPINBOXPROPERTYITEM_H

#include "SAPropertyItemContainer.h"
class QDoubleSpinBox;
class SADoubleSpinBoxPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    SADoubleSpinBoxPropertyItem(QWidget *parent = nullptr);
    ~SADoubleSpinBoxPropertyItem();
    //
    void setValue(double v);
    //
    double getValue() const;
    //
    QDoubleSpinBox* getDoubleSpinBox() const;
Q_SIGNALS:
    void valueChanged(double v);
private:
    class UI;
    SADoubleSpinBoxPropertyItem::UI *ui;
};

#endif // SADOUBLESPINBOXPROPERTYITEM_H
