#ifndef SARIBBONGALLERYITEM_H
#define SARIBBONGALLERYITEM_H
#include <QIcon>
#include <QVariant>
#include <QMap>
#include <QListWidgetItem>
class SARibbonGalleryGroup;
///
/// \brief 类似QStandardItem的GalleryItem
///
class SARibbonGalleryItem
{
public:
    SARibbonGalleryItem();
    SARibbonGalleryItem(const QString &text, QListWidget *parent = Q_NULLPTR, int type = Type);
    SARibbonGalleryItem(const QIcon &icon, const QString &text, QListWidget *parent = Q_NULLPTR, int type = Type);
    SARibbonGalleryItem(const SARibbonGalleryItem& other);
    SARibbonGalleryItem* clone();



private:
    friend class SARibbonGalleryGroup;
};

#endif // SARIBBONGALLERYITEM_H
