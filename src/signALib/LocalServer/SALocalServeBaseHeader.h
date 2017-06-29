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
        ,TypeString ///< 字符串
        ,TypeVectorPointFData ///< 线性数组处理协议，后面接一个QVector<QPointF>
    };
    SALocalServeBaseHeader();
    //设置标识
    uint getKey() const;
    void setKey(const uint &key);
    //包类型，见SALocalServeBaseHeader::Type
    int getType() const;
    void setType(int type);
    //标记下一个包的尺寸
    size_t getDataSize() const;
    void setDataSize(size_t dataSize);
    //判断是否有效
    bool isValid() const;
    //设置为有效，此操作会自动计算校验，对于接收的文件不需要调用，但对于发送的文件一定要再发送前调用
    void refreshCheck();

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
    unsigned short calcCRC16() const;
    //发送的字节数
    static size_t sendSize();
protected:
    struct PrivateData
    {
        uint key;
        int type;
        size_t dataSize;///< 标记下一个包的尺寸
    };
    PrivateData m_d;
    bool m_isValid;
    unsigned short m_crc16;
};

SALIB_EXPORT QDataStream& operator <<(QDataStream& io,const SALocalServeBaseHeader& d);
SALIB_EXPORT QDataStream& operator >>(QDataStream& io,SALocalServeBaseHeader& d);

Q_DECLARE_METATYPE(SALocalServeBaseHeader)

#endif // SALOCALSERVEXMLDATA_H
