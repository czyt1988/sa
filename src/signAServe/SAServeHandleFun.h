#ifndef SASERVEHANDLEFUN_H
#define SASERVEHANDLEFUN_H
#include <functional>
#include <QVariantHash>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
#include "SATcpSocket.h"
#include "SAXMLProtocolParser.h"
#include "SATree.h"
class QObject;

/**
 *
 * @brief 根据classid和funid来实现请求处理的函数库
 *
 *
 */
namespace SA {
/////////////////////////////////////////////////////////////
///
///  其他相关函数
///
/////////////////////////////////////////////////////////////

//把SAXMLProtocolParser 转换为tree
SASERVE_EXPORT bool cast_protocol_to_satree(const SAXMLProtocolParser *xml, SATree *tree);

//把satree 转换为标准xml
SASERVE_EXPORT QString cast_satree_to_xml(const SATree *tree);

//确保写入socket的函数，未能发完会从新接着上次的点发
SASERVE_EXPORT bool ensure_write(const char *data
    , qint64 len
    , SATcpSocket *socket
    , short maxtry = 100);

//确保写入socket的函数，未能发完会从新接着上次的点发
SASERVE_EXPORT bool ensure_write(const QByteArray& data
    , SATcpSocket *socket
    , short maxtry = 100);

//写socket
SASERVE_EXPORT bool write(const SAProtocolHeader& header
    , const QByteArray& data
    , SATcpSocket *socket);

//根据pid和appid创建token
SASERVE_EXPORT QString make_token(int pid
    , const QString& appID);

//写xml协议
SASERVE_EXPORT bool write_xml_protocol(SATcpSocket *socket
    , const SAXMLProtocolParser *xml
    , int funid
    , int sequenceID = 0
    , uint32_t extendValue = 0);


/////////////////////////////////////////////////////////////
///
///  xml协议的处理函数
///
/////////////////////////////////////////////////////////////

//客户端-发出token请求
SASERVE_EXPORT bool request_token_xml(int pid
    , const QString& appid
    , SATcpSocket *socket
    , int sequenceID = 0
    , uint32_t extendValue = 0);

//请求心跳
SASERVE_EXPORT bool request_heartbreat(SATcpSocket *socket);

/////////////////////////////////////////////////////////////
///
///  xml协议的响应函数
///
/////////////////////////////////////////////////////////////

//处理token请求
SASERVE_EXPORT bool reply_token_xml(SATcpSocket *socket
    , const SAProtocolHeader& header
    , int pid
    , const QString& appid);

//处理心跳请求
SASERVE_EXPORT bool reply_heartbreat_xml(SATcpSocket *socket, const SAProtocolHeader& header);

}


#endif // SATCPHANDLEFUN_H
