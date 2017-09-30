#include "SARibbonCategory.h"
#include <QList>
#include <QResizeEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>
class SARibbonCategoryPrivate
{
public:
    QList<SARibbonPannel*> pannelLists;
    QList<int> separatorX;
};

SARibbonCategory::SARibbonCategory(QWidget *parent)
    :QWidget(parent)
    ,m_d(new SARibbonCategoryPrivate)
{
    setFixedHeight(98);
    setAutoFillBackground(true);
    setBackgroundBrush(Qt::white);
}

SARibbonCategory::~SARibbonCategory()
{
    delete m_d;
}

///
/// \brief 添加一个面板
/// \param title 明白名称
/// \return
///
SARibbonPannel *SARibbonCategory::addPannel(const QString &title)
{
    SARibbonPannel* pannel = new SARibbonPannel(this);
    pannel->setWindowTitle(title);
    m_d->pannelLists.append(pannel);
    return pannel;
}
///
/// \brief SARibbonCategory::setBackgroundBrush
/// \param brush
///
void SARibbonCategory::setBackgroundBrush(const QBrush &brush)
{
    QPalette p = palette();
    p.setBrush(QPalette::Background,brush);
    setPalette(p);
}

void SARibbonCategory::resizeEvent(QResizeEvent *event)
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
    QWidget::resizeEvent(event);
}

void SARibbonCategory::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPixmap pixmap = drawSeparatorPixmap();
    for(int i=0;i<m_d->separatorX.size();++i)
    {
        p.drawPixmap(m_d->separatorX[i],0,pixmap);
    }
    QWidget::paintEvent(event);
}

QPixmap SARibbonCategory::drawSeparatorPixmap() const
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
