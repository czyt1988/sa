#ifndef SAFIUGRESETWIDGET_H
#define SAFIUGRESETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
#define SAFiugreSetWidget_USE_COMBOX 0
class SAFigureWindow;

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
private slots:
    void onChartTitleChanged(const QString& text);
    void onPlotDestroy(QObject* obj);
    void onFigutrDestroy(QObject* obj);
#if SAFiugreSetWidget_USE_COMBOX
    void onComboxChanged(int index);
#endif
private:
    class UI;
    SAFigureSetWidget::UI * ui;
    SAFigureWindow* m_fig;
};

#endif // SAFIUGRESETWIDGET_H
