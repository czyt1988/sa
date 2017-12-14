#include "SAMdiSubWindowSerializeHead.h"
#define MAGIC_NUM (0xF123AB02)

bool SAMdiSubWindowSerializeHead::isValid() const
{
    return MAGIC_NUM == param.magic1;
}

void SAMdiSubWindowSerializeHead::setValid()
{
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
