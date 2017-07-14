#ifndef SAPLOTITEMSETWIDGET_H
#define SAPLOTITEMSETWIDGET_H
#include <QWidget>

class SAPlotItemSetWidget : public QWidget
{
public:
    explicit SAPlotItemSetWidget(QWidget* par);
    ~SAPlotItemSetWidget();
private:
    class UI;
    SAPlotItemSetWidget::UI* ui;
};

#endif // SAPLOTITEMSETWIDGET_H
