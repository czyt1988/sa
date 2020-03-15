#ifndef SASERVEHANDLEFUN_H
#define SASERVEHANDLEFUN_H
#include <functional>
#include <QVariantHash>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
#include "SATcpSocket.h"
#include "SAXMLProtocolParser.h"
class QObject;
/**
  *
  * @brief 根据classid和funid来实现请求处理的函数库
  *
  * 所有请求的函数形式为bool(const SAProtocolHeader &,const QByteArray &,SATcpSocket*,QVariantHash*)
  *
  * 任何进程需要先进行初始化：@see init_serve_funciotn_handle,通过注册函数可以注入一些外部的处理函数@register_serve_funciton_handle
  * 大部分的特殊处理函数不会注册，需要继承SATcpSection或SATcpClient来进行处理
  *
 */
namespace SA {
//定义函数句柄
typedef std::function<bool(const SAProtocolHeader &,const QByteArray &,SATcpSocket*,QObject*)> FunHandle;



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
SASERVE_EXPORT void register_serve_funciton_handle(int classid,int funid,FunHandle fun);
//获取处理方法 - 线程安全
SASERVE_EXPORT FunHandle get_serve_funciton_handle(int classid,int funid);
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

//请求心跳
SASERVE_EXPORT bool request_heartbreat(SATcpSocket* socket);
}


#endif // SATCPHANDLEFUN_H
