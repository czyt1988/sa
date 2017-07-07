#include "SADataFeatureItem.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QPointF>
#include <QDebug>
#include "SAVariantCaster.h"
#define ROLE_ITEM_TYPE (Qt::UserRole + 1234)
#define XML_STR_ROOT__ "dfi"
#define XML_STR_ITEM__ "item"
#define XML_STR_CHILD__ "child"
#define XML_ATT_TYPE__ "type"
#define XML_ATT_NAME__ "name"
#define XML_ATT_VALUE_TYPE__ "varType"
#define XML_ATT_VALUE__ "var"
SADataFeatureItem::SADataFeatureItem()
    :m_name(""),m_parent(nullptr),m_currentRowIndex(0)
{
    m_childs.clear();
}

SADataFeatureItem::SADataFeatureItem(const QString &text)
    :m_name(""),m_parent(nullptr),m_currentRowIndex(0)
{
    setItemType(DescribeItem);
    setName(text);
}


SADataFeatureItem::SADataFeatureItem(const QString &name, const QVariant &data)
    :m_name(""),m_parent(nullptr),m_currentRowIndex(0)
{
    setName(name);
    setValue(data);
    setItemType(ValueItem);
}

void SADataFeatureItem::setValue(const QVariant &var)
{
    m_value = var;
}

QVariant SADataFeatureItem::getValue() const
{
    return m_value;
}





SADataFeatureItem::~SADataFeatureItem()
{
    std::for_each(m_childs.begin(),m_childs.end(),[](SADataFeatureItem* item){
       if(item)
       {
           delete item;
       }
    });
    m_childs.clear();
}

void SADataFeatureItem::appendItem(SADataFeatureItem *item)
{
    m_childs.append(item);
    item->m_currentRowIndex = m_childs.size()-1;
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





SADataFeatureItem::ItemType SADataFeatureItem::getItemType() const
{
    QVariant var = getData(ROLE_ITEM_TYPE);
    if(!var.isValid())
    {
        return UnKnow;
    }
    bool isOK = false;
    int d = var.toInt(&isOK);
    if(!isOK)
    {
        return UnKnow;
    }
    return static_cast<SADataFeatureItem::ItemType>(d);
}

void SADataFeatureItem::setItemType(SADataFeatureItem::ItemType type)
{
    setData((int)type,ROLE_ITEM_TYPE);
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
///
/// \brief 转换为xml
///
/// 此转换不是标准xml文件，而是以<fi>打头</fi>结尾的xml文件
/// \return
///
QString SADataFeatureItem::toXml() const
{
    return toXml(this);
}
///
/// \brief 从xml转换为item
/// \param xml
///
bool SADataFeatureItem::fromXml(const QString &xml)
{
    return fromXml(xml,this);
}
///
/// \brief 转换为xml
///
/// 此转换不是标准xml文件，而是以<fi>打头</fi>结尾的xml文件
/// \param item SADataFeatureItem指针
/// \return
///
QString SADataFeatureItem::toXml(const SADataFeatureItem *item)
{
    QString str;
    QXmlStreamWriter xml(&str);
    //为了便于观察，使用格式化
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(2);
    //
    xml.setCodec("UTF-8");
    xml.writeStartElement(XML_STR_ROOT__);
    writeItem(&xml,item);
    xml.writeEndElement();
    return str;
}


void SADataFeatureItem::setName(const QString &name)
{
    m_name = name;
}

QString SADataFeatureItem::getName() const
{
    return m_name;
}

QVariant SADataFeatureItem::getData(int role) const
{
    return m_datas.value(role,QVariant());
}

void SADataFeatureItem::setData(const QVariant &var, int role)
{
    m_datas[role] = var;
}



int SADataFeatureItem::getTypeInt(const SADataFeatureItem *item)
{
    QVariant var = item->getData(ROLE_ITEM_TYPE);
    if(!var.isValid())
    {
        return 0;
    }
    bool isOK = false;
    return var.toInt(&isOK);
}
///
/// \brief 从xml转换为item
/// \param xml
/// \param item
///
bool SADataFeatureItem::fromXml(const QString &xmlStr, SADataFeatureItem *item)
{
    bool isOK = false;
    QDomDocument doc;
    isOK = doc.setContent(xmlStr,false);
    if(!isOK)
    {
        return false;
    }
    QDomNodeList nodes = doc.elementsByTagName(XML_STR_ROOT__);
    if(!nodes.isEmpty())
    {
        QDomNode nodeRoot = nodes.at(0);
        //第一个item就是root item
        QDomElement rootItem = nodeRoot.firstChildElement(XML_STR_ITEM__);
        isOK = readRootItem(&rootItem,item);
        //读取子对象
        if(!rootItem.hasChildNodes())
        {
            return isOK;
        }
        QDomNodeList childs = rootItem.elementsByTagName(XML_STR_CHILD__);
        if(!childs.isEmpty())
        {
            QDomElement childEle = childs.at(0).toElement();
            readChildItem(&childEle,item);
        }
    }
    return isOK;
}

void SADataFeatureItem::writeItem(QXmlStreamWriter *xml, const SADataFeatureItem *item)
{
    if(nullptr == item)
    {
        return;
    }
    xml->writeStartElement(XML_STR_ITEM__);
    QXmlStreamAttributes attrs;
    attrs.append(QXmlStreamAttribute(XML_ATT_TYPE__,QString::number(getTypeInt(item))));
    attrs.append(QXmlStreamAttribute(XML_ATT_NAME__,item->getName()));
    attrs.append(QXmlStreamAttribute(XML_ATT_VALUE_TYPE__,item->getValue().typeName()));
    attrs.append(QXmlStreamAttribute(XML_ATT_VALUE__,SAVariantCaster::variantToString(item->getValue())));
    xml->writeAttributes(attrs);
    //递归子节点
    const int subItemCount = item->getChildCount();
    if(subItemCount>0)
    {
        xml->writeStartElement(XML_STR_CHILD__);
        for(int i=0;i<subItemCount;++i)
        {
            writeItem(xml,item->getChild(i));
        }
        xml->writeEndElement();
    }
    xml->writeEndElement();
}




bool SADataFeatureItem::readRootItem(QDomElement *xmlItem, SADataFeatureItem *item)
{
    return getItemInfoFromElement(xmlItem,item);
}

bool SADataFeatureItem::readChildItem(QDomElement *xmlItem, SADataFeatureItem *parentItem)
{
    if(!xmlItem->hasChildNodes())
    {
        return false;
    }
    QDomNodeList itemList = xmlItem->childNodes();//(XML_STR_ITEM__);
    for(int i = 0;i<itemList.size();++i)
    {
        if(itemList.at(i).nodeName() != XML_STR_ITEM__)
        {
            continue;
        }
        SADataFeatureItem* item = new SADataFeatureItem();
        QDomElement subItemEle = itemList.at(i).toElement();
        if(!getItemInfoFromElement(&subItemEle,item))
        {
            delete item;
            return false;
        }
        //读取子对象
        if(subItemEle.hasChildNodes())
        {
            QDomNodeList childs = subItemEle.elementsByTagName(XML_STR_CHILD__);
            if(!childs.isEmpty())
            {
                QDomElement childEle = childs.at(0).toElement();
                readChildItem(&childEle,item);
            }
        }
        parentItem->appendItem(item);
    }
    return true;
}

bool SADataFeatureItem::getItemInfoFromElement(QDomElement *xmlItem, SADataFeatureItem *item)
{
    if(xmlItem->isNull())
    {
        return false;
    }
    QDomNamedNodeMap attrs = xmlItem->attributes();
    QDomNode att = attrs.namedItem(XML_ATT_NAME__);
    if(!att.isAttr())
    {
        return false;
    }
    item->setName(att.toAttr().value());

    att = attrs.namedItem(XML_ATT_TYPE__);
    if(!att.isAttr())
    {
        return false;
    }
    int type = att.toAttr().value().toInt();
    item->setItemType(static_cast<SADataFeatureItem::ItemType>(type));

    att = attrs.namedItem(XML_ATT_VALUE_TYPE__);
    if(!att.isAttr())
    {
        return false;
    }
    QString varTypeStr = att.toAttr().value();
    att = attrs.namedItem(XML_ATT_VALUE__);
    if(!att.isAttr())
    {
        return false;
    }
    QString varStr = att.toAttr().value();
    QVariant value = SAVariantCaster::stringToVariant(varStr,varTypeStr);
    item->setValue(value);
    return true;
}
///
/// \brief 获取当前行
/// \return
///
int SADataFeatureItem::getCurrentRowIndex() const
{
    return m_currentRowIndex;
}
///
/// \brief 设置子条目指针
/// \param index 索引
/// \param newItemPtr 新指针
/// \return 返回旧的指针，返回的指针内存交由调用者管理
///
SADataFeatureItem *SADataFeatureItem::setChild(int index, SADataFeatureItem *newItemPtr)
{
    SADataFeatureItem * oldItem = getChild(index);
    m_childs[index] = newItemPtr;
    newItemPtr->m_currentRowIndex = oldItem->getCurrentRowIndex();
    return oldItem;
}

SADataFeatureItem *SADataFeatureItem::getParent() const
{
    return m_parent;
}
///
/// \brief 获取子条目数
/// \return
///
int SADataFeatureItem::getChildCount() const
{
    return m_childs.size();
}
///
/// \brief 获取子节点
/// \param index
/// \return
///
SADataFeatureItem *SADataFeatureItem::getChild(int index) const
{
    return m_childs[index];
}

void SADataFeatureItem::setParent(SADataFeatureItem *parent)
{
    m_parent = parent;
}
