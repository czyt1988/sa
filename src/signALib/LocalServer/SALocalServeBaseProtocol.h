#ifndef SALOCALSERVEBASEPROTOCOL_H
#define SALOCALSERVEBASEPROTOCOL_H
#include <QDataStream>
#include "SALibGlobal.h"
#include "SALocalServeBaseHeader.h"
#include <QVector>
#include <QPointF>
/// \def 握手协议数据区字节长度
#define SA_PROTOCOL_SHAKE_HAND_DATA_LEN (32)
///
/// \brief Local Server 传输协议的基类
///
class SALIB_EXPORT SALocalServeBaseProtocol
{
public:
    enum Type
    {
        TypeShakeHand = 1///< 握手协议
        ,TypeString ///< 字符串
        ,TypeVectorPointFData ///< 线性数组处理协议，后面接一个QVector<QPointF>
    };
    SALocalServeBaseProtocol();
    virtual ~SALocalServeBaseProtocol();
    //序列化数据区，不包括包头
    virtual void serializeIn(QDataStream& st) = 0;
    //序列化所有，包括包头和数据
    virtual void serializeOut(QDataStream& st) const = 0;
    virtual bool isValid() const;
    virtual Type type() const = 0;
    SALocalServeBaseHeader& header();
    const SALocalServeBaseHeader& header() const;
protected:
    SALocalServeBaseHeader m_header;
};

///
/// \brief 握手协议
///
class SALIB_EXPORT SALocalServeShakeHandProtocol : public SALocalServeBaseProtocol
{
public:
    SALocalServeShakeHandProtocol();
    virtual void serializeIn(QDataStream& st);
    virtual void serializeOut(QDataStream& st) const;
    virtual Type type() const;
private:
    unsigned char m_raw[SA_PROTOCOL_SHAKE_HAND_DATA_LEN];///< 握手协议的32字节数据区
};
///
/// \brief 字符串协议
///
class SALIB_EXPORT SALocalServeStringProtocol : public SALocalServeBaseProtocol
{
public:
    SALocalServeStringProtocol();
    virtual void serializeIn(QDataStream& st);
    virtual void serializeOut(QDataStream& st) const;
    virtual Type type() const;
    const QString& string() const;
    QString& string();
    void setString(const QString& str);
private:
    QString m_str;
};

///
/// \brief 数据点协议
///
class SALIB_EXPORT SALocalServeVectorPointProtocol : public SALocalServeBaseProtocol
{
public:
    SALocalServeVectorPointProtocol();
    virtual void serializeIn(QDataStream& st);
    virtual void serializeOut(QDataStream& st) const;
    virtual Type type() const;
    void setPoints(const QVector<QPointF>& points);
    const QVector<QPointF>& getPoints() const;
    QVector<QPointF>& getPoints();
    void setIDs(qintptr wndId,qintptr figId,qintptr itemId);
    void getIDs(qintptr& wndId,qintptr& figId,qintptr& itemId) const;
private:
    QVector<QPointF> m_points;
    qintptr m_winId;
    qintptr m_figId;
    qintptr m_itemId;
};


QDataStream& operator <<(QDataStream& st,const SALocalServeShakeHandProtocol& d);
QDataStream& operator >>(QDataStream& st,SALocalServeShakeHandProtocol& d);

QDataStream& operator <<(QDataStream& st,const SALocalServeStringProtocol &d);
QDataStream& operator >>(QDataStream& st,SALocalServeStringProtocol &d);

QDataStream& operator <<(QDataStream& st,const SALocalServeVectorPointProtocol &d);
QDataStream& operator >>(QDataStream& st,SALocalServeVectorPointProtocol &d);
#endif // SALOCALSERVEBASEPROTOCOL_H
