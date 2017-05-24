#ifndef MODALTREE_PROPERTY_H
#define MODALTREE_PROPERTY_H
#include <QAbstractItemModel>
#include <memory>
#include <QList>
#include "TreeItem.h"
#include <QPointF>
///
/// \brief 这是专门针对属性表显示的item，属性表类似于vs2010的属性表和qt designer的属性设置
///
class TreeItem_Property : public TreeItem
{
public:
    explicit TreeItem_Property():TreeItem(nullptr){
        bisSetPointMark = false;
    }
    explicit TreeItem_Property(const QString &name
                               ,TreeItem_Property *parent = nullptr)
        :TreeItem(static_cast<TreeItem*>(parent)){
        bisSetPointMark = false;
        TreeItem::setData(name,0);
        TreeItem::setData(QStringLiteral(""),1);
    }
    explicit TreeItem_Property(const QString &name
                               , const QVariant &data
                               ,TreeItem_Property *parent = nullptr)
        :TreeItem(static_cast<TreeItem*>(parent)){
        bisSetPointMark = false;
        TreeItem::setData(name,0);
        TreeItem::setData(data,1);
    }
    ~TreeItem_Property(){
    }

public:
    void setData(const QVariant &varData){
        TreeItem::setData(varData,1);//设置值在Property里就是第二个值
    }
    void addItem(const QString &name, const QVariant &data,bool enableEdit = true){
        TreeItem_Property*  pItem = new TreeItem_Property(name,data,this);
        pItem->setEnableEdit(enableEdit);
       // TreeItem::addChild(static_cast<TreeItem*>(pItem));
    }

    ///
    /// \brief 为了方便记录点的位置而设置的存放数据点的方法
    /// \param pos
    ///
    void setPointMark(const QPointF& pos)
    {
        bisSetPointMark = true;
        pri_point = pos;
    }
    QPointF getMarkPoint()
    {
        return pri_point;
    }
    bool isSetMarkPoint(){
        return bisSetPointMark;
    }
    void removePointMark(){
        bisSetPointMark = false;
    }
    ///
    /// \brief 设置说明
    /// \param des
    ///
    void setDes(const QString& des){
        strDes = des;
    }
    QString getDes(){
        return strDes;
    }

private:
    QPointF pri_point;
    bool bisSetPointMark;
    QString strDes;
};

///
/// \brief 这是专门针对属性表显示的modal，属性表类似于vs2010的属性表和qt designer的属性设置
///
class ModelTree_Property : public QAbstractItemModel
{
public:
    explicit ModelTree_Property();
    ///
    /// \brief 构造函数2
    /// \param 属性表的表头1
    /// \param 属性表的表头2
    ///
    explicit ModelTree_Property(const QString& strhead1,const QString& strhead2);
public:
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                       int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                     const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
public:


    ///
    /// \brief update 更新模型
    ///
    void update(){
        beginResetModel();
        endResetModel();
    }

    ///
    /// \brief 设置表头名字
    /// \param strhead1
    /// \param strhead2
    ///
    void setHeaderString(const QString& strhead1,const QString& strhead2);

    void addItem(const QString &name, const QVariant &data,bool enableEdit = true){
        m_pRootItem->addItem(name,data,enableEdit);
    }
    void addProperty(TreeItem_Property* property){
        property->setParent(static_cast<TreeItem*>(m_pRootItem.get()));
    }

    TreeItem_Property* root(){
        return m_pRootItem.get();
    }

    void clear(){
        m_pRootItem->deleteAllChild();
    }

private:
    std::unique_ptr<TreeItem_Property> m_pRootItem;//root节点作为表头存放
};

#endif // MODALTREE_PROPERTY_H
