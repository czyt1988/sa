#ifndef SASERVEHANDLEFUN_H
#define SASERVEHANDLEFUN_H
#include <functional>
#include <QVariantHash>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
#include "SATcpSocket.h"
#include "SAXMLProtocol.h"
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
SASERVE_EXPORT bool cast_protocol_to_satree(const SAXMLProtocol *xml, SATree *tree);

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
    , const SAXMLProtocol *xml
    , int funid
    , int sequenceID = 0
    , uint32_t extendValue = 0);


/////////////////////////////////////////////////////////////
///
///  xml协议相关函数
///  每个协议一共涉及4个函数 分别是request_XX_xml,receive_request_XX_xml,reply_XX_xml,receive_reply_XX_xml
///  request_XX_xml 是请求协议
///  receive_request_XX_xml 是针对request_XX_xml的xml解析
///  reply_XX_xml 是回复协议
///  receive_reply_XX_xml 是针对reply_XX_xml的xml解析
///
///
/////////////////////////////////////////////////////////////

//回复异常描述
SASERVE_EXPORT bool reply_error_xml(SATcpSocket *socket
    , const SAProtocolHeader& requestHeader
    , const QString& msg
    , int errcode);

//回复异常描述
SASERVE_EXPORT bool reply_error_xml(SATcpSocket *socket
    , int sequenceID
    , int extendValue
    , const QString& msg
    , int errcode);

//接收到错误信息
SASERVE_EXPORT bool receive_error_xml(const SAXMLProtocol *xml
    , QString& msg
    , int& errcode);

//客户端-发出token请求
SASERVE_EXPORT bool request_token_xml(int pid
    , const QString& appid
    , SATcpSocket *socket
    , int sequenceID = 0
    , uint32_t extendValue = 0);

//解析token请求参数
SASERVE_EXPORT bool receive_request_token_xml(const SAXMLProtocol *xml
    , int& pid
    , QString& appid);

//处理token请求
SASERVE_EXPORT bool reply_token_xml(SATcpSocket *socket
    , const SAProtocolHeader& header
    , int pid
    , const QString& appid);

//解析token返回参数
SASERVE_EXPORT bool receive_reply_token_xml(const SAXMLProtocol *xml
    , QString& token);

//请求心跳
SASERVE_EXPORT bool request_heartbreat(SATcpSocket *socket);

//处理心跳请求
SASERVE_EXPORT bool reply_heartbreat_xml(SATcpSocket *socket
    , const SAProtocolHeader& recheader);

//////////////////////
//请求2维数组描述
/////////////////////
SASERVE_EXPORT bool request_2d_points_describe_xml(SATcpSocket *socket
    , const QVector<QPointF>& arrs
    , int sequenceID
    , int sortcount = 20);

//
SASERVE_EXPORT bool receive_request_2d_points_describe_xml(const SAXMLProtocol *xml
    , QVector<QPointF>& arrs
    , int& sortcount);

//回复2维数组描述
SASERVE_EXPORT bool reply_2d_points_describe_xml(SATcpSocket *socket
    , const SAProtocolHeader& requestHeader
    , unsigned int count
    , double sum
    , double mean
    , double var
    , double stdVar
    , double skewness
    , double kurtosis
    , double min
    , double max
    , double mid
    , double peak2peak
    , const QPointF& minPoint
    , const QPointF& maxPoint
    , const QPointF& midPoint
    , const QVector<QPointF>& tops
    , const QVector<QPointF>& lows);

//解析2维数组描述的回复
SASERVE_EXPORT bool receive_reply_2d_points_describe_xml(const SAXMLProtocol *xml
    , unsigned int& count
    , double& sum
    , double& mean
    , double& var
    , double& stdVar
    , double& skewness
    , double& kurtosis
    , double& min
    , double& max
    , double& mid
    , double& peak2peak
    , QPointF& minPoint
    , QPointF& maxPoint
    , QPointF& midPoint
    , QVector<QPointF>& tops
    , QVector<QPointF>& lows);
}


#endif // SATCPHANDLEFUN_H
