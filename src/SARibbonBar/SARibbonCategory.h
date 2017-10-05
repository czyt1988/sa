#ifndef SARIBBONCATEGORY_H
#define SARIBBONCATEGORY_H
#include "SARibbonGlobal.h"
#include <QWidget>
#include "SARibbonPannel.h"
class SARibbonCategoryPrivate;
///
/// \brief 一项rubbon页
///
class SA_RIBBON_EXPORT SARibbonCategory : public QWidget
{
    Q_OBJECT
public:
    SARibbonCategory(QWidget* parent);
    ~SARibbonCategory();
    SARibbonPannel* addPannel(const QString& title);
    void setBackgroundBrush(const QBrush& brush);
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    //绘制分割线
    virtual QPixmap drawSeparatorPixmap() const;
private:
    SARibbonCategoryPrivate* m_d;
};

#endif // SARIBBONCATEGORY_H
