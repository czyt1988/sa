#ifndef SAFIUGRESETWIDGET_H
#define SAFIUGRESETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
#define SAFiugreSetWidget_USE_COMBOX 0
class SAFigureWindow;
class SAChart2D;

///
/// \brief 绘图设置窗口
///
class SA_COMMON_UI_EXPORT SAFigureSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAFigureSetWidget(QWidget *parent = nullptr);
    ~SAFigureSetWidget();
    void setFigureWidget(SAFigureWindow* fig);
    void clear();
    //更新内容
    void updateData();
    void updatePlotItemsSet();
    void updateAxesSet();
    void updateNormalSet();
signals:
    void chartTitleChanged(QwtPlot* chart,const QString& title);
private slots:
    void onFigutrDestroy(QObject* obj);
    //当前绘图窗口选中的窗体对象发生改变
    void onCurrentFigureWidgetChanged(QWidget* w);
private:
    void setCurrentChart(SAChart2D* chart);

private:
    class UI;
    SAFigureSetWidget::UI * ui;
    SAFigureWindow* m_fig;
};

#endif // SAFIUGRESETWIDGET_H
