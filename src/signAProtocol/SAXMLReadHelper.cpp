#include "SAXMLReadHelper.h"
#include <QDomNodeList>
#include <QDomNamedNodeMap>
#include <QXmlStreamWriter>
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


//====================================


