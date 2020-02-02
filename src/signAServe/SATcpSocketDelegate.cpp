#include "SATcpSocketDelegate.h"
#include <QMutexLocker>
#include <QDebug>
#include "SATcpSocket.h"
class SATcpSocketDelegatePrivate
{
    SA_IMPL_PUBLIC(SATcpSocketDelegate)
public:
    SATcpSocketDelegatePrivate(SATcpSocketDelegate* p);
    bool deaXmlProtocol(const QByteArray &data);
};

SATcpSocketDelegatePrivate::SATcpSocketDelegatePrivate(SATcpSocketDelegate *p):q_ptr(p)
{

}

bool SATcpSocketDelegatePrivate::deaXmlProtocol(const QByteArray &data)
{
    QString xml = QString::fromUtf8(data);
    qDebug() << "xml:\r\n"
             << xml;
}

SATcpSocketDelegate::SATcpSocketDelegate(SATcpSocket *socket)
    :QObject(qobject_cast<QObject*>(socket))
  ,d_ptr(new SATcpSocketDelegatePrivate(this))
{
}

SATcpSocketDelegate::~SATcpSocketDelegate()
{

}

/**
 * @brief 处理数据
 * @param header
 * @param data
 * @return 返回false说明没有对此包数据进行处理
 */
bool SATcpSocketDelegate::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(!header.isValid())
    {
        return false;
    }
    if(header.classID == SA_PROTOCOL_CLASS_ID_XML_PROTOCOL
            &&
            header.functionID == SA_PROTOCOL_FUNCTION_ID_STR_PROTOCOL)
    {
        return d_ptr->deaXmlProtocol(data);
    }
    return false;
}




