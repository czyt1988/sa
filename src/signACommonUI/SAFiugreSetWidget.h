#ifndef SAFIUGRESETWIDGET_H
#define SAFIUGRESETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
#define SAFiugreSetWidget_USE_COMBOX 1
class SAFigureWindow;

///
/// \brief 绘图设置窗口
///
class SA_COMMON_UI_EXPORT SAFiugreSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAFiugreSetWidget(QWidget *parent = nullptr);
    ~SAFiugreSetWidget();
    void setFigureWidget(SAFigureWindow* fig);
private slots:
    void onChartTitleChanged(const QString& text);
    void onPlotDestroy(QObject* obj);
#ifdef SAFiugreSetWidget_USE_COMBOX
    void onComboxChanged(int index);
#else
private:
    void disconnectOldFigure();
#endif
private:
    class UI;
    SAFiugreSetWidget::UI * ui;
    SAFigureWindow* m_fig;
};

#endif // SAFIUGRESETWIDGET_H
