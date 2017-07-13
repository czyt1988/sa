#ifndef SAALIGMENTPROPERTYITEM_H
#define SAALIGMENTPROPERTYITEM_H
#include "SAPropertyItemContainer.h"
class SAAligmentPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    SAAligmentPropertyItem(QWidget* par);
    ~SAAligmentPropertyItem();
    void setAlignment(Qt::Alignment al);

    QSize sizeHint() const;
signals:
    void stateChanged(Qt::Alignment al);
private slots:
    void onButtonClicked(int id);
private:
    class UI;
    SAAligmentPropertyItem::UI* ui;
};

#endif // SAALIGMENTPROPERTYITEM_H
