#ifndef SASERVERDEFINE_H
#define SASERVERDEFINE_H
///
/// \file sa 服务的相关宏定义
///


///
/// \def 主程序的appid
///
#ifndef SA_SERVER_MAIN_APP_ID
#define SA_SERVER_MAIN_APP_ID    "sa"
#endif

///
/// \def 参数默认分组
///
#ifndef SA_SERVER_VALUE_GROUP_SA_DEFAULT
#define SA_SERVER_VALUE_GROUP_SA_DEFAULT    "@sa-default"
#endif

namespace SA {
/**
 * @brief 协议类型
 */
enum ServeProtocolClassType {
    ProtocolTypeUnknow = 0
    , ProtocolTypeHeartbreat        ///< 心跳协议
    , ProtocolTypeXml               ///< xml协议
};

/**
 * @brief 定义功能id
 */
enum ServeProtocolFunctionType {
    ProtocolFunUnknow	= 0
    , ProtocolFunErrorOcc	= 1             //1 错误
    , ProtocolFunReqHeartbreat              //2 心跳请求
    , ProtocolFunReplyHeartbreat            //3 心跳回复
    , ProtocolFunReqToken                   //4 token请求
    , ProtocolFunReplyToken                 //5 token回复

    , ProtocolFunReq2DPointsDescribe        ///< 6 请求2维点序列的描述
    , ProtocolFunReply2DPointsDescribe      ///< 7 回复2维点序列的描述
};

/**
 * @brief 描述交互错误码
 */
enum ServeProtocolErrorCode {
    ProtocolErrorUnknow = 0         ///< 位置错误
    , ProtocolErrorUnknowFun        ///< 位置功能id
    , ProtocolErrorContent          ///< 协议内容错误
};
}

#endif // SALOCALSERVERDEFINE_H
