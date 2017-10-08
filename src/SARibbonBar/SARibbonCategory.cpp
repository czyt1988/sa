#include "SARibbonCategory.h"
#include <QList>
#include <QResizeEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>
QPixmap* SARibbonCategoryProxy::category_separator_pixmap = nullptr;
class SARibbonCategoryPrivate
{
public:
    SARibbonCategory* parent;
    QList<SARibbonPannel*> pannelLists;
    QList<int> separatorX;
};

SARibbonCategory::SARibbonCategory(QWidget *parent)
    :QWidget(parent)
    ,m_proxy(new SARibbonCategoryProxy(this))
{
    setFixedHeight(98);
    setAutoFillBackground(true);
    setBackgroundBrush(Qt::white);
}

SARibbonCategory::~SARibbonCategory()
{
}

///
/// \brief 添加一个面板
/// \param title 明白名称
/// \return
///
SARibbonPannel *SARibbonCategory::addPannel(const QString &title)
{
    return proxy()->addPannel(title);
}
///
/// \brief SARibbonCategory::setBackgroundBrush
/// \param brush
///
void SARibbonCategory::setBackgroundBrush(const QBrush &brush)
{
    proxy()->setBackgroundBrush(brush);
}

SARibbonCategoryProxy *SARibbonCategory::proxy()
{
    return m_proxy.data();
}

void SARibbonCategory::setProxy(SARibbonCategoryProxy *proxy)
{
    m_proxy.reset(proxy);
}

void SARibbonCategory::resizeEvent(QResizeEvent *event)
{
    proxy()->resizeEvent(event);
    QWidget::resizeEvent(event);
}

void SARibbonCategory::paintEvent(QPaintEvent *event)
{
    proxy()->paintEvent(event);
    QWidget::paintEvent(event);
}


//////////////////////////////////////////////////////////////////

SARibbonCategoryProxy::SARibbonCategoryProxy(SARibbonCategory *parent)
    :m_d(new SARibbonCategoryPrivate)
{
    m_d->parent = parent;
}

SARibbonCategoryProxy::~SARibbonCategoryProxy()
{
    delete m_d;
}

SARibbonPannel *SARibbonCategoryProxy::addPannel(const QString &title)
{
    SARibbonPannel* pannel = new SARibbonPannel(ribbonCategory());
    pannel->setWindowTitle(title);
    m_d->pannelLists.append(pannel);
    return pannel;
}

void SARibbonCategoryProxy::setBackgroundBrush(const QBrush &brush)
{
    QPalette p = ribbonCategory()->palette();
    p.setBrush(QPalette::Background,brush);
    ribbonCategory()->setPalette(p);
}


void SARibbonCategoryProxy::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    const int size = m_d->pannelLists.size();
    int lastPos = 0;
    for(int i=0;i<size;++i)
    {
        if(0 != i)
        {
            //绘制分割线
            if(m_d->separatorX.size() < i)
            {
                m_d->separatorX.append(lastPos-1);
            }
            else
            {
                m_d->separatorX[i-1] = lastPos-1;
            }
        }
        SARibbonPannel* pannel = m_d->pannelLists[i];
        pannel->move(lastPos,0);
        lastPos = pannel->width() + 1;
    }
}

void SARibbonCategoryProxy::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(ribbonCategory());
    if(nullptr == category_separator_pixmap)
    {
        category_separator_pixmap = new QPixmap(drawSeparatorPixmap());
    }
    for(int i=0;i<m_d->separatorX.size();++i)
    {
        p.drawPixmap(m_d->separatorX[i],0,*category_separator_pixmap);
    }

}

QPixmap SARibbonCategoryProxy::drawSeparatorPixmap() const
{
    QPixmap pix(1,98);
    QPainter p(&pix);
    QLinearGradient gdl(0,98,0,0);
    gdl.setColorAt(0,QColor(178,183,189));
    gdl.setColorAt(0.5,QColor(196,201,207));
    gdl.setColorAt(1,QColor(253,253,253));
    QBrush b(gdl);
    p.setBrush(b);
    QPen pen;
    pen.setBrush(b);
    p.setPen(pen);
    p.drawLine(0,97,0,0);
    return pix;
}

SARibbonCategory *SARibbonCategoryProxy::ribbonCategory()
{
    return m_d->parent;
}
