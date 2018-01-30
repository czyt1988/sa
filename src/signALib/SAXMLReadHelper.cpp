#include "SAXMLReadHelper.h"
#include "SAXMLTagDefined.h"
#include <QXmlStreamReader>
#include "SADataFeatureItem.h"
#include "SAVariantCaster.h"
#include <QDebug>
SAXMLReadHelper::SAXMLReadHelper(const QString& str)
    :m_xml(nullptr)
    ,m_isvalid(false)
    ,m_protocolType(SAXMLReadHelper::UnknowType)
    ,m_strPtr(&str)
{
    m_xml.reset(new QXmlStreamReader(str));
    init();
}

void SAXMLReadHelper::init()
{

    while(!m_xml->atEnd())
    {
        if(m_xml->readNextStartElement())
        {
            if(m_xml->name() == SA_XML_TAG_SA)
            {
                //是以<sa>打头的协议
                QXmlStreamAttributes atts = m_xml->attributes();
                for(int i=0;i<atts.size();++i)
                {
                    if(SA_XML_ATT_TYPE == atts[i].name())//取属性 type=“xx”
                    {
                        QString typeStr = atts[i].value().toString();
                        if(ATT_SA_TYPE_VPFR == typeStr)
                        {
                            //说明是
                            m_protocolType = TypeVectorPointFProcessResult;
                            m_isvalid = true;
                            return;
                        }
                    }
                }

            }
        }
    }
    qDebug() << m_xml->errorString();
}



SAXMLReadHelper::ProtocolType SAXMLReadHelper::getProtocolType() const
{
    return m_protocolType;
}

bool SAXMLReadHelper::getVectorPointFProcessResult(quintptr &wnd, quintptr &fig, quintptr &itp, SADataFeatureItem *item)
{
    //前两个是记录的指针
    if(TypeVectorPointFProcessResult != m_protocolType)
    {
        qDebug()<<"TypeVectorPointFProcessResult != m_protocolType";
        return false;
    }
    int readCount = 0;
    while(!m_xml->atEnd())
    {
        if(m_xml->readNextStartElement())
        {
            if(SA_XML_TAG_QUINTPTR == m_xml->name())
            {
                if(0 == readCount)
                {
                    wnd = m_xml->readElementText().toUInt();
                    readCount++;
                }
                else if(1 == readCount)
                {
                    fig = m_xml->readElementText().toUInt();
                    readCount++;
                }
                else if(2 == readCount)
                {
                    itp = m_xml->readElementText().toUInt();
                    readCount++;
                    break;
                }
            }
        }
    }
    return SADataProcessVectorPointFXMLHelper::read(*m_strPtr,item);
}

bool SAXMLReadHelper::isValid() const
{
    return m_isvalid;
}



//====================================



class SADataProcessVectorPointFXMLHelperPrivate
{
    SA_IMPL_PUBLIC(SADataProcessVectorPointFXMLHelper)
public:
    QScopedPointer<QXmlStreamWriter> xmlWriter;
    SADataProcessVectorPointFXMLHelper* Parent;
    SADataProcessVectorPointFXMLHelperPrivate(SADataProcessVectorPointFXMLHelper* p)
        :Parent(p)
        ,xmlWriter(nullptr)
    {

    }
    void startWrite(QString *string)
    {
        xmlWriter.reset(new QXmlStreamWriter(string));
        //为了便于观察，使用格式化
        xmlWriter->setAutoFormatting(true);
        xmlWriter->setAutoFormattingIndent(2);
        xmlWriter->setCodec("UTF-8");
        xmlWriter->writeStartElement(SA_XML_TAG_DFI_ROOT);
    }
    void endWrite()
    {
        xmlWriter->writeEndElement();
    }
    void startWriteGroup(const QString &name)
    {
        xmlWriter->writeStartElement(SA_XML_TAG_GROUP);
        xmlWriter->writeAttribute(SA_XML_ATT_NAME,name);
    }
    void endWriteGroup()
    {
        xmlWriter->writeEndElement();
    }

    void writeValue(const QVariant& d, const QString &name)
    {
        xmlWriter->writeStartElement(SA_XML_TAG_ITEM);
        QXmlStreamAttributes attrs;
        attrs.append(QXmlStreamAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VALUE));
        attrs.append(QXmlStreamAttribute(SA_XML_ATT_NAME,name));
        attrs.append(QXmlStreamAttribute(SA_XML_ATT_VALUE_TYPE,d.typeName()));
        attrs.append(QXmlStreamAttribute(SA_XML_ATT_VALUE,SAVariantCaster::variantToString(d)));
        xmlWriter->writeAttributes(attrs);
        xmlWriter->writeEndElement();
    }
};

SADataProcessVectorPointFXMLHelper::SADataProcessVectorPointFXMLHelper()
    :d_ptr(new SADataProcessVectorPointFXMLHelperPrivate(this))
{

}

SADataProcessVectorPointFXMLHelper::~SADataProcessVectorPointFXMLHelper()
{

}

void SADataProcessVectorPointFXMLHelper::startWrite(QString *string)
{
    d_ptr->startWrite(string);
}

void SADataProcessVectorPointFXMLHelper::writeValue(const QVariant& d, const QString &name)
{
    if(d_ptr->xmlWriter.isNull())
    {
        return;
    }
    d_ptr->writeValue(d,name);
}


void SADataProcessVectorPointFXMLHelper::endWrite()
{
    if(d_ptr->xmlWriter.isNull())
    {
        return;
    }
    d_ptr->endWrite();
}

void SADataProcessVectorPointFXMLHelper::startWriteGroup(const QString &name)
{
    if(d_ptr->xmlWriter.isNull())
    {
        return;
    }
    d_ptr->startWriteGroup(name);
}

void SADataProcessVectorPointFXMLHelper::endWriteGroup()
{
    if(d_ptr->xmlWriter.isNull())
    {
        return;
    }
    d_ptr->endWriteGroup();
}

bool SADataProcessVectorPointFXMLHelper::read(const QString &xmlStr, SADataFeatureItem *item)
{
    bool isOK = false;
    QDomDocument doc;
    isOK = doc.setContent(xmlStr,false);
    if(!isOK)
    {
        return false;
    }
    QDomNodeList nodes = doc.elementsByTagName(SA_XML_TAG_DFI_ROOT);
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
                if(!getGroupInfo(groupItem,groupName))
                {
                    continue;
                }
                SADataFeatureItem* group = new SADataFeatureItem(groupName);
                if(!readChildItem(groupItem,group))
                {
                    delete group;
                }
                item->appendItem(group);
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

bool SADataProcessVectorPointFXMLHelper::readChildItem(const QDomNode &groupItem, SADataFeatureItem *parentItem)
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
            if(!getItemInfoFromElement(itemList.at(i),item))
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
            if(!getGroupInfo(itemList.at(i),groupName))
            {
                continue;
            }
            SADataFeatureItem* group = new SADataFeatureItem(groupName);
            if(!readChildItem(itemList.at(i),group))
            {
                delete group;
                continue;
            }
            parentItem->appendItem(group);

        }

    }
    return true;
}

bool SADataProcessVectorPointFXMLHelper::getItemInfoFromElement(const QDomNode &xmlItem, SADataFeatureItem *item)
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

bool SADataProcessVectorPointFXMLHelper::getGroupInfo(const QDomNode &xmlItem, QString &name)
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
    name = att.toAttr().value();
    return true;
}
