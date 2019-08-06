#ifndef SALOCALSERVEBASEDATA_H
#define SALOCALSERVEBASEDATA_H
#include "SALibGlobal.h"
#include <QDataStream>
#include <QDebug>
/// \def 定义魔数头
#ifndef SA_LOCAL_SER_HEADER_MAGIC_START
#define SA_LOCAL_SER_HEADER_MAGIC_START (0xddf125a)
#endif
/// \def 定义魔数尾
#ifndef SA_LOCAL_SER_HEADER_MAGIC_END
#define SA_LOCAL_SER_HEADER_MAGIC_END (0xa521fdd)
#endif

/// \def 登录类型
#define SA_LOCAL_SER_LOGIN_TYPE 1
/// \def 登录请求(1-1)类号
#define SA_LOCAL_SER_LOGIN_CLASS 1
/// \def 登录请求(1-1)功能号
#define SA_LOCAL_SER_LOGIN_FUN 1

/// \def 握手类型
#define SA_LOCAL_SER_SHAKEHAND_TYPE 2
/// \def 握手请求(2-1)类号
#define SA_LOCAL_SER_SHAKEHAND_CLASS 2
/// \def 握手请求(2-1)功能号
#define SA_LOCAL_SER_SHAKEHAND_FUN 1

/// \def 数据类型
#define SA_LOCAL_SER_PARAM_TYPE (3)
/// \def 变量类型
#define SA_LOCAL_SER_PARAM_CLASS 3
/// \def 字符串类3-1
#define SA_LOCAL_SER_STRING_FUN 1
/// \def 2D序列类型
#define SA_LOCAL_SER_2DPOINTF_FUN 2
/// \def 二进制类型
#define SA_LOCAL_SER_BIN_FUN 99

/// \def 错误类型
#define SA_LOCAL_SER_ERROR_TYPE 4
/// \def 错误类
#define SA_LOCAL_SER_ERROR_CLASS 4
/// \def 普通错误4-1
#define SA_LOCAL_SER_ERROR_NORMAL_FUN 1

namespace SALocalServe {
    /**
     * @brief 标记local的错误类型
     */
    enum SALocalServeError{
        Unknow ///< 未知错误
        ,ReceiveDataError ///< 接收的数据有误
        ,ReceiveUnknowHeader ///< 接收到未知的文件头
    };
}


class QString;
///
/// \brief 用于sa local serve传递的表头数据结构
///
struct SALIB_EXPORT SALocalServeBaseHeader
{
    uint magic_start;///< 开始魔数，理论恒等于 \sa SA_LOCAL_SER_HEADER_MAGIC_START
    uint key;///< key 字段，用于标识有序数据包，默认为0
    int type;///< 数据包分类
    uint tokenID;///< 记录tokenID
    int classID;///< 类型号
    int functionID;///< 功能号
    uint32_t dataSize;///< 标记数据包的尺寸
    uint32_t dataCrc32;///< 标记数据区的crc32值
    uint magic_end;///< 结束魔数，理论恒等于 \sa SA_LOCAL_SER_HEADER_MAGIC_END
    void init();
    bool isValid() const;
};

SALIB_EXPORT QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
SALIB_EXPORT QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);
SALIB_EXPORT QDebug operator<<(QDebug debug, const SALocalServeBaseHeader &c);


Q_DECLARE_METATYPE(SALocalServeBaseHeader)




#endif // SALOCALSERVEXMLDATA_H
