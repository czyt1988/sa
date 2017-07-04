#include "SAXMLReadHelper.h"
#include "SAXMLTagDefined.h"
#include <QXmlStreamReader>
#include "SADataFeatureItem.h"
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
                    if(SA_XML_ATT_NAME_SA_TYPE == atts[i].name())//取属性 type=“xx”
                    {
                        QString typeStr = atts[i].value().toString();
                        if(SA_XML_ATT_SA_TYPE_VPFR == typeStr)
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
    return item->fromXml(*m_strPtr);
}

bool SAXMLReadHelper::isValid() const
{
    return m_isvalid;
}

