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
struct SALIB_EXPORT SALocalServeBaseHeader
{
public:
    enum Type
    {
        TypeShakeHand = 1///< 握手协议
        ,TypeVectorDoubleDataProc ///< 线性数组处理协议，后面接一个QVector<double>
    };
    SALocalServeBaseHeader();
    //发送端进程的pid
    uint getSendedPid() const;
    void setSendedPid(const uint &pid);
    //设置标识
    uint getKey() const;
    void setKey(const uint &key);
    //包类型，见SALocalServeBaseHeader::Type
    int getType() const;
    void setType(int type);
    //标记下一个包的尺寸
    size_t getDataSize() const;
    void setDataSize(size_t dataSize);
    //直接二进制操作
    friend QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
    friend QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);
    void write(QDataStream& io) const;
    void read(QDataStream& io);

    //TODO
    //转为xml格式
    QString toXML() const;
    //从xml格式转换
    bool fromXML(QXmlStreamReader* xml);
    //初始化xml，此时xml的节点位于<root>，最后要调用xml.writeEndElement();
    void initXmlStart(QXmlStreamWriter* xml) const;
    void writeXMLHeader(QXmlStreamWriter* xml) const;
protected:
    uint m_key;
    uint m_pid;
    int m_type;
    size_t m_dataSize;///< 标记下一个包的尺寸
};

SALIB_EXPORT QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
SALIB_EXPORT QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);

Q_DECLARE_METATYPE(SALocalServeBaseHeader)

#endif // SALOCALSERVEXMLDATA_H
