#ifndef SASETWIDGET_H
#define SASETWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include "SAFigureSetWidget.h"
#include "SAChartSetWidget.h"
#include "SAFigureWindow.h"
#include <QTabBar>
#include <QTabWidget>

class SASetWidgetTabBar : public QTabBar
{
    Q_OBJECT
public:
    SASetWidgetTabBar(QWidget *p = nullptr);
    ~SASetWidgetTabBar();
};

class SASetWidgetTab : public QTabWidget
{
    Q_OBJECT
public:
    SASetWidgetTab(QWidget *p = nullptr);
    ~SASetWidgetTab();
};

namespace Ui {
class SASetWidget;
}

class SASetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SASetWidget(QWidget *parent = nullptr);
    ~SASetWidget();
    //设置figure
    void setFigure(SAFigureWindow *fig);

    //更新某个plotitem的内容
    void setPlotItem(QwtPlotItem *it);

    //设置图表
    void setChart(SAChart2D *chart);

signals:
    void chartTitleChanged(SAChart2D *chart, const QString& text);

private:
    Ui::SASetWidget *ui;
    SAFigureSetWidget *m_figureSetWidget;
    SAChartSetWidget *m_chartSetWidget;
};

#endif // SASETWIDGET_H
