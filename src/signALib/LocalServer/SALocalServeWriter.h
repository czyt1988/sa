#ifndef SALOCALSERVEWRITER_H
#define SALOCALSERVEWRITER_H

#include <QObject>
#include <QPointF>
#include "SALibGlobal.h"
#include <QVector>
class QLocalSocket;
class SALIB_EXPORT SALocalServeWriter : public QObject
{
    Q_OBJECT
public:
    SALocalServeWriter(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeWriter(QObject* parent = nullptr);

    QLocalSocket *getSocket() const;
    void setSocket(QLocalSocket *socket);
    //判断是否具有可写条件
    bool isEnableToWrite() const;
public slots:
    //登录，成功会返回token
    Q_SLOT void login();
    //发送握手协议
    Q_SLOT void sendShakeHand();
    //发送数组
    Q_SLOT void sendDoubleVectorData(qintptr wndPtr,qintptr figPtr,qintptr itemPtr,const QVector<QPointF>& datas);
    //
    Q_SLOT void sendString(const QString str);
private:
    QLocalSocket* m_socket;
};

#endif // SALOCALSERVEWRITER_H
