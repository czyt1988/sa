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
signals:
    void penChanged(const QPen& newPen);
private slots:
    Q_SLOT void onCurrentColorChanged(const QColor& clr);
    Q_SLOT void onCurrentPenWidthChanged(int v);
    Q_SLOT void onCurrentPenStyleChanged(Qt::PenStyle style);
private:
    Ui::SAPenSetWidget *ui;
    QPen m_curPen;
    bool m_enableEmit;
};

#endif // SAPENSETWIDGET_H
