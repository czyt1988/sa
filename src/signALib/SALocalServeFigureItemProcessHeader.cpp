#include "SALocalServeFigureItemProcessHeader.h"

SALocalServeFigureItemProcessHeader::SALocalServeFigureItemProcessHeader()
    :m_wndPtr(0)
    ,m_itemPtr(0)
    ,m_dataType(0)
{

}

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
    io << d.m_wndPtr << d.m_itemPtr << d.m_dataType;
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeFigureItemProcessHeader &d)
{
    io >> d.m_wndPtr >> d.m_itemPtr >> d.m_dataType;
    return io;
}
