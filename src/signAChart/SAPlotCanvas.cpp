#include "SAPlotCanvas.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class SAPlotCanvasPrivate
{
public:
    SA_IMPL_PUBLIC(SAPlotCanvas)
    QBrush canvasBackBrush;
    QColor canvasBorderColor;
    SAPlotCanvasPrivate(SAPlotCanvas *p) :
        q_ptr(p),
        canvasBackBrush(Qt::white),
        canvasBorderColor(Qt::black)
    {
    }


    ~SAPlotCanvasPrivate()
    {
    }


    void setqss()
    {
        QString qss = QString("SAPlotCanvas"
            "{"
            "   background-color: %1;"
            "   border: %2px solid %3;"
            "}").arg(this->canvasBackBrush.color().name(QColor::HexArgb))
            .arg(q_ptr->lineWidth())
            .arg(this->canvasBorderColor.name(QColor::HexArgb));

        q_ptr->setStyleSheet(qss);
    }
};

SAPlotCanvas::SAPlotCanvas(QwtPlot *p) : QwtPlotCanvas(p)
    , d_ptr(new SAPlotCanvasPrivate(this))
{
}


SAPlotCanvas::~SAPlotCanvas()
{
}


/**
 * @brief 获取背景画刷
 * @return
 */
QBrush SAPlotCanvas::getCanvaBackBrush() const
{
    if (testAttribute(Qt::WA_StyledBackground)) {
        return (d_ptr->canvasBackBrush);
    }

    return (palette().brush(QPalette::Normal, QPalette::Window));
}


/**
 * @brief 设置背景画刷
 * @param b 画刷
 */
void SAPlotCanvas::setCanvaBackBrush(const QBrush& b)
{
    if (testAttribute(Qt::WA_StyledBackground)) {
        d_ptr->canvasBackBrush = b;
        d_ptr->setqss();
    }else {
        QPalette pal = palette();
        pal.setBrush(QPalette::Window, b);

        setPalette(pal);
    }
}


/**
 * @brief 设置边框颜色
 * @return
 */
QColor SAPlotCanvas::getCanvasBorderColor() const
{
    return (d_ptr->canvasBorderColor);
}


/**
 * @brief 获取边框颜色
 * @param c
 */
void SAPlotCanvas::setCanvasBorderColor(const QColor& c)
{
    d_ptr->canvasBorderColor = c;
    d_ptr->setqss();
}


/**
 * @brief 设置qss，在Qt::WA_StyledBackground生效时
 */
void SAPlotCanvas::useQss()
{
    d_ptr->setqss();
}


void SAPlotCanvas::paintEvent(QPaintEvent *e)
{
//    QPainter p(this);

//    if (testAttribute(Qt::WA_StyledBackground)) {
//        setAttribute(Qt::WA_StyledBackground, false);
//    }
    QwtPlotCanvas::paintEvent(e);
}
