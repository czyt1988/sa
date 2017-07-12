#ifndef SADOUBLESPINBOXPROPERTYITEM_H
#define SADOUBLESPINBOXPROPERTYITEM_H

#include "SAPropertyItemContainer.h"
class QDoubleSpinBox;
class SADoubleSpinBoxPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    explicit SADoubleSpinBoxPropertyItem(QWidget *parent = nullptr);
    ~SADoubleSpinBoxPropertyItem();
    //
    void setValue(double v);
    //
    double getValue() const;
    //
    void setMinimum(double v);

    void setMaximum(double v);
    //
    QDoubleSpinBox* getDoubleSpinBox() const;
Q_SIGNALS:
    void valueChanged(double v);
private:
    class UI;
    SADoubleSpinBoxPropertyItem::UI *ui;
};

#endif // SADOUBLESPINBOXPROPERTYITEM_H
