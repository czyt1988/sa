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

    SARibbonBarPrivate(SARibbonBar* par)
        :titleBarHight(30)
        ,tabBarHight(30)
        ,ribbonBarBackground(QColor(227,227,229))
        ,titleTextColor(Qt::black)
        ,widgetBord(1,1,1,0)
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
        par->connect(ribbonTabBar,&QTabBar::tabBarClicked
                     ,par,&SARibbonBar::onRibbonTabBarClicked);
        par->connect(ribbonTabBar,&QTabBar::tabCloseRequested
                     ,par,&SARibbonBar::onRibbonTabCloseRequested);
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

void SARibbonBar::onRibbonTabBarClicked(int index)
{

}

void SARibbonBar::onRibbonTabCloseRequested(int index)
{

}

void SARibbonBar::onContextsCategoryPageAdded(SARibbonCategory *category)
{
    Q_ASSERT_X(category != nullptr,"onContextsCategoryPageAdded","add nullptr page");
    m_d->stackedContainerWidget->addWidget(category);
}

void SARibbonBar::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    paintBackground(p);
    QWidget* parWindow = parentWidget();
    if(parWindow)
    {
        paintWindowTitle(p,parWindow->windowTitle());
        paintWindowIcon(p,parWindow->windowIcon());
    }
    //显示上下文标签
    p.save();
    QList<ContextCategoryManagerData> contextCategoryDataList = m_d->currentShowingContextCategory;
    bool isCurrentSelectContextCategoryPage = false;
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
            //绘制上下文标签
            //首先有5像素的实体
            p.setPen(Qt::NoPen);
            p.setBrush(clr);
            p.drawRect(QRect(contextTitleRect.x(),m_d->widgetBord.top(),contextTitleRect.width(),5));
            int yStart = contextTitleRect.y()+5;
            //剩下的是渐变颜色
            QColor gColor = clr;
            contextTitleRect -= QMargins(0,5,0,0);
            QLinearGradient lineGradient;
            lineGradient.setStart(contextTitleRect.x(),yStart);
            lineGradient.setFinalStop(contextTitleRect.x(),contextTitleRect.bottom());
            gColor.setAlpha(150);
            lineGradient.setColorAt(0,gColor);
            gColor.setAlpha(0);
            lineGradient.setColorAt(0.9,gColor);
            p.fillRect(contextTitleRect,lineGradient);
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
    QWidget::paintEvent(e);
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


void SARibbonBar::paintWindowTitle(QPainter &painter, const QString &title)
{
    painter.save();
    painter.setPen(m_d->titleTextColor);
    painter.drawText(0,0,width(),m_d->titleBarHight,Qt::AlignCenter,title);
    painter.restore();
}

void SARibbonBar::paintWindowIcon(QPainter &painter, const QIcon &icon)
{
    painter.save();
    if(!icon.isNull())
    {
        icon.paint(&painter,0,0,m_d->titleBarHight,m_d->titleBarHight);
    }
    painter.restore();
}
