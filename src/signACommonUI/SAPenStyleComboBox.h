#ifndef SAPENSTYLECOMBOBOX_H
#define SAPENSTYLECOMBOBOX_H
#include "SACommonUIGlobal.h"
#include "PenStyleBox.h"
class SA_COMMON_UI_EXPORT SAPenStyleComboBox : public PenStyleBox
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
