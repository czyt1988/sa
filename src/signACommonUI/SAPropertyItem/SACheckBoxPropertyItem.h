#ifndef SACHECKBOXPROPERTYITEM_H
#define SACHECKBOXPROPERTYITEM_H
#include "SAPropertyItemContainer.h"
class QCheckBox;
class SACheckBoxPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    SACheckBoxPropertyItem(QWidget *parent = nullptr);
    ~SACheckBoxPropertyItem();

    void setCheckState(Qt::CheckState state);

    Qt::CheckState checkState() const;

Q_SIGNALS:

    void stateChanged(int state);

private:
    class UI;
    SACheckBoxPropertyItem::UI *ui;
};

#endif // SACHECKBOXPROPERTYITEM_H
