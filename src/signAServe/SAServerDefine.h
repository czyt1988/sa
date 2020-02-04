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
/// \def token相关的请求
///
#ifndef SA_SERVER_CLASS_TOKEN
#define SA_SERVER_CLASS_TOKEN 0x20
#endif
///
/// \def 请求token
///
#ifndef SA_SERVER_FUN_TOKEN_REQ
#define SA_SERVER_FUN_TOKEN_REQ 0x1
#endif
///
/// \def 请求token的返回
///
#ifndef SA_SERVER_FUN_TOKEN_REPLY
#define SA_SERVER_FUN_TOKEN_REPLY 0x2
#endif
///
/// \def 参数默认分组
///
#ifndef SA_SERVER_VALUE_GROUP_SA_DEFAULT
#define SA_SERVER_VALUE_GROUP_SA_DEFAULT "@sa-default"
#endif
#endif // SALOCALSERVERDEFINE_H
