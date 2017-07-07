#ifndef SACOLORSETPROPERTYITEM_H
#define SACOLORSETPROPERTYITEM_H

#include <QWidget>

namespace Ui {
class SAColorSetPropertyItem;
}

class SAColorSetPropertyItem : public QWidget
{
    Q_OBJECT

public:
    explicit SAColorSetPropertyItem(QWidget *parent = 0);
    ~SAColorSetPropertyItem();

private:
    Ui::SAColorSetPropertyItem *ui;
};

#endif // SACOLORSETPROPERTYITEM_H
