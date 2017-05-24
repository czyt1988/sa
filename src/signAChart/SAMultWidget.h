#ifndef SAMULTWIDGET_H
#define SAMULTWIDGET_H
#include "SAChartGlobals.h"
#include <QWidget>
class QGridLayout;
///
/// \brief 自带一个grid layout 可以
///
class SA_CHART_EXPORT SAMultWidget : public QWidget
{
public:
    explicit SAMultWidget(QWidget *parent = 0);
    //添加窗体
    void addWidget(QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0);
    //获取内部的grid布局
    QGridLayout* getGridLayout() const;
    //当前的窗体
    QWidget *currentWidget() const;
private:
    QGridLayout* m_mainLayout;
    QWidget* m_currentWidget;///< 记录最后一个窗体，也就是激活的窗体
};

#endif // SAMULTWIDGET_H
