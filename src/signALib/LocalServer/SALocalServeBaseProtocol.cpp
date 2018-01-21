#include "SALocalServeBaseProtocol.h"

SALocalServeBaseProtocol::SALocalServeBaseProtocol()
{

}

SALocalServeBaseProtocol::~SALocalServeBaseProtocol()
{

}


void SALocalServeShakeHandProtocol::serializeIn(QDataStream &st)
{
    SALocalServeBaseHeader header;
    st >> header;
    if(header.isValid())
    {

    }
}

void SALocalServeShakeHandProtocol::serializeOut(QDataStream &st) const
{

}
