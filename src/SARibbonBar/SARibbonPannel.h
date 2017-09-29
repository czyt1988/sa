#ifndef SARIBBONPANNEL_H
#define SARIBBONPANNEL_H
#include <QWidget>
class SARibbonToolButton;
class QGridLayout;
///
/// \brief The SARibbonPannel class
///
class SARibbonPannel : public QWidget
{
    Q_OBJECT
public:
    SARibbonPannel(QWidget* parent = 0);
    using QWidget::addAction;
    SARibbonToolButton* addLargeAction(QAction *action);
    SARibbonToolButton *addSmallAction(QAction *action);
    void addWidget(QWidget* w);
protected:
    static QSize maxHightIconSize(const QSize& size,int height);
    void paintEvent(QPaintEvent *event);
private:
    QGridLayout* m_gridLayout;
    QPoint m_nextElementPosition;
    int m_row;
};

#endif // SARIBBONPANNEL_H
