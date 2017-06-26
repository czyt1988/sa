#ifndef SALOCALSERVEWRITER_H
#define SALOCALSERVEWRITER_H

#include <QObject>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
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

public slots:
    //发送握手协议
    Q_SLOT void sendShakeHand();
private:
    QLocalSocket* m_socket;
};

#endif // SALOCALSERVEWRITER_H
