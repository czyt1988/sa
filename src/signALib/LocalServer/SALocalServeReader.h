#ifndef SALOCALSERVEREADER_H
#define SALOCALSERVEREADER_H
#include <QObject>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include "SALibGlobal.h"
#include <QVector>

#include <QPointF>

//#define __USE_FIFO__
#ifdef __USE_FIFO__
#include <queue>
#endif
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
public slots:
    //清空状态和缓存
    Q_SLOT void clear();
protected:
    //接收的数据写入
    Q_SLOT void receiveData(const QByteArray& datas);
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
    //处理接收到的数据
    void dealRecDatas();
    //处理主包头
    void dealMainHeaderData();
    //处理线性数组的数据
    void dealVectorDoubleDataProcData();
    //处理线性数组的数据
    void dealString();
    //从FIFO中取数据，FIFO取出的数据会删除
    void getDataFromBuffer(QByteArray& data,int dataLen);
    //重置标志位
    void resetFlags();
private slots:
    Q_SLOT void onReadyRead();
private:
    QLocalSocket* m_socket;
#ifdef __USE_FIFO__
    std::queue<char> m_fifo;///< 读取数据的fifo
#else
    QByteArray m_buffer;///< 接收数据的缓冲，之前用QQueue<char>，发现性能表现太差，尤其是dequeue函数的性能太差
    int m_readedDataSize;///< 记录已经读取到的数据长度
    int m_readPos;///< 记录已经读取的位置
#endif
    //QQueue<char> m_fifo;///< 数据接收队列
    SALocalServeBaseHeader m_mainHeader;///< 当前的主包头
    bool m_isReadedMainHeader;///< 标记是否读取了包头
};
Q_DECLARE_METATYPE(QVector<double>)
#endif // SALOCALSERVEREADER_H
