#ifndef SAMESSAGEWIDGET_H
#define SAMESSAGEWIDGET_H

#include <QWidget>
#include <QColor>
namespace Ui {
class SAMessageWidget;
}

class SAMessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAMessageWidget(QWidget *parent = 0);
    ~SAMessageWidget();
public slots:
    void addString(const QString& str,QColor clr = Qt::black);
    void insertTimeLine(QColor clr = Qt::red);
    void addStringWithTime(const QString& str,QColor clr = Qt::black);
private:
    Ui::SAMessageWidget *ui;
};

#endif // SAMESSAGEWIDGET_H
