#ifndef SASERVEHANDLEFUN_H
#define SASERVEHANDLEFUN_H
#include <functional>
#include <QVariantHash>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
#include "SATcpSocket.h"
#include "SAXMLProtocolParser.h"

/**
  *
  * @brief 根据classid和funid来实现请求处理的函数库
  *
  * 所有请求的函数形式为bool(const SAProtocolHeader &,const QByteArray &,SATcpSocket*,QVariantHash*)
  *
 */
namespace SA {
//定义函数句柄
typedef std::function<bool(const SAProtocolHeader &,const QByteArray &,SATcpSocket*,QVariantHash*)> FunHandle;



//确保写入socket的函数，未能发完会从新接着上次的点发
SASERVE_EXPORT bool ensure_write(const char *data, qint64 len, SATcpSocket* socket, short maxtry=100);
//确保写入socket的函数，未能发完会从新接着上次的点发
SASERVE_EXPORT bool ensure_write(const QByteArray &data,SATcpSocket* socket, short maxtry=100);
//写socket
SASERVE_EXPORT bool write(const SAProtocolHeader &header, const QByteArray &data,SATcpSocket* socket);
//根据pid和appid创建token
SASERVE_EXPORT QString make_token(int pid,const QString& appID);
//写xml协议
SASERVE_EXPORT bool write_xml_protocol(SATcpSocket* socket,const SAXMLProtocolParser *xml
                                       ,int funid , int sequenceID=0, uint32_t extendValue=0);

/////////////////////////////////////////////////////////////
/// 
///  注册函数
/// 
/////////////////////////////////////////////////////////////

// 注册处理函数 - 线程安全
SASERVE_EXPORT void register_serve_funciton_handle(int funid,FunHandle fun);
//获取处理方法 - 线程安全
SASERVE_EXPORT FunHandle get_serve_funciton_handle(int funid);
//初始化 - 线程安全
SASERVE_EXPORT void init_serve_funciotn_handle();
/////////////////////////////////////////////////////////////
/// 
///  xml协议的处理函数
/// 
/////////////////////////////////////////////////////////////

//客户端-发出token请求
SASERVE_EXPORT bool request_token_xml(int pid
                                      , const QString& appid
                                      , SATcpSocket* socket
                                      , int sequenceID = 0, uint32_t extendValue = 0);
//服务端处理请求token的命令
SASERVE_EXPORT bool deal_xml_request_token(const SAProtocolHeader &header
                                           , const QByteArray &data
                                           , SATcpSocket* socket
                                           , QVariantHash* res = nullptr);
//客户端处理服务端发回的token请求
SASERVE_EXPORT bool deal_xml_reply_token(const SAProtocolHeader &header
                                         , const QByteArray &data
                                         , SATcpSocket* socket
                                         , QVariantHash* res);
//请求心跳
SASERVE_EXPORT bool request_heartbreat(SATcpSocket* socket);
//服务端 - 处理心跳请求
SASERVE_EXPORT bool deal_request_heartbreat(const SAProtocolHeader &header
                                         , const QByteArray &data
                                         , SATcpSocket* socket
                                         , QVariantHash* res);


}


#endif // SATCPHANDLEFUN_H
