#include "SARibbonBar.h"
#include <QPainter>
#include "SARibbonApplicationButton.h"
#include "SARibbonTabBar.h"
#include <QSet>
#include <QStackedWidget>
#include <QVariant>
#include <QLinearGradient>
#include <QDebug>
class ContextCategoryManagerData
{
public:
    SARibbonContextCategory* contextCategory;
    QList<int> tabPageIndex;
    bool operator ==(const SARibbonContextCategory* contextPage)
    {
        return (this->contextCategory == contextPage);
    }
};

class SARibbonBarPrivate
{
public:
    QMargins widgetBord;
    SARibbonBar* MainClass;
    const int titleBarHight;
    const int tabBarHight;
    QBrush ribbonBarBackground;
    QColor titleTextColor;
    QAbstractButton* applitionButton;
    SARibbonTabBar* ribbonTabBar;
    QStackedWidget* stackedContainerWidget;
    QList<ContextCategoryManagerData> currentShowingContextCategory;
    int iconRightBorderPosition;///< 标题栏x值得最小值，在有图标和快捷启动按钮，此值都需要变化
    SARibbonBarPrivate(SARibbonBar* par)
        :titleBarHight(30)
        ,tabBarHight(30)
        ,ribbonBarBackground(QColor(227,227,229))
        ,titleTextColor(Qt::black)
        ,applitionButton(nullptr)
        ,widgetBord(1,1,1,0)
        ,iconRightBorderPosition(1)
    {
        MainClass = par;
        applitionButton = new SARibbonApplicationButton(par);
        applitionButton->setObjectName(QStringLiteral("SARibbonApplicationButton"));
        applitionButton->setGeometry(widgetBord.left(),titleBarHight+widgetBord.top(),56,30);
        par->connect(applitionButton,&QAbstractButton::clicked
                     ,par,&SARibbonBar::applitionButtonClicked);
        //
        ribbonTabBar = new SARibbonTabBar(par);
        ribbonTabBar->setGeometry(applitionButton->geometry().right()
                                  ,titleBarHight+widgetBord.top()
                                  ,par->width(),tabBarHight);
        par->connect(ribbonTabBar,&QTabBar::currentChanged
                     ,par,&SARibbonBar::onCurrentRibbonTabChanged);

        //
        stackedContainerWidget = new QStackedWidget(par);
        stackedContainerWidget->setGeometry(widgetBord.left()
                                            ,ribbonTabBar->geometry().bottom()+1
                                            ,par->width()-widgetBord.left()-widgetBord.right()
                                            ,par->height()-ribbonTabBar->geometry().bottom()-2-widgetBord.bottom());
        }

    void setApplitionButton(QAbstractButton *btn)
    {
        if(applitionButton)
        {
            delete applitionButton;
        }
        applitionButton = btn;
        if(applitionButton)
        {
            applitionButton->move(0,titleBarHight);
            MainClass->connect(applitionButton,&QAbstractButton::clicked
                         ,MainClass,&SARibbonBar::applitionButtonClicked);
        }
    }

    bool isContainContextCategoryInList(SARibbonContextCategory* contextCategory)
    {
        for(int i=0;i<currentShowingContextCategory.size();++i)
        {
            if(currentShowingContextCategory[i] == contextCategory)
            {
                return true;
            }
        }
        return false;
    }
};



SARibbonBar::SARibbonBar(QWidget *parent):QWidget(parent)
  ,m_d(new SARibbonBarPrivate(this))
{
    setFixedHeight(160);
    connect(parent,&QWidget::windowTitleChanged,this,&SARibbonBar::onWindowTitleChanged);
    connect(parent,&QWidget::windowIconChanged,this,&SARibbonBar::onWindowIconChanged);
}

void SARibbonBar::setRibbonBarBackground(const QBrush &brush)
{
    m_d->ribbonBarBackground = brush;
    repaint();
}

QAbstractButton *SARibbonBar::applitionButton()
{
    return m_d->applitionButton;
}

void SARibbonBar::setApplitionButton(QAbstractButton *btn)
{
    m_d->setApplitionButton(btn);
    repaint();
}

SARibbonTabBar *SARibbonBar::ribbonTabBar()
{
    return m_d->ribbonTabBar;
}
///
/// \brief 添加一个标签
/// \param title 标签名字
/// \return 返回一个窗口容器，在Category里可以添加其他控件
/// \see SARibbonCategory
///
SARibbonCategory *SARibbonBar::addCategoryPage(const QString &title)
{
    SARibbonCategory* catagory = new SARibbonCategory(this);
    catagory->setWindowTitle(title);
    int index = m_d->ribbonTabBar->addTab(title);
    m_d->ribbonTabBar->setTabData(index,QVariant((quint64)catagory));
    m_d->stackedContainerWidget->addWidget(catagory);
    return catagory;
}

SARibbonContextCategory *SARibbonBar::addContextCategory(const QString &title, const QColor &color, const QVariant &id)
{
    SARibbonContextCategory* context = new SARibbonContextCategory(this);
    context->setContextTitle(title);
    context->setId(id);
    context->setContextColor(color);
    connect(context,&SARibbonContextCategory::categoryPageAdded
            ,this,&SARibbonBar::onContextsCategoryPageAdded);
    return context;
}
///
/// \brief 显示上下文标签
/// \param context 上下文标签指针
///
void SARibbonBar::showContextCategory(SARibbonContextCategory *context)
{
    if(nullptr == context || m_d->isContainContextCategoryInList(context))
    {
        return;
    }
    ContextCategoryManagerData contextCategoryData;
    contextCategoryData.contextCategory = context;
    for(int i=0;i<context->categoryCount();++i)
    {
        SARibbonCategory* category = context->categoryPage(i);
        int index = m_d->ribbonTabBar->addTab(category->windowTitle());
        contextCategoryData.tabPageIndex.append(index);
        m_d->ribbonTabBar->setTabData(index,QVariant((quint64)category));
    }
    m_d->currentShowingContextCategory.append(contextCategoryData);

    repaint();
}

void SARibbonBar::hideContextCategory(SARibbonContextCategory *context)
{
    for(int i=0;i<m_d->currentShowingContextCategory.size();++i)
    {
        if(m_d->currentShowingContextCategory[i].contextCategory == context)
        {
            const QList<int>& indexs = m_d->currentShowingContextCategory[i].tabPageIndex;
            for(int j=indexs.size()-1;j>=0;--j)
            {
                m_d->ribbonTabBar->removeTab(indexs[j]);
            }
            m_d->currentShowingContextCategory.removeAt(i);
        }
    }
    repaint();
}

void SARibbonBar::setContextCategoryVisible(SARibbonContextCategory *context, bool visible)
{
    if(visible)
    {
        showContextCategory(context);
    }
    else
    {
        hideContextCategory(context);
    }
}



void SARibbonBar::onWindowTitleChanged(const QString &title)
{
    Q_UNUSED(title);
    repaint();
}

void SARibbonBar::onWindowIconChanged(const QIcon &icon)
{
    Q_UNUSED(icon);
    repaint();
}

void SARibbonBar::onCurrentRibbonTabChanged(int index)
{
    QVariant var = m_d->ribbonTabBar->tabData(index);
    SARibbonCategory* category = nullptr;
    if(var.isValid())
    {
        quint64 p = var.value<quint64>();
        category = (SARibbonCategory*)p;
    }
    m_d->stackedContainerWidget->setCurrentWidget(category);
    if(!m_d->currentShowingContextCategory.isEmpty())
    {
        repaint();
    }
    emit currentRibbonTabChanged(index);
}



void SARibbonBar::onContextsCategoryPageAdded(SARibbonCategory *category)
{
    Q_ASSERT_X(category != nullptr,"onContextsCategoryPageAdded","add nullptr page");
    m_d->stackedContainerWidget->addWidget(category);
}

void SARibbonBar::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    //!
    paintBackground(p);

    //! 显示上下文标签
    p.save();
    QList<ContextCategoryManagerData> contextCategoryDataList = m_d->currentShowingContextCategory;
    bool isCurrentSelectContextCategoryPage = false;
    QPoint contextCategoryRegion = QPoint(width(),-1);
    for(int i=0;i<contextCategoryDataList.size();++i)
    {
        QRect contextTitleRect;
        QList<int> indexs = contextCategoryDataList[i].tabPageIndex;
        QColor clr = contextCategoryDataList[i].contextCategory->contextColor();
        if(!indexs.isEmpty())
        {
            contextTitleRect = m_d->ribbonTabBar->tabRect(indexs.first());
            QRect endRect = m_d->ribbonTabBar->tabRect(indexs.last());
            contextTitleRect.setRight(endRect.right());
            contextTitleRect.translate(m_d->ribbonTabBar->x(),m_d->ribbonTabBar->y());
            contextTitleRect.setHeight(m_d->ribbonTabBar->height());
            contextTitleRect-=m_d->ribbonTabBar->tabMargin();
            //把区域顶部扩展到窗口顶部
            contextTitleRect.setTop(m_d->widgetBord.top());
            //绘制
            paintContextCategoryTab(p
                                    ,contextCategoryDataList[i].contextCategory->contextTitle()
                                    ,contextTitleRect
                                    ,clr);
            //更新上下文标签的范围，用于控制标题栏的显示
            if(contextTitleRect.left() < contextCategoryRegion.x())
            {
                contextCategoryRegion.setX(contextTitleRect.left());
            }
            if(contextTitleRect.right() > contextCategoryRegion.y())
            {
                contextCategoryRegion.setY(contextTitleRect.right());
            }
        }
        isCurrentSelectContextCategoryPage = indexs.contains(m_d->ribbonTabBar->currentIndex());
        if(isCurrentSelectContextCategoryPage)
        {
            QPen pen;
            pen.setColor(clr);
            pen.setWidth(1);
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawRect(m_d->stackedContainerWidget->geometry());
            isCurrentSelectContextCategoryPage = false;
        }
    }
    p.restore();
    //! 显示标题等
    QWidget* parWindow = parentWidget();
    if(parWindow)
    {
        paintWindowTitle(p,parWindow->windowTitle(),contextCategoryRegion);
        paintWindowIcon(p,parWindow->windowIcon());
    }
    QWidget::paintEvent(e);
}
void SARibbonBar::paintContextCategoryTab(QPainter &painter, const QString &title, QRect contextRect, const QColor &color)
{
    //绘制上下文标签
    //首先有5像素的实体
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRect(QRect(contextRect.x(),m_d->widgetBord.top(),contextRect.width(),5));
    int yStart = contextRect.y()+5;

    //剩下的是渐变颜色
    contextRect -= QMargins(0,5,0,0);
    QColor gColor = color;
    QLinearGradient lineGradient;
    lineGradient.setStart(contextRect.x(),yStart);
    lineGradient.setFinalStop(contextRect.x(),contextRect.bottom());
    gColor.setAlpha(150);
    lineGradient.setColorAt(0,gColor);
    gColor.setAlpha(0);
    lineGradient.setColorAt(0.9,gColor);
    painter.fillRect(contextRect,lineGradient);
    //绘制标题
    //TODO 判断如果颜色很接近，需要替换为白色
    contextRect.setBottom(m_d->ribbonTabBar->geometry().top());
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(contextRect,Qt::AlignCenter,title);
}
void SARibbonBar::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    int x = m_d->widgetBord.left();
    if(m_d->applitionButton)
    {
        m_d->applitionButton->move(m_d->widgetBord.left(),m_d->titleBarHight+m_d->widgetBord.top());
        x = m_d->applitionButton->geometry().right();
        m_d->ribbonTabBar->setGeometry(x
                                       ,m_d->titleBarHight+m_d->widgetBord.top()
                                       ,width()-x-m_d->widgetBord.right()
                                       ,m_d->tabBarHight);
    }
    else
    {
        m_d->ribbonTabBar->setGeometry(x,m_d->titleBarHight+m_d->widgetBord.top()
                                ,width()-x-m_d->widgetBord.right()
                                       ,m_d->tabBarHight);
    }
    m_d->stackedContainerWidget->setGeometry(m_d->widgetBord.left()
                                             ,m_d->ribbonTabBar->geometry().bottom()+1
                                            ,width()-m_d->widgetBord.left()-m_d->widgetBord.right()
                                             ,height()-m_d->ribbonTabBar->geometry().bottom()-2-m_d->widgetBord.bottom());
}

void SARibbonBar::paintBackground(QPainter &painter)
{
    painter.save();
    painter.setBrush(m_d->ribbonBarBackground);
    painter.drawRect(rect());
    painter.restore();
}

///
/// \brief 绘制标题栏
/// \param painter
/// \param title 标题
/// \param contextCategoryRegion 当前显示的上下文标签的范围，上下文标签是可以遮挡标题栏的，因此需要知道上下文标签的范围
/// x表示左边界，y表示右边界
///
void SARibbonBar::paintWindowTitle(QPainter &painter, const QString &title, const QPoint &contextCategoryRegion)
{
    painter.save();
    painter.setPen(m_d->titleTextColor);
    if(contextCategoryRegion.y() < 0)
    {
        int x = m_d->iconRightBorderPosition;
        painter.drawText(x
                         ,m_d->widgetBord.top()
                         ,width()- m_d->iconRightBorderPosition - m_d->widgetBord.right()
                         ,m_d->titleBarHight,Qt::AlignCenter,title);
    }
    else
    {
        if(width() - contextCategoryRegion.y() > contextCategoryRegion.x()-m_d->iconRightBorderPosition)
        {
            //说明左边的区域大一点，标题显示在坐标
            int x = contextCategoryRegion.y();
            painter.drawText(x
                             ,m_d->widgetBord.top()
                             ,width()-x
                             ,m_d->titleBarHight,Qt::AlignCenter,title);
        }
        else
        {
            //说明右边的大一点
            int x = m_d->iconRightBorderPosition;
            painter.drawText(x
                             ,m_d->widgetBord.top()
                             ,contextCategoryRegion.x()-m_d->iconRightBorderPosition
                             ,m_d->titleBarHight,Qt::AlignCenter,title);
        }
    }
    painter.restore();
}

void SARibbonBar::paintWindowIcon(QPainter &painter, const QIcon &icon)
{
    painter.save();
    if(!icon.isNull())
    {
        int iconMinSize = m_d->titleBarHight;
        icon.paint(&painter,m_d->widgetBord.left(),m_d->widgetBord.top()
                   ,iconMinSize,iconMinSize);
        m_d->iconRightBorderPosition = m_d->widgetBord.left()+iconMinSize;
    }
    else
    {
        m_d->iconRightBorderPosition = m_d->widgetBord.left();
    }
    painter.restore();
}


