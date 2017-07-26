#ifndef SAPENSETWIDGET_H
#define SAPENSETWIDGET_H
#include <QPen>
#include <QWidget>


class SAPenSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAPenSetWidget(QWidget *parent = 0,Qt::Orientation orient = Qt::Vertical);
    ~SAPenSetWidget();
    QPen getPen() const;
    void setPen(const QPen& pen);
signals:
    void penChanged(const QPen& newPen);
private slots:
    Q_SLOT void onCurrentColorChanged(const QColor& clr);
    Q_SLOT void onCurrentPenWidthChanged(int v);
    Q_SLOT void onCurrentPenStyleChanged(Qt::PenStyle style);
    Q_SLOT void onCurrentPenColorAlphaChanged(int v);
private:
    class UI;
    SAPenSetWidget::UI* ui;
    QPen m_curPen;
    bool m_enableEmit;
};

#endif // SAPENSETWIDGET_H
