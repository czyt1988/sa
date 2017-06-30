#ifndef SALOCALCONNECTION_H
#define SALOCALCONNECTION_H
#include <QLocalSocket>
#include <QObject>
#include "SALocalServeFigureItemProcessHeader.h"
#include <QVector>
#include <QPointF>
class QThread;
class QLocalSocket;
class SALocalServeWriter;
class SALocalServeReader;
class SADataProcessVectorPointF;
class SADataFeatureItem;
///
/// \brief 连接服务器的socket
///
class SALocalConnection : public QObject
{
    Q_OBJECT
public:
    SALocalConnection(QLocalSocket* socket,QObject* parent = nullptr);
    ~SALocalConnection();
signals:
    void connected();
    void disconnected();
    //
private:
    ///
    /// \brief 私有信号，用于调用数据处理线程
    ///
    Q_SIGNAL void callVectorPointFProcess(const QVector<QPointF>& points,quintptr widget,quintptr item);
private:
    void initThread();
private slots:
    //错误发生
    Q_SLOT void errorOccurred(QLocalSocket::LocalSocketError err);
    //接收到客户端发的点数组
    Q_SLOT void onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader& header,QVector<QPointF>& datas);
    //接收到客户端的文字
    Q_SLOT void onReceivedString(const QString& str);
    //接收到握手协议
    Q_SLOT void onRecShakeHand();
    //接收到点数组的计算结果
    Q_SLOT void onProcessVectorPointFResult(SADataFeatureItem* result,quintptr widget,quintptr item);

private:
    QThread* m_calcThread;///< 计算线程
    SADataProcessVectorPointF* m_pointFCalctor;///< 点集计算处理
private:
    QLocalSocket* m_socket;
    SALocalServeWriter* m_writer;
    SALocalServeReader* m_reader;

};

#endif // SALOCALCONNECTION_H
