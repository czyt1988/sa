#include "SADataFeatureItem.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QPointF>
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
    QXmlStreamReader xml(xmlStr);
    while(!xml.atEnd())
    {
        if(xml.readNextStartElement())
        {
            if(xml.name()==XML_STR_ROOT__)
            {
                //读取到根目录
                isOK = readItem(&xml,item);
            }
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

bool SADataFeatureItem::readItem(QXmlStreamReader *xml, SADataFeatureItem *item)
{
    bool isReadItem = false;
    while(!xml->atEnd())
    {
        if(xml->readNextStartElement())
        {
            if(xml->name()==XML_STR_ITEM__)
            {
                //读取到条目
                QXmlStreamAttributes atts = xml->attributes();
                QString text,varType;
                QVariant value;
                ItemType type = UnKnow;
                for(int i = 0;i < atts.size();++i)
                {
                    if(XML_ATT_TYPE__ == atts[i].name())
                    {
                        bool isOK = false;
                        int v = atts[i].value().toInt(&isOK);
                        if(!isOK)
                        {
                            continue;
                        }
                        type = static_cast<SADataFeatureItem::ItemType>(v);
                    }
                    else if(XML_ATT_NAME__ == atts[i].name())
                    {
                        text = atts[i].value().toString();
                    }
                    else if(XML_ATT_VALUE_TYPE__ == atts[i].name())
                    {
                        varType = atts[i].value().toString();
                    }
                }
                //必须读取了类型才能解析值,所以这个循环分开，以确保安全
                for(int i = 0;i < atts.size();++i)
                {
                    if(XML_ATT_VALUE__ == atts[i].name())
                    {
                        QString varStr = atts[i].value().toString();
                        switch(type)
                        {
                        case UnKnow:
                        case DescribeItem:
                            value = varStr;
                            break;
                        case ValueItem:
                        case PointItem:
                            value = SAVariantCaster::stringToVariant(varStr,varType);
                            break;
                        }
                    }
                }
                item->setName(text);
                item->setValue(value);
                item->setItemType(type);
                isReadItem = true;
            }
            if(xml->name()==XML_STR_CHILD__)
            {
                //子条目
                SADataFeatureItem* childItem = new SADataFeatureItem();
                if(readItem(xml,childItem))
                {
                    item->appendItem(childItem);
                }
                else
                {
                    delete childItem;
                    childItem = nullptr;
                }
            }
        }

    }
    return isReadItem;
}
///
/// \brief 获取当前行
/// \return
///
int SADataFeatureItem::getCurrentRowIndex() const
{
    return m_currentRowIndex;
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
