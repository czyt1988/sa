#ifndef SARIBBONGALLERYITEM_H
#define SARIBBONGALLERYITEM_H
#include <QIcon>
#include <QVariant>
#include <QMap>
class SARibbonGalleryGroup;
///
/// \brief 类似QStandardItem的GalleryItem
///
class SARibbonGalleryItem
{
public:
    SARibbonGalleryItem();
    //设置角色
    void setData(int role,const QVariant& data);
    QVariant data(int role) const;
    //设置文字描述
    void setText(const QString& text);
    QString text() const;
    //设置tooltip
    void setToolTip(const QString& text);
    QString toolTip() const;
    //设置图标
    void setIcon(const QIcon& ico);
    QIcon icon() const;

    //设置是否可见
    bool isVisible() const;
    void setVisible(bool isVisible);
    //设置是否可选
    bool isEnable() const;
    void setEnable(bool isEnable);

private:
    friend class SARibbonGalleryGroup;
    QMap<int,QVariant> m_datas;
    bool m_isVisible;
    bool m_isEnable;
    SARibbonGalleryGroup* m_group;
};

#endif // SARIBBONGALLERYITEM_H
