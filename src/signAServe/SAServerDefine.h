#ifndef SASERVERDEFINE_H
#define SASERVERDEFINE_H

///
/// \file sa 服务的相关宏定义
///


///
/// \def 主程序的appid
///
#ifndef SA_SERVER_MAIN_APP_ID
#define SA_SERVER_MAIN_APP_ID "sa"
#endif

///
/// \def 参数默认分组
///
#ifndef SA_SERVER_VALUE_GROUP_SA_DEFAULT
#define SA_SERVER_VALUE_GROUP_SA_DEFAULT "@sa-default"
#endif

namespace SA {
/**
 * @brief 协议类型
 */
enum ServeProtocolClassType{
    ProtocolTypeUnknow = 0
    ,ProtocolTypeHeartbreat ///< 心跳协议
    ,ProtocolTypeXml ///< xml协议
};

enum ServeProtocolFunctionType{
    ProtocolFunUnknow = 0
    ,ProtocolFunReqHeartbreat //1
    ,ProtocolFunReplyHeartbreat //2
    ,ProtocolFunReqToken //3
    ,ProtocolFunReplyToken //4
    ,ProtocolFunReq2DPointsDescribe ///< 请求2维点序列的描述
};
}

#endif // SALOCALSERVERDEFINE_H
