#include "QCrc.h"
#include "CRC.h"

QCrc::QCrc()
{

}

quint32 QCrc::crc32(const char *p, uint len)
{
    return CRC::Calculate(p, len, CRC::CRC_32());
}

quint32 QCrc::crc32(const QByteArray &bytearr)
{
    return CRC::Calculate(bytearr.constData(), bytearr.size(), CRC::CRC_32());
}

