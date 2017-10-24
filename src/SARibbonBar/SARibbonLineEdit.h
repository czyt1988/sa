#ifndef SARIBBONLINEEDIT_H
#define SARIBBONLINEEDIT_H
#include <QLineEdit>
#include "SARibbonCtrolContainer.h"


class SARibbonLineEdit : public SARibbonCtrolContainer
{
public:
    SARibbonLineEdit(QWidget *parent = Q_NULLPTR);
    QLineEdit* lineEdit();
protected:
    void initStyleOption(QStyleOption* opt);
private:
    QLineEdit* m_w;
};

#endif // SARIBBONLINEEDIT_H
