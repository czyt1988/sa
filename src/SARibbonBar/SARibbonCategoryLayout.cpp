#include "SARibbonCategoryLayout.h"
#include <QLayoutItem>
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QApplication>
#include <QDesktopWidget>
class SARibbonReduceActionInfo
{
public:
    QPoint showStartPoint;
    SARibbonPannel* reduceModeShowPannel;
    SARibbonPannel* realShowPannel;
    SARibbonReduceActionInfo()
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

SARibbonCategoryLayout::SARibbonCategoryLayout(SARibbonCategory *parent):QLayout(parent)
{
    setContentsMargins(2,2,2,2);
}

SARibbonCategoryLayout::SARibbonCategoryLayout():QLayout()
{
    
}

SARibbonCategoryLayout::~SARibbonCategoryLayout()
{
    QLayoutItem *item = Q_NULLPTR;
    while ((item = takeAt(0)))
    {
          delete item;
    }
}

SARibbonCategory *SARibbonCategoryLayout::ribbonCategory()
{
    return qobject_cast<SARibbonCategory*>(parent());
}

void SARibbonCategoryLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

QLayoutItem *SARibbonCategoryLayout::itemAt(int index) const
{
    return itemList.value(index,Q_NULLPTR);
}

QLayoutItem *SARibbonCategoryLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
      return itemList.takeAt(index);
    else
      return Q_NULLPTR;
}

int SARibbonCategoryLayout::count() const
{
    return itemList.size();
}


QSize SARibbonCategoryLayout::sizeHint() const
{
    QSize size;
    QLayoutItem *item;
    for(int i=0;i<itemList.size();++i)
    {
        size = size.expandedTo(itemList[i]->minimumSize()); 
    }
    
    size += QSize(2*margin(), 2*margin());
    return size;
}

QSize SARibbonCategoryLayout::minimumSize()
{
    return QSize(50,10);
}

Qt::Orientations SARibbonCategoryLayout::expandingDirections() const
{
    return Qt::Horizontal;
}

void SARibbonCategoryLayout::layoutNormalPannel(SARibbonPannel *pannel, int x, int y, int w, int h)
{
    pannel->setGeometry(x,y,w,h);
    if(!(pannel->isVisible()))
    {
        pannel->setVisible(true);
    }
}

int SARibbonCategoryLayout::buildReduceModePannel(SARibbonPannel *realPannel,int x,int y)
{
    SARibbonCategory* categoryPage = ribbonCategory();
    SARibbonReduceActionInfo info;
    info.realShowPannel = realPannel;
    SARibbonPannel* reducePannel = new SARibbonPannel(categoryPage);
    reducePannel->setWindowTitle(realPannel->windowTitle());
    reducePannel->setWindowIcon(realPannel->windowIcon());
    SARibbonToolButton* btn = reducePannel->addLargeToolButton(realPannel->windowTitle(),realPannel->windowIcon()
                                    ,QToolButton::InstantPopup);
    info.reduceModeShowPannel = reducePannel;
    reducePannel->move(x,y);
    connect(btn,&SARibbonToolButton::clicked
                                  ,this,[categoryPage,info](bool on){
        Q_UNUSED(on);
        int pannelX = info.reduceModeShowPannel->geometry().x();
        QPoint pos = SARibbonCategoryLayout::calcPopupPannelPosition(categoryPage,info.realShowPannel,pannelX);
        info.realShowPannel->move(pos);
        info.realShowPannel->setFocus();
        info.realShowPannel->show();
    });
    m_pannelReduceInfo[realPannel] = info;
    return reducePannel->geometry().right();
}

QPoint SARibbonCategoryLayout::calcPopupPannelPosition(SARibbonCategory *category, SARibbonPannel *pannel, int x)
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

void SARibbonCategoryLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    //
    int marginLeft,marginRight,marginTop,marginBottom;
    getContentsMargins(&marginLeft,&marginTop,&marginRight,&marginBottom);
    int x = marginLeft;
    int y = marginTop;
    QLayoutItem *item = Q_NULLPTR;
    for(int i=0;i<itemList.size();++i)
    {
        item = itemList[i];
        QWidget *wid = item->widget();
        QSize widSize = wid->sizeHint();
        int nextX = x + widSize.width();
        SARibbonPannel* pannel = qobject_cast<SARibbonPannel*>(wid);
        if(nextX > rect.right())
        {
            //说明超出边界
            if(pannel)
            {//此时无法显示全一个pannel
                if(!m_pannelReduceInfo.contains(pannel))
                {
                    //建立一个最小显示方案
                    buildReduceModePannel(pannel,x,y);
                }
                pannel->setReduce(true);
                pannel->setVisible(false);
                SARibbonReduceActionInfo reduceInfo = m_pannelReduceInfo.value(pannel);
                QSize reducePannelSize = reduceInfo.reduceModeShowPannel->sizeHint();
                layoutNormalPannel(reduceInfo.reduceModeShowPannel
                                                       ,x,y
                                                       ,reducePannelSize.width()
                                                       ,reducePannelSize.height());
                x = x + reduceInfo.reduceModeShowPannel->width();
            }

        }
        else
        {
            //此时能显示全一个pannel
            if(pannel)
            {
                pannel->setReduce(false);
                layoutNormalPannel(pannel,x,y,widSize.width(),widSize.height());
                if(m_pannelReduceInfo.contains(pannel))
                {
                    //把reduce信息删除
                    m_pannelReduceInfo[pannel].release();
                    m_pannelReduceInfo.remove(pannel);
                }
            }
            else
            {
                wid->setGeometry(x,y,widSize.width(),widSize.height());
            }
            x = nextX;
        }

    }
}
