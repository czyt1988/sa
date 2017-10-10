#include "SARibbonCategory.h"
#include <QList>
#include <QResizeEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "SARibbonToolButton.h"
QPixmap* SARibbonCategoryProxy::category_separator_pixmap = nullptr;

class ReduceActionInfo
{
public:
    QPoint showStartPoint;
    SARibbonPannel* reduceModeShowPannel;
    SARibbonPannel* realShowPannel;
    ReduceActionInfo()
        :showStartPoint(0,0)
        ,reduceModeShowPannel(nullptr)
        ,realShowPannel(nullptr)
    {

    }

    void release()
    {
        realShowPannel = nullptr;
        if(reduceModeShowPannel)
        {
            delete reduceModeShowPannel;
            reduceModeShowPannel = nullptr;
        }
    }
};

class SARibbonCategoryPrivate
{
public:
    SARibbonCategory* parent;
    QList<SARibbonPannel*> pannelLists;
    QList<int> separatorX;
    QMap<SARibbonPannel*,ReduceActionInfo> pannelReduceInfo;
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
    const int pannelCount = m_d->pannelLists.size();
    int nextShowPannelPos = 0;
    const QSize categorySize = ribbonCategory()->size();
    for(int i=0;i<pannelCount;++i)
    {
        SARibbonPannel* pannel = m_d->pannelLists[i];
        if((nextShowPannelPos+pannel->width()) < categorySize.width())
        {
            //此时能显示全一个pannel
            pannel->setReduce(false);
            nextShowPannelPos = showNormalPannel(pannel,i,nextShowPannelPos);
            if(m_d->pannelReduceInfo.contains(pannel))
            {
                //把reduce信息删除
                m_d->pannelReduceInfo[pannel].release();
                m_d->pannelReduceInfo.remove(pannel);
            }
        }
        else
        {
            pannel->setReduce(true);
            //此时无法显示全一个pannel
            if(!m_d->pannelReduceInfo.contains(pannel))
            {
                //建立一个最小显示方案
                buildReduceModePannel(pannel,nextShowPannelPos);
            }
            //已经有未显示全时的最小显示方案
            ReduceActionInfo reduceInfo = m_d->pannelReduceInfo.value(pannel);
            nextShowPannelPos = showNormalPannel(reduceInfo.reduceModeShowPannel,i,nextShowPannelPos);
            pannel->setVisible(false);
        }
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

int SARibbonCategoryProxy::showNormalPannel(SARibbonPannel *pannel,int index,int x)
{
    //已经有未显示全时的最小显示方案
    pannel->move(x,0);
    if(!(pannel->isVisible()))
    {
        pannel->setVisible(true);
    }
    //更新分割线坐标
    int pannelEndPos = pannel->geometry().right();
    ++pannelEndPos;
    if(m_d->separatorX.size() <= index)
    {
        m_d->separatorX.append(pannelEndPos);
    }
    else
    {
        m_d->separatorX[index] = pannelEndPos;
    }
    return pannelEndPos;
}

void SARibbonCategoryProxy::buildReduceModePannel(SARibbonPannel* pannel, int x)
{
    SARibbonCategory* categoryPage = ribbonCategory();
    ReduceActionInfo info;
    info.realShowPannel = pannel;
    SARibbonPannel* reducePannel = new SARibbonPannel(categoryPage);
    reducePannel->setWindowTitle(pannel->windowTitle());
    reducePannel->setWindowIcon(pannel->windowIcon());
    SARibbonToolButton* btn = reducePannel->addLargeToolButton(pannel->windowTitle(),pannel->windowIcon()
                                    ,QToolButton::InstantPopup);
    info.reduceModeShowPannel = reducePannel;
    //计算弹出窗口的位置


    categoryPage->connect(btn,&SARibbonToolButton::clicked
                              ,categoryPage,[categoryPage,info](bool on){
        Q_UNUSED(on);
        int x = info.reduceModeShowPannel->geometry().x();
        QPoint pos = SARibbonCategoryProxy::calcPopupPannelPosition(categoryPage,info.realShowPannel,x);
        info.realShowPannel->move(pos);
        info.realShowPannel->setFocus();
        info.realShowPannel->show();

    });
    m_d->pannelReduceInfo[pannel] = info;
}

QPoint SARibbonCategoryProxy::calcPopupPannelPosition(SARibbonCategory* category,SARibbonPannel* pannel,int x)
{
    QPoint absPosition = category->mapToGlobal(QPoint(x,category->height()));
    QRect r = QApplication::desktop()->availableGeometry(category);
    //qDebug() << "x"<<x<<"desktop:" << r << " absPosition" << absPosition;
    if((absPosition.x() + pannel->width()) < r.width())
    {
        return absPosition;
    }
    //x - ( popupPannelwidth - ( r.right() - x ));
    return QPoint(r.width()-  pannel->width(),absPosition.y());

}
