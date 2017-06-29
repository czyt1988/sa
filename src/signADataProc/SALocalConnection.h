#ifndef SALOCALCONNECTION_H
#define SALOCALCONNECTION_H
#include <QLocalSocket>
#include <QObject>
#include "SALocalServeFigureItemProcessHeader.h"
#include <QVector>
#include <QPointF>

class QLocalSocket;
class SALocalServeWriter;
class SALocalServeReader;
class SALocalConnection : public QObject
{
    Q_OBJECT
public:
    SALocalConnection(QLocalSocket* socket,QObject* parent = nullptr);
    ~SALocalConnection();
signals:
    void connected();
    void disconnected();
private slots:
    //错误发生
    Q_SLOT void errorOccurred(QLocalSocket::LocalSocketError err);

    Q_SLOT void onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader& header,QVector<QPointF>& datas);

    Q_SLOT void onReceivedString(const QString& str);
    //接收到握手协议
    Q_SLOT void onRecShakeHand();
private:
    QLocalSocket* m_socket;
    SALocalServeWriter* m_writer;
    SALocalServeReader* m_reader;
};

#endif // SALOCALCONNECTION_H
