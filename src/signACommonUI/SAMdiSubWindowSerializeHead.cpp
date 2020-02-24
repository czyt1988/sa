#include "SAMdiSubWindowSerializeHead.h"
#define MAGIC_NUM (0xF123AB02)

bool SAMdiSubWindowSerializeHead::isValid() const
{
    return MAGIC_NUM == param.magic1;
}

void SAMdiSubWindowSerializeHead::init()
{
    memset(&param,0,sizeof(SAMdiSubWindowSerializeHead));
    param.magic1 = MAGIC_NUM;
}

QDataStream &operator <<(QDataStream &out, const SAMdiSubWindowSerializeHead &p)
{
    out.writeRawData(p.raw,SA_SUB_WINDOW_HEADER_LENGTH);
    return out;
}

QDataStream &operator >>(QDataStream &in, SAMdiSubWindowSerializeHead &p)
{
    in.readRawData(p.raw,SA_SUB_WINDOW_HEADER_LENGTH);
    return in;
}

/**
 * @brief 把类名写入SAMdiSubWindowSerializeHead
 * @param className
 * @param p
 */
void setTypeNameToSerializeHead(const QString &typeName, SAMdiSubWindowSerializeHead &p)
{
    QByteArray n = typeName.toLatin1();
    if(n.size()>=64)
        n = n.left(63);
    strcpy(p.param.typeName,n.data());
}

/**
 * @brief 从SAMdiSubWindowSerializeHead中获取类型名
 * @param p
 * @return
 */
QString getTypeNameFromSerializeHead(const SAMdiSubWindowSerializeHead &p)
{
    return QString::fromLatin1(p.param.typeName);
}
