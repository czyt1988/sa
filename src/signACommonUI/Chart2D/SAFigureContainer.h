#ifndef SAFIGURECONTAINER_H
#define SAFIGURECONTAINER_H
#include "SACommonUIGlobal.h"
#include <QWidget>
#include <QMap>
#include <QRectF>
SA_IMPL_FORWARD_DECL(SAFigureContainer)
class QResizeEvent;

/**
 * @brief 一个窗口容器，由于根据比例布局窗口位置
 *
 * 被管理的窗口可以直接设置位置移动，此窗口会监听事件调整对应的比例
 */
class SA_COMMON_UI_EXPORT SAFigureContainer : public QWidget
{
    Q_OBJECT
    SA_IMPL(SAFigureContainer)
public:
    SAFigureContainer(QWidget *parent);
    ~SAFigureContainer();
    //添加窗体
    void addWidget(QWidget *widget, const QRectF& posPercent);

    //添加窗口
    void addWidget(QWidget *widget, float xPercent, float yPercent, float wPercent, float hPercent);

    //获取所有被管理的窗口
    QList<QWidget *> getWidgetList() const;

    //获取窗口的位置信息
    QRectF getWidgetPosPercent(QWidget *w) const;
    void setWidgetPosPercent(QWidget *w, const QRectF& posPercent);
    void setWidgetPosPercent(QWidget *w, float xPercent, float yPercent, float wPercent, float hPercent);
    void updateWidgetPos(QWidget *w, const QRect& rect);

    //判断是否存在这个widget
    bool isWidgetInContainer(const QWidget *w);

    //设置允许在窗口改变的时候重新修改widgetPosPersent,此函数需要和endResetSubWidget配套
    void beginResetSubWidget();
    void endResetSubWidget();

    //计算size对应SAFigureContainer的位置占比
    static QRectF calcWidgetPosPercent(SAFigureContainer *c, const QRect& size);

    //通过窗口Percent计算窗口尺寸
    static QRect calcWidgetRectByPercent(SAFigureContainer *c, const QRectF& percent);

    //设置浮点数的精度，默认为3位
    static qreal setRealPrecision(qreal v, int precision = 3);

private slots:
    //void onWidgetDestroy(QObject* obj);
protected:
    void resizeEvent(QResizeEvent *event);
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void calcWidgetSize(const QRectF& present, QRect& newSize);
};

#endif // SAFIGURECONTAINER_H
