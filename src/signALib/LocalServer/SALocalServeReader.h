#ifndef SALOCALSERVEREADER_H
#define SALOCALSERVEREADER_H

#include <QObject>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include "SALibGlobal.h"
#include <QVector>

#include <QPointF>
#include <QLocalSocket>

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
protected:
    //
    bool readFromSocket(void *p,int n);
signals:
    ///
    /// \brief 有错误发生
    /// \param info 错误描述
    ///
    void errorOccure(const QString& info);
    ///
    /// \brief 接收到握手协议
    /// \param 协议文件头
    ///
    void receivedShakeHand(const SALocalServeBaseHeader& mainHeader);
    ///
    /// \brief 接收到线性数组数据
    /// \param header 文件头
    /// \param ys 数据
    ///
    void receivedVectorPointFData(const SALocalServeFigureItemProcessHeader& header,QVector<QPointF>& ys);
    ///
    /// \brief 接收到xml字符
    /// \param 字符
    ///
    void receivedString(const QString& info);
private:
    //处理线性数组的数据
    void dealVectorDoubleDataProcData(const QByteArray& datas);
    //处理线性数组的数据
    void dealString(const QByteArray& datas);
private slots:
    Q_SLOT void onReadyRead();
    Q_SLOT void onDisconnected();
    Q_SLOT void onError(QLocalSocket::LocalSocketError socketError);
private:
    QLocalSocket* m_socket;
    SALocalServeBaseHeader m_mainHeader;///< 当前的主包头
    bool m_isReadedMainHeader;///< 标记是否读取了包头
};

Q_DECLARE_METATYPE(QVector<double>)
#endif // SALOCALSERVEREADER_H
