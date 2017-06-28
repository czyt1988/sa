#include "SALocalServeBaseHeader.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "SACRC.h"

SALocalServeBaseHeader::SALocalServeBaseHeader()
{
    memset(this,0,sizeof(SALocalServeBaseHeader));
}

///
/// \brief 标识
/// \return 标识
/// \see setKey
///
uint SALocalServeBaseHeader::getKey() const
{
    return m_d.key;
}
///
/// \brief 设置标识
/// \param key 标识
/// \see getKey
///
void SALocalServeBaseHeader::setKey(const uint &key)
{
    m_d.key = key;
}
///
/// \brief 包类型，见SALocalServeBaseHeader::Type
/// \return
/// \see SALocalServeBaseHeader::Type setType
///
int SALocalServeBaseHeader::getType() const
{
    return m_d.type;
}
///
/// \brief 设置包类型，见SALocalServeBaseHeader::Type
/// \param type SALocalServeBaseHeader::Type
/// \see SALocalServeBaseHeader::Type getType
///
void SALocalServeBaseHeader::setType(int type)
{
    m_d.type = type;
}

///
/// \brief 包头后数据的大小
/// \return
/// \see setDataSize
///
size_t SALocalServeBaseHeader::getDataSize() const
{
    return m_d.dataSize;
}
///
/// \brief 设置数据区的大小
/// \param dataSize
/// \see getDataSize
///
void SALocalServeBaseHeader::setDataSize(size_t dataSize)
{
    m_d.dataSize = dataSize;
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
                    m_d.key = xml->text().toUInt();
                }
                else if(xml->name() == SA_XML_LOCALSERVE_HEADER_TYPE)
                {
                    m_d.type = xml->text().toInt();
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
    xml->writeTextElement(SA_XML_LOCALSERVE_HEADER_KEY,QString::number(m_d.key));
    xml->writeTextElement(SA_XML_LOCALSERVE_HEADER_TYPE,QString::number(m_d.type));
    xml->writeEndElement();
}
///
/// \brief 计算内部数据的crc16值
/// \return
///
unsigned short SALocalServeBaseHeader::calcCRC16() const
{
    return SACRC::crc16((const unsigned char*)&m_d,sizeof(SALocalServeBaseHeader::PrivateData));
}

size_t SALocalServeBaseHeader::sendSize()
{
    return sizeof(uint)+sizeof(int)+sizeof(size_t)+sizeof(unsigned short);
}

bool SALocalServeBaseHeader::isValid() const
{
    return m_isValid;
}

void SALocalServeBaseHeader::refreshCheck()
{
    m_isValid = true;
    m_crc16 = calcCRC16();
}





//===========================================================

QDataStream &operator <<(QDataStream &io, const SALocalServeBaseHeader &d)
{
    io.writeRawData((const char*)(&(d.m_d.key)),sizeof(uint));
    io.writeRawData((const char*)(&(d.m_d.type)),sizeof(int));
    io.writeRawData((const char*)(&(d.m_d.dataSize)),sizeof(size_t));
    io.writeRawData((const char*)(&(d.m_crc16)),sizeof(unsigned short));
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeBaseHeader &d)
{
    io.readRawData((char*)(&(d.m_d.key)),sizeof(uint));
    io.readRawData((char*)(&(d.m_d.type)),sizeof(int));
    io.readRawData((char*)(&(d.m_d.dataSize)),sizeof(size_t));
    io.readRawData((char*)(&(d.m_crc16)),sizeof(unsigned short));
    unsigned short crcTmp = d.calcCRC16();
    d.m_isValid = (crcTmp == d.m_crc16);
    return io;
}
