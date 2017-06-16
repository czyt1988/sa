#ifndef SALOCALSERVEBASEDATA_H
#define SALOCALSERVEBASEDATA_H
#include "SALibGlobal.h"
#include <QDataStream>
class QString;
///
/// \brief 用于local serve传递的类
///
class SALIB_EXPORT SALocalServeBaseData
{
public:
    SALocalServeBaseData();
    //创建握手字符串
    uint getSendedPid() const;
    void setSendedPid(const uint &pid);

    uint getKey() const;
    void setKey(const uint &key);
    int getType() const;
    void setType(int type);

    friend QDataStream& operator <<(QDataStream& io,const SALocalServeBaseData& d);
    friend QDataStream& operator >>(QDataStream& io,SALocalServeBaseData& d);
    virtual void write(QDataStream& io) const;
    virtual void read(QDataStream& io);
private:
    uint m_key;
    uint m_pid;
    int m_type;
};
QDataStream& operator <<(QDataStream& io,const SALocalServeBaseData& d);
QDataStream& operator >>(QDataStream& io,SALocalServeBaseData& d);

#endif // SALOCALSERVEXMLDATA_H
