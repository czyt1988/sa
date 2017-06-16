#include "SALocalServeBaseData.h"
SALocalServeBaseData::SALocalServeBaseData()
{

}

uint SALocalServeBaseData::getSendedPid() const
{
    return m_pid;
}

void SALocalServeBaseData::setSendedPid(const uint &pid)
{
    m_pid = pid;
}

uint SALocalServeBaseData::getKey() const
{
    return m_key;
}

void SALocalServeBaseData::setKey(const uint &key)
{
    m_key = key;
}

int SALocalServeBaseData::getType() const
{
    return m_type;
}

void SALocalServeBaseData::setType(int type)
{
    m_type = type;
}

void SALocalServeBaseData::write(QDataStream &io) const
{
    io << *this;
}

void SALocalServeBaseData::read(QDataStream &io)
{
    io >> *this;
}


//===========================================================

QDataStream &operator <<(QDataStream &io, const SALocalServeBaseData &d)
{
    io<<d.getKey()<<d.getSendedPid()<<d.getType();
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeBaseData &d)
{
    io >> d.m_key >> d.m_pid >> d.m_type;
    return io;
}
