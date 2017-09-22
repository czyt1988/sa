#ifndef SARIBBONBAR_H
#define SARIBBONBAR_H
#include <QWidget>
#include "SARibbonCategory.h"
class SARibbonBarPrivate;
class QAbstractButton;
class SARibbonTabBar;
class SARibbonBar : public QWidget
{
    Q_OBJECT
public:
    SARibbonBar(QWidget* parent);
    //设置ribbon的背景
    void setRibbonBarBackground(const QBrush& brush);
    //获取applitionButton
    QAbstractButton* applitionButton();
    //设置applitionButton
    void setApplitionButton(QAbstractButton* btn);
    //获取tabbar
    SARibbonTabBar* ribbonTabBar();
    //
    SARibbonCategory* addCategory(const QString& title);
signals:
    void applitionButtonClicked();
    //
    void currentRibbonTabChanged(int index);
    void ribbonTabBarClicked(int index);
    void ribbonTabBarDoubleClicked(int index);
    void ribbonTabCloseRequested(int index);
    void ribbonTabMoved(int from, int to);

private slots:
    void onWindowTitleChanged(const QString &title);
    void onWindowIconChanged(const QIcon &icon);
protected:
    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent* e);
    virtual void paintBackground(QPainter& painter);
    virtual void paintWindowTitle(QPainter& painter, const QString &title);
    virtual void paintWindowIcon(QPainter& painter, const QIcon &icon);
private:
    SARibbonBarPrivate* m_d;
};

#endif // SARIBBONBAR_H
