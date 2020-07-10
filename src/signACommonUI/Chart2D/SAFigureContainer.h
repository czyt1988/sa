#ifndef SAFIGURECONTAINER_H
#define SAFIGURECONTAINER_H
#include "SACommonUIGlobal.h"
#include <QWidget>
#include <QMap>
#include <QRectF>
class QResizeEvent;
///
/// \brief 一个窗口容器，由于根据比例布局窗口位置
///
class SA_COMMON_UI_EXPORT SAFigureContainer : public QWidget
{
    Q_OBJECT
public:
    SAFigureContainer(QWidget *parent);
    ~SAFigureContainer();
    //添加窗体
    void addWidget(QWidget *widget, const QRectF& pos);

    //添加窗口
    void addWidget(QWidget *widget, float xPresent, float yPresent, float wPresent, float hPresent);

    //获取所有被管理的窗口
    QList<QWidget *> getWidgetList() const;

    //获取窗口的位置信息
    QRectF getWidgetPos(QWidget *w) const;

    //判断是否存在这个widget
    bool isWidgetInContainer(const QWidget *w);

private slots:
    //void onWidgetDestroy(QObject* obj);
protected:
    void resizeEvent(QResizeEvent *event);
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void calcWidgetSize(const QRectF& present, QRect& newSize);

private:
    QMap<QWidget *, QRectF> m_widgetPos;
};

#endif // SAFIGURECONTAINER_H
