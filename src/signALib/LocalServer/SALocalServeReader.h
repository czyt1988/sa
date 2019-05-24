#ifndef SALOCALSERVEREADER_H
#define SALOCALSERVEREADER_H

#include <QObject>
#include "SALocalServeBaseHeader.h"
#include "SALibGlobal.h"
#include <QVector>

#include <QPointF>
#include <QLocalSocket>

/**
 * @brief 读取服务器信息
 */
class SALIB_EXPORT SALocalServeReader : public QObject
{
    Q_OBJECT
public:
    SALocalServeReader(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeReader(QObject* parent = nullptr);
public:
    //获取套接字
    QLocalSocket *getSocket() const;
    //设置套接字
    void setSocket(QLocalSocket *socket, bool autoDicConnect = false);
    int getToken() const;
    void setToken(int token);

protected:
    //
    bool readFromSocket(void *p,int n);
signals:
    ///
    /// \brief 有错误发生
    /// \param info 错误描述
    ///
    void errorOccure(int errcode);
    ///
    /// \brief 接收
    /// \param head
    /// \param datas
    ///
    void receive(const SALocalServeBaseHeader& head,const QByteArray& datas);
    ///
    /// \brief 断开连接
    ///
    void disconnectFromServe();
private:
    //根据类型分发协议
    void deal(const SALocalServeBaseHeader& head,const QByteArray& datas);
private slots:
    Q_SLOT void onReadyRead();
    Q_SLOT void onDisconnected();
    Q_SLOT void onError(QLocalSocket::LocalSocketError socketError);
private:
    QLocalSocket* m_socket;
    SALocalServeBaseHeader m_mainHeader;///< 当前的主包头
    bool m_isReadedMainHeader;///< 标记是否读取了包头
    QByteArray m_buffer;
    int m_index;
    int m_token;
};

Q_DECLARE_METATYPE(QVector<double>)
#endif // SALOCALSERVEREADER_H
