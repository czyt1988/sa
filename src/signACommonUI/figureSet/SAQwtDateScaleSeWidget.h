#ifndef SAQWTDATESCALESEWIDGET_H
#define SAQWTDATESCALESEWIDGET_H

#include <QWidget>

namespace Ui {
class SAQwtDateScaleSeWidget;
}

class SAQwtDateScaleSeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAQwtDateScaleSeWidget(QWidget *parent = 0);
    ~SAQwtDateScaleSeWidget();
    void setText(const QString& text);
    QString text() const;
signals:
    void timeFormatStringChanged(const QString& format);
private:
    Ui::SAQwtDateScaleSeWidget *ui;
};

#endif // SAQWTDATESCALESEWIDGET_H
