#ifndef SARIBBONPANNEL_H
#define SARIBBONPANNEL_H
#include <QWidget>
class SARibbonToolButton;
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
protected:
    static QSize maxHightIconSize(const QSize& size,int height);
private:
    QPoint m_nextElementPosition;
};

#endif // SARIBBONPANNEL_H
