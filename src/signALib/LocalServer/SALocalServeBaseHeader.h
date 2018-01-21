#ifndef SALOCALSERVEBASEDATA_H
#define SALOCALSERVEBASEDATA_H
#include "SALibGlobal.h"
#include <QDataStream>
#define SA_LOCAL_SER_HEADER_MAGIC_START (0xddf125a)
#define SA_LOCAL_SER_HEADER_MAGIC_END (0xa521fdd)
class QString;
///
/// \brief 用于sa local serve传递的表头数据结构
///
struct SALIB_EXPORT SALocalServeBaseHeader
{
    uint magic_start;///< 开始魔数，理论恒等于 \sa SA_LOCAL_SER_HEADER_MAGIC_START
    uint key;///< key 字段，由于标识有序数据包，默认为0
    int type;///< 数据包分类
    size_t dataSize;///< 标记数据包的尺寸
    uint magic_end;///< 结束魔数，理论恒等于 \sa SA_LOCAL_SER_HEADER_MAGIC_END
    void init();
    bool isValid() const;
};

SALIB_EXPORT QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
SALIB_EXPORT QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);

Q_DECLARE_METATYPE(SALocalServeBaseHeader)

#endif // SALOCALSERVEXMLDATA_H
