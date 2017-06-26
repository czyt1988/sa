#ifndef SALOCALSERVEREADER_H
#define SALOCALSERVEREADER_H
#include <QObject>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include "SALibGlobal.h"
#include <QQueue>
#include <QVector>
class SALIB_EXPORT SALocalServeReader : public QObject
{
    Q_OBJECT
public:
    SALocalServeReader(QObject* parent = nullptr);
public:
    Q_SLOT receiveData(const QByteArray& datas);
    const SALocalServeBaseHeader& getMainHeader() const;
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
private:
    QQueue<char> m_fifo;///< 数据接收队列
    SALocalServeBaseHeader m_mainHeader;///< 当前的主包头
    //QByteArray m_receiveData;///< 接收的数据
    bool m_isReadedMainHeader;///< 标记是否读取了包头
    size_t m_dataLength;///< 数据长度
};
Q_DECLARE_METATYPE(QVector<double>)
#endif // SALOCALSERVEREADER_H
