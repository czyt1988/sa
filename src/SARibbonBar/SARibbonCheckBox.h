#ifndef SARIBBONCHECKBOX_H
#define SARIBBONCHECKBOX_H
#include <QCheckBox>
#include "SARibbonCtrolContainer.h"
class SARibbonCheckBox : public SARibbonCtrolContainer
{
public:
    SARibbonCheckBox(QWidget *parent = Q_NULLPTR);
    QCheckBox* checkBox();
protected:
    void initStyleOption(QStyleOption* opt);
private:
    QCheckBox* m_w;
};

#endif // SARIBBONCHECKBOX_H
