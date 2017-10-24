#ifndef SARIBBONCOMBOBOX_H
#define SARIBBONCOMBOBOX_H
#include <QComboBox>
#include "SARibbonCtrolContainer.h"

///
/// \brief QComboBox的Ribbon显示，可以显示QIcon和windowTitle在左侧
///
class SARibbonComboBox : public SARibbonCtrolContainer
{
public:
    SARibbonComboBox(QWidget *parent = Q_NULLPTR);
    QComboBox* comboBox();
protected:
    void initStyleOption(QStyleOption* opt);
private:
    QComboBox* m_w;
};

#endif // SARIBBONCOMBOBOX_H
