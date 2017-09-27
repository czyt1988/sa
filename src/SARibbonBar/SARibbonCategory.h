#ifndef SARIBBONCATEGORY_H
#define SARIBBONCATEGORY_H
#include <QWidget>
#include "SARibbonPannel.h"
class SARibbonCategoryPrivate;
///
/// \brief 一项rubbon页
///
class SARibbonCategory : public QWidget
{
    Q_OBJECT
public:
    SARibbonCategory(QWidget* parent);
    ~SARibbonCategory();
    SARibbonPannel* addPannel(const QString& title);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    SARibbonCategoryPrivate* m_d;
};

#endif // SARIBBONCATEGORY_H
