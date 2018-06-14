#ifndef SAFIUGRESETWIDGET_H
#define SAFIUGRESETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
#define SAFiugreSetWidget_USE_COMBOX 1
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
#ifdef SAFiugreSetWidget_USE_COMBOX
    void clear();
#endif
    //更新内容
    void updateData();
    void updatePlotItemsSet();
    void updateAxesSet();
    void updateNormalSet();
private slots:
    void onChartTitleChanged(const QString& text);
    void onPlotDestroy(QObject* obj);
    void onFigutrDestroy(QObject* obj);
#ifdef SAFiugreSetWidget_USE_COMBOX
    void onComboxChanged(int index);
#else
private:
    void disconnectOldFigure();
#endif
private:
    class UI;
    SAFigureSetWidget::UI * ui;
    SAFigureWindow* m_fig;
};

#endif // SAFIUGRESETWIDGET_H
