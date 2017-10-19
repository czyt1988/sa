#include "SARibbonGallery.h"
#include "SARibbonControlButton.h"
#include <QIcon>
#define ICON_ARROW_UP QIcon(":/image/resource/ArrowUp.png")
#define ICON_ARROW_DOWN QIcon(":/image/resource/ArrowDown.png")
#define ICON_ARROW_MORE QIcon(":/image/resource/ArrowMore.png")
class SARibbonGalleryPrivate
{
public:
    SARibbonControlButton* buttonUp;
    SARibbonControlButton* buttonDown;
    SARibbonControlButton* buttonMore;
    SARibbonGallery* Parent;
    SARibbonGalleryPrivate():Parent(nullptr)
    {
    }

    void init(SARibbonGallery* parent)
    {
        buttonUp = new SARibbonControlButton(parent);
        buttonDown = new SARibbonControlButton(parent);
        buttonMore = new SARibbonControlButton(parent);
        buttonUp->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonDown->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonMore->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonUp->setFixedSize(15,20);
        buttonDown->setFixedSize(15,20);
        buttonMore->setFixedSize(15,20);
        buttonUp->setIcon(ICON_ARROW_UP);
        buttonDown->setIcon(ICON_ARROW_DOWN);
        buttonMore->setIcon(ICON_ARROW_MORE);
        Parent = parent;
    }

    bool isValid() const
    {
        return Parent != nullptr;
    }
};

SARibbonGallery::SARibbonGallery(QWidget *parent):QFrame(parent)
  ,m_d(new SARibbonGalleryPrivate)
{
    m_d->init(this);
    setFrameShape(QFrame::Box);
    //setFrameShape(QFrame::Panel);
    setFixedHeight(60);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setMinimumWidth(88);
}

SARibbonGallery::~SARibbonGallery()
{
    delete m_d;
}

QSize SARibbonGallery::sizeHint() const
{
    return QSize(232,60);
}

QSize SARibbonGallery::minimumSizeHint() const
{
    return QSize(88,60);
}

void SARibbonGallery::addGalleryGroup(SARibbonGalleryGroup *group)
{

}

void SARibbonGallery::onPageDown()
{

}

void SARibbonGallery::onPageUp()
{

}

void SARibbonGallery::onShowMoreDetail()
{

}

void SARibbonGallery::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if(m_d->isValid())
    {
        const QRect r = rect();
        m_d->buttonUp->move(r.width() - m_d->buttonUp->width(),0);
        m_d->buttonDown->move(r.width() - m_d->buttonDown->width(),m_d->buttonUp->height());
        m_d->buttonMore->move(r.width() - m_d->buttonMore->width(),m_d->buttonDown->geometry().bottom());
    }
}

void SARibbonGallery::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

}
