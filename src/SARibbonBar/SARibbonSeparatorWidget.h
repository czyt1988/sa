#ifndef SARIBBONSEPARATORWIDGET_H
#define SARIBBONSEPARATORWIDGET_H
#include "SARibbonGlobal.h"
#include <QWidget>
#include <QStyleOption>
///
/// \brief 用于显示分割线
///
class SA_RIBBON_EXPORT SARibbonSeparatorWidget : public QWidget
{
public:
    SARibbonSeparatorWidget(int height,QWidget* parent = nullptr);
    virtual QSize sizeHint() const;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SARIBBONSEPARATORWIDGET_H
