#include "SATcpSection.h"
#include <QMap>
#include "SAServeHandleFun.h"

class SATcpSectionPrivate
{
public:
    SA_IMPL_PUBLIC(SATcpSection)
    SATcpSectionPrivate(SATcpSection* p,SATcpSocket *socket);
public:
    SATcpSocket* m_socket;
};

SATcpSectionPrivate::SATcpSectionPrivate(SATcpSection *p, SATcpSocket *socket):q_ptr(p)
  ,m_socket(socket)
{
    if (p != socket->parent())
    {
        socket->setParent(p);
    }
}

/**
 * @brief socket的生命周期由section管理,且socket的父对象会设置为section
 * @param socket
 * @param par
 */
SATcpSection::SATcpSection(SATcpSocket *socket, QObject *par):QObject(par)
  ,d_ptr(new SATcpSectionPrivate(this,socket))
{
    connect(socket,&SATcpSocket::receivedData,this,&SATcpSection::onReceivedSocketData);
    connect(socket,&QAbstractSocket::disconnected, this, &SATcpSection::onSocketDisconnected);
}

SATcpSection::~SATcpSection()
{

}

SATcpSocket *SATcpSection::socket() const
{
    return d_ptr->m_socket;
}

/**
 * @brief 处理收到的数据
 * @param header 协议头
 * @param data 数据
 * @return 如果成功处理返回true，如果处理失败返回false，继承此函数可以通过父类的返回来判断是否进行了处理
 *
 * 返回的状态不受dealResult影响
 *
 * @note 此函数默认会调用dealResult，重写时，需要注意
 */
bool SATcpSection::dealReceiveData(const SAProtocolHeader &header, const QByteArray &data)
{
    FUNCTION_RUN_PRINT();
    SA::FunHandle fun = SA::get_serve_funciton_handle(header.protocolFunID);
    if(nullptr == fun)
    {
        return false;
    }
    QVariantHash res;
    bool stat = fun(header,data,d_ptr->m_socket,&res);
    if(!stat)
    {
        return false;
    }
    dealResult(header,res);
    return true;
}

/**
 * @brief 处理函数返回的结果
 *
 * 这个是针对一些特殊协议会返回结果需要进一步处理的情况
 * @param header 协议头
 * @param data 返回的结果集
 * @return
 */
bool SATcpSection::dealResult(const SAProtocolHeader &header, const QVariantHash &data)
{
    FUNCTION_RUN_PRINT();
    Q_UNUSED(header);
    Q_UNUSED(data);
    return false;
}

void SATcpSection::onReceivedSocketData(const SAProtocolHeader &header, const QByteArray &data)
{
    dealReceiveData(header,data);
}

void SATcpSection::onSocketDisconnected()
{
    emit socketDisconnected();
}


