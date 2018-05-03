#ifndef SAQWTSYMBOLCOMBOBOX_H
#define SAQWTSYMBOLCOMBOBOX_H

#include "SymbolBox.h"
#include "SACommonUIGlobal.h"

class SA_COMMON_UI_EXPORT SAQwtSymbolComboBox : public SymbolBox
{
    Q_OBJECT
public:
    SAQwtSymbolComboBox(bool showNoSymbol = true, QWidget *parent = 0);
private:
    void onCurrentIndexChanged(int index);
signals:
    void symbolSelectChanged(QwtSymbol::Style style);
};

#endif // SAQWTSYMBOLCOMBOBOX_H
