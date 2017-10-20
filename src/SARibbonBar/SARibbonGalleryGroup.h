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
    //设置item的尺寸，一个group的item只有一个尺寸，默认为72X56
    void setItemSize(const QSize& size);
    void enableTitle(bool b);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    bool m_enableTitle;///< 是否显示标题
    QList<SARibbonGalleryItem*> m_items;
    QString m_title;
    QSize m_itemSize;
};

#endif // SARIBBONGALLERYGROUP_H
