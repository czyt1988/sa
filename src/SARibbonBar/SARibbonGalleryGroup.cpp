#include "SARibbonGalleryGroup.h"

SARibbonGalleryGroup::SARibbonGalleryGroup(QWidget *w):QWidget(w)
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
    m_items.at(index);
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

}

void SARibbonGalleryGroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void SARibbonGalleryGroup::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}
