#include "SALocalServeBaseHeader.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>


SALocalServeBaseHeader::SALocalServeBaseHeader()
{
    memset(this,0,sizeof(SALocalServeBaseHeader));
}
///
/// \brief 获取发送端进程的pid
/// \return
/// \see setSendedPid
///
uint SALocalServeBaseHeader::getSendedPid() const
{
    return m_pid;
}
///
/// \brief 设置发送端进程的pid
/// \param pid
/// \see getSendedPid
///
void SALocalServeBaseHeader::setSendedPid(const uint &pid)
{
    m_pid = pid;
}
///
/// \brief 标识
/// \return 标识
/// \see setKey
///
uint SALocalServeBaseHeader::getKey() const
{
    return m_key;
}
///
/// \brief 设置标识
/// \param key 标识
/// \see getKey
///
void SALocalServeBaseHeader::setKey(const uint &key)
{
    m_key = key;
}
///
/// \brief 包类型，见SALocalServeBaseHeader::Type
/// \return
/// \see SALocalServeBaseHeader::Type setType
///
int SALocalServeBaseHeader::getType() const
{
    return m_type;
}
///
/// \brief 设置包类型，见SALocalServeBaseHeader::Type
/// \param type SALocalServeBaseHeader::Type
/// \see SALocalServeBaseHeader::Type getType
///
void SALocalServeBaseHeader::setType(int type)
{
    m_type = type;
}

///
/// \brief 包头后数据的大小
/// \return
/// \see setDataSize
///
size_t SALocalServeBaseHeader::getDataSize() const
{
    return m_dataSize;
}
///
/// \brief 设置数据区的大小
/// \param dataSize
/// \see getDataSize
///
void SALocalServeBaseHeader::setDataSize(size_t dataSize)
{
    m_dataSize = dataSize;
}
///
/// \brief 从xml中生成包数据
/// \param xml
/// \return
/// \todo 移除此函数，放置在别处
///
bool SALocalServeBaseHeader::fromXML(QXmlStreamReader* xml)
{
    while(!xml->atEnd() && !xml->hasError())
    {
        xml->readNext();
        if(xml->isStartElement())
        {
            if(xml->name() == SA_XML_LOCALSERVE_ROOT)
            {
                xml->readNext();
                if(xml->name() == SA_XML_LOCALSERVE_HEADER_KEY)
                {
                    m_key = xml->text().toUInt();
                }
                else if(xml->name() == SA_XML_LOCALSERVE_HEADER_TYPE)
                {
                    m_type = xml->text().toInt();
                }
                else if(xml->name() == SA_XML_LOCALSERVE_HEADER_PID)
                {
                    m_pid = xml->text().toInt();
                }
            }
        }
    }
}
///
/// \brief 把包数据写为xml
/// \return
/// \todo 移除此函数
///
QString SALocalServeBaseHeader::toXML() const
{
    QString str;
    QXmlStreamWriter xml(&str);
    initXmlStart(&xml);
    writeXMLHeader(&xml);
    xml.writeEndElement();
}
///
/// \brief 写入内存
/// \param io
///
void SALocalServeBaseHeader::write(QDataStream &io) const
{
    io << (*this);
}
///
/// \brief 从内存读取
/// \param io
///
void SALocalServeBaseHeader::read(QDataStream &io)
{
    io >> (*this);
}




///
/// \brief 初始化xml，此时xml的节点位于<root>，最后要调用xml.writeEndElement();
/// \param xml
///
void SALocalServeBaseHeader::initXmlStart(QXmlStreamWriter *xml) const
{
    xml->setCodec("UTF-8");//设置编码
    xml->setAutoFormatting(true);//自动换行，否则会写成一坨不好看
    xml->setAutoFormattingIndent(2);//设置一个tab的空格数，也就是缩进量，一般是2或4
    xml->writeStartDocument();//写入xml文档头<?xml version="1.0" encoding="UTF-8"?>
    xml->writeStartElement(SA_XML_LOCALSERVE_ROOT);//每个start ele都要有write en
}

void SALocalServeBaseHeader::writeXMLHeader(QXmlStreamWriter *xml) const
{
    xml->writeStartElement(SA_XML_LOCALSERVE_HEADER);
    xml->writeTextElement(SA_XML_LOCALSERVE_HEADER_KEY,QString::number(m_key));
    xml->writeTextElement(SA_XML_LOCALSERVE_HEADER_TYPE,QString::number(m_type));
    xml->writeTextElement(SA_XML_LOCALSERVE_HEADER_PID,QString::number(m_pid));
    xml->writeEndElement();
}





//===========================================================

QDataStream &operator <<(QDataStream &io, const SALocalServeBaseHeader &d)
{
    //io<<d.getKey()<<d.getSendedPid()<<d.getType()<<d.getDataSize();
    io.writeRawData((const char*)(&d.m_key),sizeof(uint));
    io.writeRawData((const char*)(&d.m_pid),sizeof(uint));
    io.writeRawData((const char*)(&d.m_type),sizeof(int));
    io.writeRawData((const char*)(&d.m_dataSize),sizeof(size_t));
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeBaseHeader &d)
{
    //io >> d.m_key >> d.m_pid >> d.m_type >> d.m_dataSize;
    io.readRawData((char*)(&d.m_key),sizeof(qintptr));
    io.readRawData((char*)(&d.m_pid),sizeof(qintptr));
    io.readRawData((char*)(&d.m_type),sizeof(int));
    io.readRawData((char*)(&d.m_dataSize),sizeof(int));
    return io;
}
