#ifndef SACRC_H
#define SACRC_H
#include <QByteArray>
#include "SARPCGlobal.h"
class SARPC_EXPORT SACRC
{
public:
    SACRC();
    static quint16 crc16(const char* input, size_t num_bytes);
    static quint16 crc16(const QByteArray& data);
    static quint32 crc32(const char* input, size_t num_bytes);
    static quint32 crc32(const QByteArray& data);
};

#endif // QCRC_H
