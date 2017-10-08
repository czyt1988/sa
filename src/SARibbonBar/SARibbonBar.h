#ifndef SARIBBONBAR_H
#define SARIBBONBAR_H
#include "SARibbonGlobal.h"
#include <QWidget>
#include <QVariant>
#include "SARibbonCategory.h"
#include "SARibbonContextCategory.h"
#include <QScopedPointer>
class SARibbonBarPrivate;
class QAbstractButton;
class SARibbonTabBar;
class SARibbonBarSubElementCreateProxy;
///
/// \brief The SARibbonBar class
///
class SA_RIBBON_EXPORT SARibbonBar : public QWidget
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
    //设置子元素创建代理，此代理的生命周期由此类控制
    void setSubElementCreateProxy(SARibbonBarSubElementCreateProxy* proxy);
protected:
    SARibbonBarSubElementCreateProxy* subElementCreateProxy();
signals:
    void applitionButtonClicked();
    //
    void currentRibbonTabChanged(int index);

private slots:
    void onWindowTitleChanged(const QString &title);
    void onWindowIconChanged(const QIcon &icon);
    void onCurrentRibbonTabChanged(int index);
    void onContextsCategoryPageAdded(SARibbonCategory* category);

protected:
    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent* e);
    virtual void paintBackground(QPainter& painter);
    virtual void paintWindowTitle(QPainter& painter, const QString &title,const QPoint& contextCategoryRegion);
    virtual void paintWindowIcon(QPainter& painter, const QIcon &icon);
    virtual void paintContextCategoryTab(QPainter& painter,const QString& title, QRect contextRect, const QColor& color);
private:
    friend class SARibbonBarPrivate;
    QScopedPointer<SARibbonBarSubElementCreateProxy> m_createProxy;
    SARibbonBarPrivate* m_d;
};

class SARibbonApplicationButton;
///
/// \brief RibbonBar 子元素创建调用的代理类
/// 如果重写了\sa SARibbonCategory \sa SARibbonTabBar \sa SARibbonApplicationButton 可以通过重载此代理
/// 来对子元素创建进行重定向
///
class SA_RIBBON_EXPORT SARibbonBarSubElementCreateProxy
{
public:
    SARibbonBarSubElementCreateProxy(SARibbonBar* parent);
    virtual ~SARibbonBarSubElementCreateProxy();


    SARibbonBar* ribbonBar();
    virtual SARibbonTabBar* createRibbonTabBar();
    virtual SARibbonApplicationButton* createRibbonApplicationButton();
    virtual SARibbonCategory* createRibbonCategory();
private:
    SARibbonBar* m_ribbonBar;
};

#endif // SARIBBONBAR_H
