#ifndef SARIBBONGALLERYGROUP_H
#define SARIBBONGALLERYGROUP_H
#include <QList>
#include "SARibbonGalleryItem.h"
#include <QWidget>
///
/// \brief Gallery的组
///
/// 组负责管理Gallery Item，
///
class SARibbonGalleryGroup : public QWidget
{
    Q_OBJECT
public:
    SARibbonGalleryGroup(QWidget* w = 0);
    virtual ~SARibbonGalleryGroup();
    void append(SARibbonGalleryItem* item);
    SARibbonGalleryItem* at(int index);
    int size() const;
    SARibbonGalleryItem* takeAt(int index);

    QList<SARibbonGalleryItem*>& itemList();
    const QList<SARibbonGalleryItem*>& itemList() const;
    QSize sizeHint() const;


protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    QList<SARibbonGalleryItem*> m_items;
    QString m_title;
};

#endif // SARIBBONGALLERYGROUP_H
