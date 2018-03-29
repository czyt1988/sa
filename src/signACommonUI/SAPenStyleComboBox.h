#ifndef SAPENSTYLECOMBOBOX_H
#define SAPENSTYLECOMBOBOX_H
#include "PenStyleBox.h"

class SAPenStyleComboBox : public PenStyleBox
{
    Q_OBJECT
public:
    SAPenStyleComboBox(QWidget *parent = 0);
private slots:
    Q_SLOT void onComboBoxIndexChanged(int index);
signals:
    void penStyleChanged(Qt::PenStyle style);
};

#endif // SAPENSTYLECOMBOBOX_H
