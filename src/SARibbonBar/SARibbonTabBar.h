#ifndef SARIBBONTABBAR_H
#define SARIBBONTABBAR_H
#include <QTabBar>
#include <QMargins>
class SARibbonTabBar : public QTabBar
{
    Q_OBJECT
public:
    SARibbonTabBar(QWidget *parent = Q_NULLPTR);
    QMargins tabMargin() const;
    void setTabMargin(const QMargins &tabMargin);

private:
    QMargins m_tabMargin;
};

#endif // SARIBBONTABBAR_H
