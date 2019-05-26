#include "SAXMLReadHelper.h"
#include <QDomNodeList>
#include <QDomNamedNodeMap>
#include <QXmlStreamWriter>
#include "SADataFeatureItem.h"
#include "SAVariantCaster.h"
#include <QDebug>
SAXMLReadHelper::SAXMLReadHelper(const QString& str)
    :m_isvalid(false)
    ,m_protocolType(SA_XML::UnknowType)
{
    if(m_doc.setContent(str,&m_errorMsg))
    {
        init();
    }
}

void SAXMLReadHelper::init()
{
    m_rootEle =m_doc.documentElement();
    QString rootName = m_rootEle.tagName();
    if(SA_XML_TAG_SA == rootName)
    {
        QDomNamedNodeMap atts = m_rootEle.attributes();
        if(atts.contains(SA_XML_ATT_TYPE))
        {
            QString typeStr = atts.namedItem(SA_XML_ATT_TYPE).nodeValue();
            if(ATT_SA_TYPE_VPFR == typeStr)
            {
                //说明是
                m_protocolType = SA_XML::TypeVectorPointFProcessResult;
                m_isvalid = true;
            }
        }
        QDomNodeList nls = m_rootEle.elementsByTagName(SA_XML_TAG_HEADER_ROOT);
        if(nls.size() > 0)
        {
            m_headerEle = nls.at(nls.size()-1).toElement();
        }
        nls = m_rootEle.elementsByTagName(SA_XML_TAG_CONTENT_ROOT);
        if(nls.size() > 0)
        {
            m_contentEle = nls.at(nls.size()-1).toElement();
        }
    }
}



QString SAXMLReadHelper::getErrorMsg() const
{
    return m_errorMsg;
}



SA_XML::ProtocolType SAXMLReadHelper::getProtocolType() const
{
    return m_protocolType;
}

/**
 * @brief 获取点序列处理结果
 * @param key 返回的key
 * @param item 解析好的item
 * @return 成功返回true，否则返回false
 */
bool SAXMLReadHelper::getVectorPointFProcessResult(uint &key, SADataFeatureItem *item)
{
    //前两个是记录的指针
    if(SA_XML::TypeVectorPointFProcessResult != m_protocolType)
    {
        qDebug()<<"TypeVectorPointFProcessResult != m_protocolType";
        return false;
    }
    if(!m_headerEle.isNull())
    {
        QDomNodeList els = m_headerEle.elementsByTagName(SA_XML_TAG_UINTVAL);
        for(int i=0;i<els.size();++i)
        {
            QDomElement el = els.at(i).toElement();
            if(!el.isNull())
            {
                QString name = el.attribute(SA_XML_ATT_NAME);
                if(name == "key")
                {
                    bool isOK = false;
                    uint v = el.text().toUInt(&isOK);
                    if(isOK)
                    {
                        key = v;
                    }
                }
            }
        }
    }
    return readDfi(item);
}

/**
 * @brief 读取dfi节点
 * @param item
 * @return 读取成功返回true
 */
bool SAXMLReadHelper::readDfi(SADataFeatureItem *item)
{
    bool isOK = false;
    QDomNodeList nodes = m_rootEle.elementsByTagName(SA_XML_TAG_DFI_ROOT);
    if(!nodes.isEmpty())
    {
        QDomNode nodeRoot = nodes.at(0);
        QDomNodeList groupsNode = nodeRoot.childNodes();
        for(int i=0;i<groupsNode.size();++i)
        {
            QDomNode groupItem = groupsNode.at(i);
            if(SA_XML_TAG_GROUP == groupItem.nodeName())
            {
                //读取子对象
                QString groupName;
                if(!getAttrValue(groupItem,SA_XML_ATT_NAME,groupName))
                {
                    continue;
                }
                SADataFeatureItem* group = new SADataFeatureItem(groupName);
                if(readDfiChildItem(groupItem,group))
                {
                    isOK = true;
                    item->appendItem(group);
                }
            }
            else
            {
                qDebug() << "unknow tag :"<<groupItem.toElement().tagName() << " under tag "<<SA_XML_TAG_DFI_ROOT;
            }
        }
    }
    else
    {
        qDebug() << "can not find tag:"<<SA_XML_TAG_DFI_ROOT;
    }
    return isOK;
}

bool SAXMLReadHelper::isValid() const
{
    return m_isvalid;
}

bool SAXMLReadHelper::getAttrValue(const QDomNode &xmlItem,const QString& name, QString &value)
{
    if(xmlItem.isNull())
    {
        return false;
    }
    QDomNamedNodeMap attrs = xmlItem.attributes();
    QDomNode att = attrs.namedItem(name);
    if(!att.isAttr())
    {
        return false;
    }
    value = att.toAttr().value();
    return true;
}

bool SAXMLReadHelper::getDfiParamItem(const QDomNode &xmlItem, SADataFeatureItem *item)
{
    if(xmlItem.isNull())
    {
        return false;
    }
    QDomNamedNodeMap attrs = xmlItem.attributes();
    QDomNode att = attrs.namedItem(SA_XML_ATT_NAME);
    if(!att.isAttr())
    {
        return false;
    }
    item->setName(att.toAttr().value());

    att = attrs.namedItem(SA_XML_ATT_TYPE);
    if(!att.isAttr())
    {
        return false;
    }
    if(ATT_TYPE_VALUE != att.toAttr().value())
    {
        return false;
    }

    att = attrs.namedItem(SA_XML_ATT_VALUE_TYPE);
    if(!att.isAttr())
    {
        return false;
    }
    QString varTypeStr = att.toAttr().value();
    att = attrs.namedItem(SA_XML_ATT_VALUE);
    if(!att.isAttr())
    {
        return false;
    }
    QString varStr = att.toAttr().value();
    QVariant value = SAVariantCaster::stringToVariant(varStr,varTypeStr);
    item->setValue(value);
    return true;
}
/**
 * @brief 读取dfi节点下的子节点
 * @param groupItem
 * @param parentItem
 * @return
 */
bool SAXMLReadHelper::readDfiChildItem(const QDomNode &groupItem, SADataFeatureItem *parentItem)
{
    if(!groupItem.hasChildNodes())
    {
        return false;
    }
    QDomNodeList itemList = groupItem.childNodes();//(XML_STR_ITEM__);
    for(int i = 0;i<itemList.size();++i)
    {
        if(SA_XML_TAG_ITEM == itemList.at(i).nodeName())
        {
            SADataFeatureItem* item = new SADataFeatureItem();
            if(!getDfiParamItem(itemList.at(i),item))
            {
                delete item;
                continue;
            }
            parentItem->appendItem(item);
        }
        else if(SA_XML_TAG_GROUP == itemList.at(i).nodeName())
        {
            //遇到嵌套group
            QString groupName;
            if(!getAttrValue(itemList.at(i),SA_XML_ATT_NAME,groupName))
            {
                continue;
            }
            SADataFeatureItem* group = new SADataFeatureItem(groupName);
            if(!readDfiChildItem(itemList.at(i),group))
            {
                delete group;
                continue;
            }
            parentItem->appendItem(group);

        }

    }
    return true;
}

//====================================


