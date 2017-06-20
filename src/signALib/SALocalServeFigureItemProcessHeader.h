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

    qintptr getItem() const;
    void setItem(const qintptr &item);

    int getDataType() const;
    void setDataType(int dataType);

    //直接二进制操作
    friend QDataStream& operator <<(QDataStream& io,const SALocalServeFigureItemProcessHeader& d);
    friend QDataStream& operator >>(QDataStream& io,SALocalServeFigureItemProcessHeader& d);
    void write(QDataStream& io) const;
    void read(QDataStream& io);
protected:
    qintptr m_wndPtr;///< 用于记录对应的子窗口，这个指针只作为标记，不可调用
    qintptr m_itemPtr;///< 用于记录窗口对应的绘图条目的指针，这个指针只作为标记，不可调用
    int m_dataType;///< 记录数据类型
};
SALIB_EXPORT QDataStream& operator <<(QDataStream& io,const SALocalServeFigureItemProcessHeader& d);
SALIB_EXPORT QDataStream& operator >>(QDataStream& io,SALocalServeFigureItemProcessHeader& d);
#endif // SALOCALSERVEFIGUREITEMPROCESSDATA_H
