#include "SARibbonGalleryGroup.h"

SARibbonGalleryGroup::SARibbonGalleryGroup(QWidget *w):QWidget(w)
  ,m_enableTitle(true)
  ,m_itemSize(72,56)
{

}

SARibbonGalleryGroup::~SARibbonGalleryGroup()
{
    while(!m_items.isEmpty())
    {
        delete m_items.takeFirst();
    }
}

void SARibbonGalleryGroup::append(SARibbonGalleryItem *item)
{
    item->m_group = this;
    m_items.append(item);
}

SARibbonGalleryItem *SARibbonGalleryGroup::at(int index)
{
    return m_items.at(index);
}

int SARibbonGalleryGroup::size() const
{
    return m_items.size();
}

SARibbonGalleryItem *SARibbonGalleryGroup::takeAt(int index)
{
    SARibbonGalleryItem* item = m_items.takeAt(index);
    item->m_group = nullptr;
    return item;
}

QList<SARibbonGalleryItem *> &SARibbonGalleryGroup::itemList()
{
    return m_items;
}

const QList<SARibbonGalleryItem *> &SARibbonGalleryGroup::itemList() const
{
    return m_items;
}

QSize SARibbonGalleryGroup::sizeHint() const
{
    QSize size = minimumSizeHint();
    if(m_enableTitle)
    {
        if(0 == m_items.size())
        {
            //只有标题栏显示
            size.setHeight(qMax(size.height(),fontMetrics().height()));
        }
    }

}

void SARibbonGalleryGroup::setItemSize(const QSize &size)
{

}

void SARibbonGalleryGroup::enableTitle(bool b)
{
    m_enableTitle = b;
}

void SARibbonGalleryGroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void SARibbonGalleryGroup::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}
