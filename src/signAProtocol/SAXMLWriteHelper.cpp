#include "SAXMLWriteHelper.h"
#include "SAXMLTagDefined.h"
#include "SAVariantCaster.h"
SAXMLWriteHelper::SAXMLWriteHelper(const QString& type)
    :m_doc("SA_XML")
{
    m_rootEle = m_doc.createElement(SA_XML_TAG_SA);
    m_rootEle.setAttribute(SA_XML_ATT_TYPE,type);
    m_doc.appendChild(m_rootEle);
    m_headerEle = m_doc.createElement(SA_XML_TAG_HEADER_ROOT);
    m_rootEle.appendChild(m_headerEle);
    m_contentEle = m_doc.createElement(SA_XML_TAG_CONTENT_ROOT);
    m_rootEle.appendChild(m_contentEle);
}

SAXMLWriteHelper::~SAXMLWriteHelper()
{

}

void SAXMLWriteHelper::startHeadWriteGroup(const QString &name)
{
    m_tmpHeadEle = m_doc.createElement(name);
    m_rootEle.appendChild(m_tmpHeadEle);
}

void SAXMLWriteHelper::endHeadWriteGroup()
{
    m_tmpHeadEle = QDomElement();
}

void SAXMLWriteHelper::writeHeadValue(const QVariant &d, const QString &name)
{
    QDomElement ei;
    ei = m_doc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VALUE);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setAttribute(SA_XML_ATT_VALUE_TYPE,d.typeName());
    ei.setAttribute(SA_XML_ATT_VALUE,SAVariantCaster::variantToString(d));
    if(m_tmpHeadEle.isNull())
    {
        m_headerEle.appendChild(ei);
    }
    else
    {
        m_tmpHeadEle.appendChild(ei);
    }
}

void SAXMLWriteHelper::startContentWriteGroup(const QString &name)
{
    m_tmpContentEle = m_doc.createElement(name);
    m_rootEle.appendChild(m_tmpContentEle);
}

void SAXMLWriteHelper::endContentWriteGroup()
{
    m_tmpContentEle = QDomElement();
}

void SAXMLWriteHelper::writeContentValue(const QVariant &d, const QString &name)
{
    QDomElement ei;
    ei = m_doc.createElement(SA_XML_TAG_ITEM);
    ei.setAttribute(SA_XML_ATT_TYPE,ATT_TYPE_VALUE);
    ei.setAttribute(SA_XML_ATT_NAME,name);
    ei.setAttribute(SA_XML_ATT_VALUE_TYPE,d.typeName());
    ei.setAttribute(SA_XML_ATT_VALUE,SAVariantCaster::variantToString(d));
    if(m_tmpContentEle.isNull())
    {
        m_contentEle.appendChild(ei);
    }
    else
    {
        m_tmpContentEle.appendChild(ei);
    }
}

QString SAXMLWriteHelper::toString(int indent) const
{
    return m_doc.toString(indent);
}
