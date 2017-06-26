#ifndef SALOCALSERVEREADER_H
#define SALOCALSERVEREADER_H
#include <QObject>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include "SALibGlobal.h"
#include <QQueue>
#include <QVector>
class QLocalSocket;
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
    void setSocket(QLocalSocket *socket);
protected:
    //接收的数据写入
    Q_SLOT receiveData(const QByteArray& datas);
signals:
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
    void receivedVectorDoubleData(const SALocalServeFigureItemProcessHeader& header,QVector<double>& ys);
private:
    //处理接收到的数据
    void dealRecDatas();
    //处理主包头
    void dealMainHeaderData();
    //处理线性数组的数据
    void dealVectorDoubleDataProcData();
    //从FIFO中取数据，FIFO取出的数据会删除
    void getDataFromFifo(QByteArray& data,int dataLen);
    //重置标志位
    void resetFlags();
private slots:
    Q_SLOT void onReadyRead();
private:
    QLocalSocket* m_socket;
    QQueue<char> m_fifo;///< 数据接收队列
    SALocalServeBaseHeader m_mainHeader;///< 当前的主包头
    bool m_isReadedMainHeader;///< 标记是否读取了包头
    size_t m_dataLength;///< 数据长度
};
Q_DECLARE_METATYPE(QVector<double>)
#endif // SALOCALSERVEREADER_H
