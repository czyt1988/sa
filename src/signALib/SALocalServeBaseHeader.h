#ifndef SALOCALSERVEBASEDATA_H
#define SALOCALSERVEBASEDATA_H
#include "SALibGlobal.h"
#include <QDataStream>
#define SA_XML_LOCALSERVE_ROOT "root"
#define SA_XML_LOCALSERVE_HEADER "header"
#define SA_XML_LOCALSERVE_HEADER_KEY "key"
#define SA_XML_LOCALSERVE_HEADER_TYPE "type"
#define SA_XML_LOCALSERVE_HEADER_PID "pid"
class QXmlStreamWriter;
class QXmlStreamReader;
class QString;
///
/// \brief 用于local serve传递的类
///
class SALIB_EXPORT SALocalServeBaseHeader
{
public:
    enum Type
    {
        ShakeHand = 1///< 握手协议
        ,VectorDoubleDataProc ///< 线性数组处理协议，后面接一个QVector<double>

    };

    SALocalServeBaseHeader();
    //创建握手字符串
    uint getSendedPid() const;
    void setSendedPid(const uint &pid);

    uint getKey() const;
    void setKey(const uint &key);
    int getType() const;
    void setType(int type);
    //转为xml格式
    virtual QString toXML() const;
    //从xml格式转换
    virtual bool fromXML(QXmlStreamReader* xml);
    //直接二进制操作
    friend QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
    friend QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);
    virtual void write(QDataStream& io) const;
    virtual void read(QDataStream& io);
    //初始化xml，此时xml的节点位于<root>，最后要调用xml.writeEndElement();
    void initXmlStart(QXmlStreamWriter* xml) const;
    void writeXMLHeader(QXmlStreamWriter* xml) const;
protected:
    uint m_key;
    uint m_pid;
    int m_type;
};
QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);

#endif // SALOCALSERVEXMLDATA_H
