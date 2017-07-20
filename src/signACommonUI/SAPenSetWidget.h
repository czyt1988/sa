#ifndef SAPENSETWIDGET_H
#define SAPENSETWIDGET_H
#include <QPen>
#include <QWidget>

namespace Ui {
class SAPenSetWidget;
}

class SAPenSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAPenSetWidget(QWidget *parent = 0);
    ~SAPenSetWidget();
    QPen getPen() const;
    void setPen(const QPen& pen);
private:
    Ui::SAPenSetWidget *ui;
};

#endif // SAPENSETWIDGET_H
