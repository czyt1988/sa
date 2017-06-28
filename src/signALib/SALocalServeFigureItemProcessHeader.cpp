#include "SALocalServeFigureItemProcessHeader.h"
#include "SACRC.h"

qintptr SALocalServeFigureItemProcessHeader::getWndPtr() const
{
    return m_d.wndPtr;
}

void SALocalServeFigureItemProcessHeader::setWndPtr(const qintptr &wnd)
{
    m_d.wndPtr = wnd;
}

qintptr SALocalServeFigureItemProcessHeader::getItem() const
{
    return m_d.itemPtr;
}

void SALocalServeFigureItemProcessHeader::setItem(const qintptr &item)
{
    m_d.itemPtr = item;
}

int SALocalServeFigureItemProcessHeader::getDataType() const
{
    return m_d.dataType;
}

void SALocalServeFigureItemProcessHeader::setDataType(int dataType)
{
    m_d.dataType = dataType;
}

size_t SALocalServeFigureItemProcessHeader::getDataLength() const
{
    return m_d.dataLength;
}

void SALocalServeFigureItemProcessHeader::setDataLength(size_t dataLen)
{
    m_d.dataLength = dataLen;
}

size_t SALocalServeFigureItemProcessHeader::getDataVectorNum() const
{
    return m_d.dataVectorNum;
}

void SALocalServeFigureItemProcessHeader::setDataVectorNum(size_t dataNum)
{
    m_d.dataVectorNum = dataNum;
}

void SALocalServeFigureItemProcessHeader::write(QDataStream &io) const
{
    io << *this;
}

void SALocalServeFigureItemProcessHeader::read(QDataStream &io)
{
    io >> *this;
}


///
/// \brief 设置为有效，此操作会自动计算校验，对于接收的文件不需要调用，但对于发送的文件一定要再发送前调用
///
void SALocalServeFigureItemProcessHeader::refreshCheck()
{
    m_isValid = true;
    m_crc16 = calcCRC16();
}
///
/// \brief 计算crc
/// \return
///
unsigned short SALocalServeFigureItemProcessHeader::calcCRC16() const
{
    return SACRC::crc16((const unsigned char*)&m_d,sizeof(SALocalServeFigureItemProcessHeader::PrivateData));
}

bool SALocalServeFigureItemProcessHeader::isValid() const
{
    return m_isValid;
}
///
/// \brief 发送的字节数
/// \return
///
size_t SALocalServeFigureItemProcessHeader::sendSize()
{
    return sizeof(qintptr)
            +sizeof(qintptr)
            +sizeof(int)
            +sizeof(size_t)
            +sizeof(size_t)
            +sizeof(unsigned short)
            ;
}
QDataStream &operator <<(QDataStream &io, const SALocalServeFigureItemProcessHeader &d)
{
    io.writeRawData((const char*)(&(d.m_d.wndPtr)),sizeof(qintptr));
    io.writeRawData((const char*)(&(d.m_d.itemPtr)),sizeof(qintptr));
    io.writeRawData((const char*)(&(d.m_d.dataType)),sizeof(int));
    io.writeRawData((const char*)(&(d.m_d.dataLength)),sizeof(size_t));
    io.writeRawData((const char*)(&(d.m_d.dataVectorNum)),sizeof(size_t));
    io.writeRawData((const char*)(&(d.m_crc16)),sizeof(unsigned short));
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeFigureItemProcessHeader &d)
{
    io.readRawData((char*)(&(d.m_d.wndPtr)),sizeof(qintptr));
    io.readRawData((char*)(&(d.m_d.itemPtr)),sizeof(qintptr));
    io.readRawData((char*)(&(d.m_d.dataType)),sizeof(int));
    io.readRawData((char*)(&(d.m_d.dataLength)),sizeof(size_t));
    io.readRawData((char*)(&(d.m_d.dataVectorNum)),sizeof(size_t));
    io.readRawData((char*)(&(d.m_crc16)),sizeof(unsigned short));
    unsigned short crcTmp = d.calcCRC16();
    d.m_isValid = (crcTmp == d.m_crc16);
    return io;
}
