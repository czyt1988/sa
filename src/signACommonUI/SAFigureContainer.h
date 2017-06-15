#ifndef SAFIGURECONTAINER_H
#define SAFIGURECONTAINER_H
#include "SACommonUIGlobal.h"
#include <QWidget>
#include <QMap>
#include <QRectF>
class QResizeEvent;
class SA_COMMON_UI_EXPORT SAFigureContainer : public QWidget
{
public:
    SAFigureContainer(QWidget* parent);
    //添加窗体
    void addWidget(QWidget* widget, const QRectF &pos);
    //添加窗口
    void addWidget(QWidget* widget, float xPresent,float yPresent,float wPresent,float hPresent);
    //获取所有被管理的窗口
    QList<QWidget *> getWidgetList() const;
protected:
    void resizeEvent(QResizeEvent *event);
private:
    void calcWidgetSize(const QRectF& present, QRect& newSize);
private:
    QMap<QWidget*,QRectF> m_widgetPos;
};

#endif // SAFIGURECONTAINER_H
