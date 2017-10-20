#include "SARibbonGalleryItem.h"
#include "SARibbonGalleryGroup.h"

SARibbonGalleryItem::SARibbonGalleryItem()
{

}

SARibbonGalleryItem::SARibbonGalleryItem(const QString &text, QListWidget *parent, int type)
    :QListWidgetItem(text,parent,type)
{

}

SARibbonGalleryItem::SARibbonGalleryItem(const QIcon &icon, const QString &text, QListWidget *parent, int type)
    :QListWidgetItem(icon,text,parent,type)
{

}


SARibbonGalleryItem::SARibbonGalleryItem(const SARibbonGalleryItem &other)
    :QListWidgetItem(other.icon(),other.text(),other.listWidget(),other.type())
{

}


SARibbonGalleryItem *SARibbonGalleryItem::clone()
{
    return new SARibbonGalleryItem(*this);
}
