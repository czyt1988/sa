#ifndef QCRC_H
#define QCRC_H
#include <QtGlobal>
#include <QByteArray>
#include "SALibGlobal.h"
/**
 * @brief 基于CRCpp封装的crc算法
 */
class SALIB_EXPORT QCrc
{
public:
    QCrc();
    static quint32 crc32(const char* p,uint len);
    static quint32 crc32(const QByteArray& bytearr);
};

#endif // QCRC_H
