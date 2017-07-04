#ifndef SALOCALSERVEFIGUREITEMPROCESSDATA_H
#define SALOCALSERVEFIGUREITEMPROCESSDATA_H
#include "SALibGlobal.h"
#include "SALocalServeBaseHeader.h"
///
/// \brief 用于通知处理进程处理曲线数据内容
///
struct SALIB_EXPORT SALocalServeFigureItemProcessHeader
{
public:
    enum Type
    {
        CurveData = 1
    };


    qintptr getWndPtr() const;
    void setWndPtr(const qintptr &wnd);

    qintptr getFigPtr() const;
    void setFigPtr(const qintptr &p);

    qintptr getItemPtr() const;
    void setItem(const qintptr &item);

    int getDataType() const;
    void setDataType(int dataType);

    size_t getDataLength() const;
    void setDataLength(size_t dataLen);

    size_t getDataVectorNum() const;
    void setDataVectorNum(size_t dataNum);
    //直接二进制操作
    friend QDataStream& operator <<(QDataStream& io,const SALocalServeFigureItemProcessHeader& d);
    friend QDataStream& operator >>(QDataStream& io,SALocalServeFigureItemProcessHeader& d);
    void write(QDataStream& io) const;
    void read(QDataStream& io);
    //发送的字节数
    static size_t sendSize();
    //判断数据是否有效
    bool isValid() const;
    //设置为有效，此操作会自动计算校验，对于接收的文件不需要调用，但对于发送的文件一定要再发送前调用
    void refreshCheck();
    //计算crc
    unsigned short calcCRC16() const;
protected:
    struct PrivateData
    {
        qintptr wndPtr;///< 用于记录对应的子窗口，这个指针只作为标记，不可调用
        qintptr figPtr;///< 用于记录对应的子窗口，的figure的指针，这个指针只作为标记，不可调用
        qintptr itemPtr;///< 用于记录窗口对应的绘图条目的指针，这个指针只作为标记，不可调用
        int dataType;///< 记录数据类型
        size_t dataLength;///< 数据的字节长度
        size_t dataVectorNum;///< 数组的长度
    };
    PrivateData m_d;
    bool m_isValid;
    unsigned short m_crc16;
};
SALIB_EXPORT QDataStream& operator <<(QDataStream& io,const SALocalServeFigureItemProcessHeader& d);
SALIB_EXPORT QDataStream& operator >>(QDataStream& io,SALocalServeFigureItemProcessHeader& d);

Q_DECLARE_METATYPE(SALocalServeFigureItemProcessHeader)
#endif // SALOCALSERVEFIGUREITEMPROCESSDATA_H
