#ifndef SAALIGMENTSETWIDGET_H
#define SAALIGMENTSETWIDGET_H

#include <QWidget>

#include "SACommonUIGlobal.h"

class SA_COMMON_UI_EXPORT SAAligmentSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAAligmentSetWidget(QWidget *parent = 0);
    ~SAAligmentSetWidget();
    void setAlignment(Qt::Alignment al);

signals:
    void alignmentStateChanged(Qt::Alignment al);
private slots:
    void onButtonClicked(int id);
private:

    class UI;
    SAAligmentSetWidget::UI* ui;
};

#endif // SAALIGMENTSETWIDGET_H
