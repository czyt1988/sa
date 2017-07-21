#include "SAQwtSymbolComboBox.h"

SAQwtSymbolComboBox::SAQwtSymbolComboBox(bool showNoSymbol, QWidget *parent)
    :SymbolBox(showNoSymbol,parent)
{
    connect(this,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
            ,this,&SAQwtSymbolComboBox::onCurrentIndexChanged);
}

void SAQwtSymbolComboBox::onCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    emit symbolSelectChanged(this->selectedSymbol());
}
