#include "SARibbonGalleryItem.h"
#include "SARibbonGalleryGroup.h"

SARibbonGalleryItem::SARibbonGalleryItem():m_group(nullptr)
{

}



void SARibbonGalleryItem::setData(int role, const QVariant &data)
{
    m_datas[role] = data;
}

QVariant SARibbonGalleryItem::data(int role) const
{
    return m_datas.value(role);
}

void SARibbonGalleryItem::setText(const QString &text)
{
    setData(Qt::DisplayRole,text);
}

QString SARibbonGalleryItem::text() const
{
    return data(Qt::DisplayRole).toString();
}

void SARibbonGalleryItem::setToolTip(const QString &text)
{
    setData(Qt::ToolTipRole, text);
}

QString SARibbonGalleryItem::toolTip() const
{
    return data(Qt::ToolTipRole).toString();
}

void SARibbonGalleryItem::setIcon(const QIcon &ico)
{
    setData(Qt::DecorationRole, ico);
}

QIcon SARibbonGalleryItem::icon() const
{
    return qvariant_cast<QIcon>(data(Qt::DecorationRole));
}

bool SARibbonGalleryItem::isVisible() const
{
    return m_isVisible;
}

void SARibbonGalleryItem::setVisible(bool isVisible)
{
    m_isVisible = isVisible;
    if(m_group)
    {
        m_group->update();
    }
}

bool SARibbonGalleryItem::isEnable() const
{
    return m_isEnable;
}

void SARibbonGalleryItem::setEnable(bool isEnable)
{
    m_isEnable = isEnable;
    if(m_group)
    {
        m_group->update();
    }
}
