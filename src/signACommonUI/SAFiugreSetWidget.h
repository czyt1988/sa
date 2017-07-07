#ifndef SAFIUGRESETWIDGET_H
#define SAFIUGRESETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
class SAFigureWindow;

///
/// \brief 绘图设置窗口
///
class SA_COMMON_UI_EXPORT SAFiugreSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAFiugreSetWidget(QWidget *parent = 0);
    ~SAFiugreSetWidget();
    void setFigureWidget(SAFigureWindow* fig);
private:
    class UI;
    SAFiugreSetWidget::UI * ui;
    SAFigureWindow* m_fig;
};

#endif // SAFIUGRESETWIDGET_H
