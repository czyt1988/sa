#include "SALocalServeFigureItemProcessHeader.h"


qintptr SALocalServeFigureItemProcessHeader::getWndPtr() const
{
    return m_wndPtr;
}

void SALocalServeFigureItemProcessHeader::setWndPtr(const qintptr &wnd)
{
    m_wndPtr = wnd;
}

qintptr SALocalServeFigureItemProcessHeader::getItem() const
{
    return m_itemPtr;
}

void SALocalServeFigureItemProcessHeader::setItem(const qintptr &item)
{
    m_itemPtr = item;
}

int SALocalServeFigureItemProcessHeader::getDataType() const
{
    return m_dataType;
}

void SALocalServeFigureItemProcessHeader::setDataType(int dataType)
{
    m_dataType = dataType;
}

void SALocalServeFigureItemProcessHeader::write(QDataStream &io) const
{
    io << *this;
}

void SALocalServeFigureItemProcessHeader::read(QDataStream &io)
{
    io >> *this;
}

QDataStream &operator <<(QDataStream &io, const SALocalServeFigureItemProcessHeader &d)
{
    io.writeRawData((const char*)(&d.m_wndPtr),sizeof(qintptr));
    io.writeRawData((const char*)(&d.m_itemPtr),sizeof(qintptr));
    io.writeRawData((const char*)(&d.m_dataType),sizeof(int));
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeFigureItemProcessHeader &d)
{
    io.readRawData((char*)(&d.m_wndPtr),sizeof(qintptr));
    io.readRawData((char*)(&d.m_itemPtr),sizeof(qintptr));
    io.readRawData((char*)(&d.m_dataType),sizeof(int));
    return io;
}
