#include "SARibbonCategory.h"
#include <QList>
#include <QResizeEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "SARibbonToolButton.h"
#include <QHBoxLayout>
#include "SARibbonSeparatorWidget.h"
#include "SARibbonCategoryLayout.h"


class SARibbonCategoryPrivate
{
public:
    SARibbonCategory* parent;
    QList<SARibbonPannel*> pannelLists;
};

SARibbonCategory::SARibbonCategory(QWidget *parent)
    :QWidget(parent)
    ,m_proxy(new SARibbonCategoryProxy(this))
{
    new SARibbonCategoryLayout(this);
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
    ribbonCategory()->layout()->addWidget(pannel);
    SARibbonSeparatorWidget* seprator = new SARibbonSeparatorWidget(ribbonCategory()->height(),ribbonCategory());
    ribbonCategory()->layout()->addWidget(seprator);
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
    return;
}

void SARibbonCategoryProxy::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    return;
}



SARibbonCategory *SARibbonCategoryProxy::ribbonCategory()
{
    return m_d->parent;
}
