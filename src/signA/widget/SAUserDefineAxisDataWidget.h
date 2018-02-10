#ifndef SAUSERDEFINEAXISDATAWIDGET_H
#define SAUSERDEFINEAXISDATAWIDGET_H

#include <QWidget>

namespace Ui {
class SAUserDefineAxisDataWidget;
}
///
/// \brief 用于设置自定义开始值和增加值的窗口
///
class SAUserDefineAxisDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAUserDefineAxisDataWidget(QWidget *parent = 0);
    ~SAUserDefineAxisDataWidget();
    double getStartValue() const;
    double getIncreaseValue() const;
    void setStartValue(double s);
    void setIncreaseValue(double s);
private slots:
    void onStartValueChanged(double d);
    void onIncreaseValueChanged(double d);
private:
    void updateText();
private:
    Ui::SAUserDefineAxisDataWidget *ui;
};

#endif // SAUSERDEFINEAXISDATAWIDGET_H

