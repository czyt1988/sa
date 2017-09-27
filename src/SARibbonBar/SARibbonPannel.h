#ifndef SARIBBONPANNEL_H
#define SARIBBONPANNEL_H
#include <QWidget>
///
/// \brief The SARibbonPannel class
///
class SARibbonPannel : public QWidget
{
    Q_OBJECT
public:
    SARibbonPannel(QWidget* parent = 0);
    using QWidget::addAction;
    void addLargeAction(QAction *action);
};

#endif // SARIBBONPANNEL_H
