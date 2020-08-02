#ifndef SADATAPROCFUNCTIONS_H
#define SADATAPROCFUNCTIONS_H
#include <QFuture>
#include "SAXMLProtocolParser.h"
#include "SAProtocolHeader.h"

/**
 * @brief 通用函数
 *
 */

//生成一个应答xml
SAXMLProtocolParser xmlProtocol(int funid, const SAXMLProtocolParser& reqxml);

//根据请求头创建回复头
SAProtocolHeader createXMLReplyHeader(const SAProtocolHeader& requestHeader, const QByteArray& data, int Funid);

#endif // SADATAPROCFUNCTIONS_H
