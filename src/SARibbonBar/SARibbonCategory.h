#ifndef SARIBBONCATEGORY_H
#define SARIBBONCATEGORY_H
#include "SARibbonGlobal.h"
#include <QWidget>
#include "SARibbonPannel.h"
#include <QScopedPointer>
class SARibbonCategoryPrivate;
class SARibbonCategoryProxy;
///
/// \brief 一项ribbon页
///
class SA_RIBBON_EXPORT SARibbonCategory : public QWidget
{
    Q_OBJECT
public:
    SARibbonCategory(QWidget* parent);
    ~SARibbonCategory();
    SARibbonPannel* addPannel(const QString& title);
    void setBackgroundBrush(const QBrush& brush);
    SARibbonCategoryProxy* proxy();
    void setProxy(SARibbonCategoryProxy* proxy);
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QScopedPointer<SARibbonCategoryProxy> m_proxy;
};

///
/// \brief ribbon页的代理类
/// 如果需要修改重绘SARibbonCategory，可以通过设置SARibbonCategory::setProxy
///
class SA_RIBBON_EXPORT SARibbonCategoryProxy
{
public:
    SARibbonCategoryProxy(SARibbonCategory* parent);
    virtual ~SARibbonCategoryProxy();

    virtual SARibbonPannel* addPannel(const QString& title);
    virtual void setBackgroundBrush(const QBrush& brush);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    //绘制分割线
    virtual QPixmap drawSeparatorPixmap() const;
    SARibbonCategory* ribbonCategory();
protected:
    static QPixmap* category_separator_pixmap;
private:
    int showNormalPannel(SARibbonPannel* pannel, int index, int x);
    void buildReduceModePannel(SARibbonPannel* pannel, int x);
    static QPoint calcPopupPannelPosition(SARibbonCategory* category,SARibbonPannel *pannel, int x);
private:
    SARibbonCategoryPrivate* m_d;
};

#endif // SARIBBONCATEGORY_H
