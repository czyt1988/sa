#ifndef SADATAPROCFUNCTIONS_H
#define SADATAPROCFUNCTIONS_H
#include <QFuture>
#include "SAXMLProtocol.h"
#include "SAProtocolHeader.h"

/**
 * @brief 通用函数
 *
 */

//生成一个应答xml
SAXMLProtocol xmlProtocol(int funid, const SAXMLProtocol& reqxml);

//根据请求头创建回复头
SAProtocolHeader createXMLReplyHeader(const SAProtocolHeader& requestHeader, const QByteArray& data, int Funid);

#endif // SADATAPROCFUNCTIONS_H
