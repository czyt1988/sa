#ifndef SALOCALSERVEBASEPROTOCOL_H
#define SALOCALSERVEBASEPROTOCOL_H
#include <QDataStream>
#include "SALocalServeBaseHeader.h"
///
/// \brief Local Server 传输协议的基类
///
class SALocalServeBaseProtocol
{
public:
    enum Type
    {
        TypeShakeHand = 1///< 握手协议
        ,TypeString ///< 字符串
        ,TypeVectorPointFData ///< 线性数组处理协议，后面接一个QVector<QPointF>
    }
    SALocalServeBaseProtocol();
    virtual ~SALocalServeBaseProtocol();
    virtual void serializeIn(QDataStream& st) = 0;
    virtual void serializeOut(QDataStream& st) const = 0;
    virtual bool isValid() const = 0;
};

///
/// \brief 握手协议
///
class SALocalServeShakeHandProtocol : public SALocalServeBaseProtocol
{
public:
    SALocalServeShakeHandProtocol();
    virtual void serializeIn(QDataStream& st);
    virtual void serializeOut(QDataStream& st) const;
};

#endif // SALOCALSERVEBASEPROTOCOL_H
