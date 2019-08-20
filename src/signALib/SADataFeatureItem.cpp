#include "SADataFeatureItem.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QPointF>
#include <QDebug>
#define ROLE_ITEM_TYPE (Qt::UserRole + 1234)
#define XML_STR_ROOT__ "dfi"
#define XML_STR_ITEM__ "item"
#define XML_STR_CHILD__ "child"
#define XML_ATT_TYPE__ "type"
#define XML_ATT_NAME__ "name"
#define XML_ATT_VALUE_TYPE__ "varType"
#define XML_ATT_VALUE__ "var"

class SADataFeatureItemPrivate
{
    SA_IMPL_PUBLIC(SADataFeatureItem)
public:
    QString m_name;
    SADataFeatureItem* m_parent;
    int m_currentRowIndex;
    QList<SADataFeatureItem*> m_childs;
    QVariant m_value;
    QHash<int,QVariant> m_datas;
    SADataFeatureItemPrivate(SADataFeatureItem* p):q_ptr(p)
      ,m_name(""),m_parent(nullptr),m_currentRowIndex(0)
    {

    }
};



SADataFeatureItem::SADataFeatureItem()
    :d_ptr(new SADataFeatureItemPrivate(this))
{

}

SADataFeatureItem::SADataFeatureItem(const QString &text)
    :d_ptr(new SADataFeatureItemPrivate(this))
{
    setName(text);
}


SADataFeatureItem::SADataFeatureItem(const QString &name, const QVariant &data)
    :d_ptr(new SADataFeatureItemPrivate(this))
{
    setName(name);
    setValue(data);
}

void SADataFeatureItem::setValue(const QVariant &var)
{
    SA_D(SADataFeatureItem);
    d->m_value = var;
}

QVariant SADataFeatureItem::getValue() const
{
    SA_DC(SADataFeatureItem);
    return d->m_value;
}





SADataFeatureItem::~SADataFeatureItem()
{
    SA_D(SADataFeatureItem);
    std::for_each(d->m_childs.begin(),d->m_childs.end(),[](SADataFeatureItem* item){
       if(item)
       {
           delete item;
       }
    });
    d->m_childs.clear();
}

void SADataFeatureItem::appendItem(SADataFeatureItem *item)
{
    SA_D(SADataFeatureItem);
    d->m_childs.append(item);
    item->d_ptr->m_currentRowIndex = d->m_childs.size()-1;
    item->setParent(this);
}

///
/// \brief 子条目添加点序列条目
/// \param name
/// \param datas
///
SADataFeatureItem *SADataFeatureItem::appendItem(const QString &name,const QVector<QPointF> &datas)
{
    SADataFeatureItem* item = new SADataFeatureItem(name);
    for(int i=0;i<datas.size();++i)
    {
        item->appendItem(QString::number(i+1),datas.at(i));
    }
    appendItem(item);
    return item;
}
///
/// \brief 子条目添加数组序列
/// \param datas
///
SADataFeatureItem* SADataFeatureItem::appendItem(const QString &name,const QVector<double> &datas)
{
    SADataFeatureItem* item = new SADataFeatureItem(name);
    for(int i=0;i<datas.size();++i)
    {
        item->appendItem(QString::number(i+1),datas.at(i));
    }
    appendItem(item);
    return item;
}

SADataFeatureItem* SADataFeatureItem::appendItem(const QString &name, const QVariant &data)
{
    SADataFeatureItem* item = new SADataFeatureItem(name,data);
    appendItem(item);
    return item;
}

QVariant SADataFeatureItem::getBackground() const
{
    return getData(Qt::BackgroundRole);
}

///
/// \brief 获取颜色
/// \return
///
QBrush SADataFeatureItem::getBackgroundBrush() const
{
    QVariant v = getData(Qt::BackgroundRole);
    if(v.isValid())
    {
        if(v.canConvert<QBrush>())
        {
            return v.value<QBrush>();
        }
    }
    return QBrush();
}

void SADataFeatureItem::setBackground(const QBrush &b)
{
    setData(b,Qt::BackgroundRole);
}

void SADataFeatureItem::setBackgroundColor(const QColor &c)
{
    setBackground(QBrush(c));
}

int SADataFeatureItem::getValueType() const
{
    return d_ptr->m_value.type();
}







///
/// \brief 获取最顶层条目
/// \return
///
SADataFeatureItem *SADataFeatureItem::topParent() const
{
    SADataFeatureItem* cur = const_cast<SADataFeatureItem *>(this);
    SADataFeatureItem* par = static_cast<SADataFeatureItem *>(cur->getParent());
    while(par != nullptr)
    {
        cur = par;
        par = static_cast<SADataFeatureItem *>(cur->getParent());
    }
    return cur;
}


void SADataFeatureItem::setName(const QString &name)
{
    SA_D(SADataFeatureItem);
    d->m_name = name;
}

QString SADataFeatureItem::getName() const
{
    SA_DC(SADataFeatureItem);
    return d->m_name;
}

QVariant SADataFeatureItem::getData(int role) const
{
    SA_DC(SADataFeatureItem);
    return d->m_datas.value(role,QVariant());
}

void SADataFeatureItem::setData(const QVariant &var, int role)
{
    SA_D(SADataFeatureItem);
    d->m_datas[role] = var;
}





///
/// \brief 获取当前行
/// \return
///
int SADataFeatureItem::getCurrentRowIndex() const
{
    SA_DC(SADataFeatureItem);
    return d->m_currentRowIndex;
}
///
/// \brief 设置子条目指针
/// \param index 索引
/// \param newItemPtr 新指针
/// \return 返回旧的指针，返回的指针内存交由调用者管理
///
SADataFeatureItem *SADataFeatureItem::setChild(int index, SADataFeatureItem *newItemPtr)
{
    SA_D(SADataFeatureItem);
    SADataFeatureItem * oldItem = getChild(index);
    d->m_childs[index] = newItemPtr;
    newItemPtr->d_ptr->m_currentRowIndex = oldItem->getCurrentRowIndex();
    return oldItem;
}

SADataFeatureItem *SADataFeatureItem::getParent() const
{
    SA_DC(SADataFeatureItem);
    return d->m_parent;
}
///
/// \brief 获取子条目数
/// \return
///
int SADataFeatureItem::getChildCount() const
{
    SA_DC(SADataFeatureItem);
    return d->m_childs.size();
}
///
/// \brief 获取子节点
/// \param index
/// \return
///
SADataFeatureItem *SADataFeatureItem::getChild(int index) const
{
    SA_DC(SADataFeatureItem);
    return d->m_childs[index];
}

void SADataFeatureItem::setParent(SADataFeatureItem *parent)
{
    SA_D(SADataFeatureItem);
    d->m_parent = parent;
}
