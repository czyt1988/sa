#ifndef SAFIGURECANVASSETWIDGET_H
#define SAFIGURECANVASSETWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include "SAFigureWindow.h"
class SAFigureCanvasSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAFigureCanvasSetWidget(QWidget* parent = nullptr);
    ~SAFigureCanvasSetWidget();
    void setFigureWindow(SAFigureWindow* fig);
private:
    class UI;
    SAFigureCanvasSetWidget::UI* ui;
    SAFigureWindow* m_fig;
};

#endif // SAFIGURECANVASSETWIDGET_H
