#ifndef SACRC_H
#define SACRC_H
#include <QByteArray>
#include "SALibGlobal.h"
class SALIB_EXPORT SACRC
{
public:
    SACRC();
    static unsigned short crc16(const unsigned char* input_str
                                ,unsigned int num_bytes);
    static unsigned short crc16(const QByteArray& data);
private:
    static void initCRC16Table();
};

#endif // QCRC_H
