#ifndef SADATAPROCFUNCTIONS_H
#define SADATAPROCFUNCTIONS_H
#include <QFuture>
#include "SASocketHandle.h"
#include "SAProtocolHeader.h"
/**
  * @brief 通用函数
  *
 */

SASocketHandle::XMLDataPtr createXMLDataPtr(int funid);
SASocketHandle::XMLDataPtr createXMLDataPtr(int funid,SASocketHandle::XMLDataPtr reqxml);
//根据请求头创建回复头
SAProtocolHeader createXMLReplyHeader(const SAProtocolHeader &requestHeader,const QByteArray& data,int Funid);

#endif // SADATAPROCFUNCTIONS_H
