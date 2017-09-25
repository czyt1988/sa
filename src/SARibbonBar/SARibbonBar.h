#ifndef SARIBBONBAR_H
#define SARIBBONBAR_H
#include <QWidget>
#include <QVariant>
#include "SARibbonCategory.h"
#include "SARibbonContextCategory.h"
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
    //添加一个标签
    SARibbonCategory* addCategoryPage(const QString& title);
    //添加一个上下文标签
    SARibbonContextCategory* addContextCategory(const QString& title,const QColor& color,const QVariant& id=QVariant());
    //显示一个上下文标签
    void showContextCategory(SARibbonContextCategory* context);
    //隐藏一个上下文标签
    void hideContextCategory(SARibbonContextCategory* context);
    //设置上下文标签的显示或隐藏
    void setContextCategoryVisible(SARibbonContextCategory* context,bool visible);
signals:
    void applitionButtonClicked();
    //
    void currentRibbonTabChanged(int index);

private slots:
    void onWindowTitleChanged(const QString &title);
    void onWindowIconChanged(const QIcon &icon);
    void onCurrentRibbonTabChanged(int index);
    void onRibbonTabBarClicked(int index);
    void onRibbonTabCloseRequested(int index);
    void onContextsCategoryPageAdded(SARibbonCategory* category);

protected:
    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent* e);
    virtual void paintBackground(QPainter& painter);
    virtual void paintWindowTitle(QPainter& painter, const QString &title);
    virtual void paintWindowIcon(QPainter& painter, const QIcon &icon);
   // virtual void paintContextCategoryTab(QPainter& painter);
private:
    friend class SARibbonBarPrivate;
    SARibbonBarPrivate* m_d;
};

#endif // SARIBBONBAR_H
