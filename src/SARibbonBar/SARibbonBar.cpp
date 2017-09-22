#include "SARibbonBar.h"
#include <QPainter>
#include "SARibbonApplicationButton.h"
#include "SARibbonTabBar.h"
class SARibbonBarPrivate
{
public:
    SARibbonBar* MainClass;
    const int titleBarHight;
    const int tabBarHight;
    QBrush ribbonBarBackground;
    QColor titleTextColor;
    QAbstractButton* applitionButton;
    SARibbonTabBar* ribbonTabBar;
    SARibbonBarPrivate(SARibbonBar* par)
        :titleBarHight(30)
        ,tabBarHight(30)
        ,ribbonBarBackground(QColor(227,227,229))
        ,titleTextColor(Qt::black)
    {
        MainClass = par;
        applitionButton = new SARibbonApplicationButton(par);
        applitionButton->setObjectName(QStringLiteral("SARibbonApplicationButton"));
        applitionButton->setGeometry(0,titleBarHight,56,30);
        par->connect(applitionButton,&QAbstractButton::clicked
                     ,par,&SARibbonBar::applitionButtonClicked);
        //
        ribbonTabBar = new SARibbonTabBar(par);
        ribbonTabBar->setGeometry(applitionButton->rect().right(),titleBarHight
                                  ,par->width(),tabBarHight);
        par->connect(ribbonTabBar,&QTabBar::currentChanged
                     ,par,&SARibbonBar::currentRibbonTabChanged);
        par->connect(ribbonTabBar,&QTabBar::tabBarClicked
                     ,par,&SARibbonBar::ribbonTabBarClicked);
        par->connect(ribbonTabBar,&QTabBar::tabBarDoubleClicked
                     ,par,&SARibbonBar::ribbonTabBarDoubleClicked);
        par->connect(ribbonTabBar,&QTabBar::tabCloseRequested
                     ,par,&SARibbonBar::ribbonTabCloseRequested);
        par->connect(ribbonTabBar,&QTabBar::tabMoved
                     ,par,&SARibbonBar::ribbonTabMoved);
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
};



SARibbonBar::SARibbonBar(QWidget *parent):QWidget(parent)
  ,m_d(new SARibbonBarPrivate(this))
{
    setFixedHeight(160);
    connect(parent,&QWidget::windowTitleChanged,this,&SARibbonBar::onWindowTitleChanged);
    connect(parent,&QWidget::windowIconChanged,this,&SARibbonBar::onWindowIconChanged);
    m_d->ribbonTabBar->addTab("1");
    m_d->ribbonTabBar->addTab("2");
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
    QWidget::paintEvent(e);
}

void SARibbonBar::resizeEvent(QResizeEvent *e)
{
    int x = 2;
    if(m_d->applitionButton)
    {
        m_d->applitionButton->move(2,m_d->titleBarHight);
        x = m_d->applitionButton->rect().right();
        m_d->ribbonTabBar->setGeometry(x,m_d->titleBarHight
                                ,width()-x,m_d->tabBarHight);
    }
    else
    {
        m_d->ribbonTabBar->setGeometry(x,m_d->titleBarHight
                                ,width()-x,m_d->tabBarHight);
    }
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
