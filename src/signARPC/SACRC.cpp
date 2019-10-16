#include "SACRC.h"
#include "3rdParty/CRC.h"
SACRC::SACRC()
{

}

quint16 SACRC::crc16(const char *input, size_t num_bytes)
{
    return CRC::Calculate(input,num_bytes,CRC::CRC_16_ARC());
}

quint16 SACRC::crc16(const QByteArray &data)
{
    return SACRC::crc16(data.constData(),data.size());
}

quint32 SACRC::crc32(const char *input, size_t num_bytes)
{
    return CRC::Calculate(input, num_bytes, CRC::CRC_32());
}

quint32 SACRC::crc32(const QByteArray &data)
{
    return CRC::Calculate(data.constData(), data.size(), CRC::CRC_32());
}

