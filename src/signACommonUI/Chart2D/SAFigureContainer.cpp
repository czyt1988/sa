#include "SAFigureContainer.h"
#include <math.h>
#include <QResizeEvent>
#include <QDebug>
#include <QChildEvent>
#define PRECISION    5

class SAFigureContainerPrivate {
public:
    SA_IMPL_PUBLIC(SAFigureContainer)
    QMap<QWidget *, QRectF> mWidgetPos;
    bool mIsResetWidgetPos;
    SAFigureContainerPrivate(SAFigureContainer *p) : q_ptr(p)
        , mIsResetWidgetPos(false)
    {
    }
};

SAFigureContainer::SAFigureContainer(QWidget *parent) : QWidget(parent)
    , d_ptr(new SAFigureContainerPrivate(this))
{
}


SAFigureContainer::~SAFigureContainer()
{
    //qDebug() <<"SAFigureContainer destroy";
}


void SAFigureContainer::addWidget(QWidget *widget, const QRectF& posPercent)
{
    if (widget->parentWidget() != this) {
        widget->setParent(this);
    }
    QRect widgetSize;

    calcWidgetSize(posPercent, widgetSize);
    widget->setGeometry(widgetSize);
    //connect(widget,&QObject::destroyed,this,&SAFigureContainer::onWidgetDestroy);
    (d_ptr->mWidgetPos)[widget] = posPercent;
    widget->installEventFilter(this);
}


void SAFigureContainer::addWidget(QWidget *widget, float xPercent, float yPercent, float wPercent, float hPercent)
{
    addWidget(widget, QRectF(setRealPrecision(xPercent, PRECISION)
        , setRealPrecision(yPercent, PRECISION)
        , setRealPrecision(wPercent, PRECISION)
        , setRealPrecision(hPercent, PRECISION)));
}


QList<QWidget *> SAFigureContainer::getWidgetList() const
{
    QList<QWidget *> res;

    for (auto i = d_ptr->mWidgetPos.begin(); i != d_ptr->mWidgetPos.end(); ++i)
    {
        res.append(i.key());
    }
    return (res);
}


QRectF SAFigureContainer::getWidgetPosPercent(QWidget *w) const
{
    return (d_ptr->mWidgetPos.value(w));
}


/**
 * @brief 设置窗口的百分比
 * @param w
 * @param posPercent
 */
void SAFigureContainer::setWidgetPosPercent(QWidget *w, const QRectF& posPercent)
{
    auto i = d_ptr->mWidgetPos.find(w);

    if (i != d_ptr->mWidgetPos.end()) {
        i.value() = posPercent;
    }
}


/**
 * @brief 设置窗口的百分比
 * @param w
 * @param xPercent
 * @param yPercent
 * @param wPercent
 * @param hPercent
 */
void SAFigureContainer::setWidgetPosPercent(QWidget *w, float xPercent, float yPercent, float wPercent, float hPercent)
{
    setWidgetPosPercent(w, QRectF(setRealPrecision(xPercent, PRECISION)
        , setRealPrecision(yPercent, PRECISION)
        , setRealPrecision(wPercent, PRECISION)
        , setRealPrecision(hPercent, PRECISION)));
}


/**
 * @brief 更新维护的窗口的尺寸
 * @param w 窗口指针
 * @param rect 需要进行设置的尺寸，这个尺寸将是窗口的Geometry，在update过程中，会更新对应的相对位置
 */
void SAFigureContainer::updateWidgetPos(QWidget *w, const QRect& rect)
{
    if (!(d_ptr->mWidgetPos.contains(w))) {
        return;
    }
    QRectF present = calcWidgetPosPercent(this, rect);

    (d_ptr->mWidgetPos)[w] = present;
}


bool SAFigureContainer::isWidgetInContainer(const QWidget *w)
{
    return (d_ptr->mWidgetPos.contains(const_cast<QWidget *>(w)));
}


/**
 * @brief 设置允许在窗口改变的时候重新修改widgetPosPersent,此函数需要和 @sa endResetSubWidget 配套
 */
void SAFigureContainer::beginResetSubWidget()
{
    d_ptr->mIsResetWidgetPos = true;
}


/**
 * @brief 设置不允许在窗口改变的时候重新修改widgetPosPersent,此函数需要和 @sa beginResetSubWidget 配套
 */
void SAFigureContainer::endResetSubWidget()
{
    d_ptr->mIsResetWidgetPos = false;
}


/**
 * @brief 计算size对应SAFigureContainer的位置占比
 * @param c
 * @param size
 * @return 返回rect对应SAFigureContainer的位置占比
 */
QRectF SAFigureContainer::calcWidgetPosPercent(SAFigureContainer *c, const QRect& rect)
{
    QRectF present;
    QSize widgetSize = c->size();

    present.setX(setRealPrecision((qreal)rect.left() / widgetSize.width(), PRECISION));
    present.setY(setRealPrecision((qreal)rect.top() / widgetSize.height(), PRECISION));
    present.setWidth(setRealPrecision((qreal)rect.width()/widgetSize.width(), PRECISION));
    present.setHeight(setRealPrecision((qreal)rect.height()/widgetSize.height(), PRECISION));
    return (present);
}


/**
 * @brief 通过窗口Percent计算窗口尺寸
 * @param c
 * @param percent
 * @return
 */
QRect SAFigureContainer::calcWidgetRectByPercent(SAFigureContainer *c, const QRectF& percent)
{
    QSize widgetSize = c->size();
    QRect newrect;

    newrect.setRect(
        widgetSize.width() * percent.left()
        , widgetSize.height() * percent.top()
        , widgetSize.width() * percent.width()
        , widgetSize.height() * percent.height());
    return (newrect);
}


/**
 * @brief 限制浮点数的位数，避免出现过多小数
 * @param v 需要限制的浮点数
 * @param precision 精度，默认为3位
 * @return 限制后的浮点数值
 */
qreal SAFigureContainer::setRealPrecision(qreal v, int precision)
{
    qreal p = pow(10, precision);

    return (round(v*p)/p);
}


void SAFigureContainer::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QRect subWidgetSize;

    for (auto i = (d_ptr->mWidgetPos.begin()); i != (d_ptr->mWidgetPos.end()); ++i)
    {
        calcWidgetSize(i.value(), subWidgetSize);
        QWidget *w = i.key();
//        qDebug()	<<" == " <<w
//                << "present:"<< i.value()
//                << " size :" <<  event->size()
//                << " old:" << event->oldSize()
//                << " set to :" << subWidgetSize;
        //这里设置尺寸，会导致eventFilter的resizeevent,重新改变了百分比，因此，需要设置忽略百分比改变
        w->setGeometry(subWidgetSize);
    }
    //  QWidget:resizeEvent(event);
}


bool SAFigureContainer::event(QEvent *e)
{
    if (e) {
        if (QEvent::ChildRemoved == e->type()) {
            QChildEvent *ce = static_cast<QChildEvent *>(e);
            QObject *obj = ce->child();
            if ((nullptr != obj) && obj->isWidgetType()) {
                QWidget *w = (QWidget *)obj;
                d_ptr->mWidgetPos.remove(w);
            }
        }
    }
    return (QWidget::event(e));
}


bool SAFigureContainer::eventFilter(QObject *watched, QEvent *event)
{
    if (nullptr == event) {
        return (QWidget::eventFilter(watched, event));
    }
    if (d_ptr->mIsResetWidgetPos) {
        if (QEvent::Resize == event->type()) {
            if (watched && watched->isWidgetType()) {
                QWidget *w = qobject_cast<QWidget *>(watched);
                if (w && isWidgetInContainer(w)) {
                    QResizeEvent *e = static_cast<QResizeEvent *>(event);
                    QRectF& data = (d_ptr->mWidgetPos)[w];
                    data.setWidth(setRealPrecision(e->size().width()/(double)width(), PRECISION));
                    data.setHeight(setRealPrecision(e->size().height()/(double)height(), PRECISION));
                    //qDebug() <<" == " <<watched << " set new:" <<  e->size() << " old" <<e->oldSize() << "present:"<<data;
                }
            }
        }else if (QEvent::Move == event->type()) {
            if (watched && watched->isWidgetType()) {
                QWidget *w = qobject_cast<QWidget *>(watched);
                if (w && isWidgetInContainer(w)) {
                    QMoveEvent *e = static_cast<QMoveEvent *>(event);
                    QRectF& data = (d_ptr->mWidgetPos)[w];
                    data.setX(setRealPrecision(e->pos().x()/(double)width(), PRECISION));
                    data.setY(setRealPrecision(e->pos().y()/(double)height(), PRECISION));
                }
            }
        }
    }
    return (QWidget::eventFilter(watched, event));
}


void SAFigureContainer::calcWidgetSize(const QRectF& present, QRect& newSize)
{
    newSize = calcWidgetRectByPercent(this, present);
}
