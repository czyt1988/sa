#include "SALocalServeBaseHeader.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "SACRC.h"
///
/// \brief 初始化结构体
///
void SALocalServeBaseHeader::init()
{
    memset(this,0,sizeof(SALocalServeBaseHeader));
    this->magic_start = SA_LOCAL_SER_HEADER_MAGIC_START;
    this->magic_end = SA_LOCAL_SER_HEADER_MAGIC_END;
}

///
/// \brief 通过magic数判断表头文件是否有效
/// \return
///
bool SALocalServeBaseHeader::isValid() const
{
    return ((SA_LOCAL_SER_HEADER_MAGIC_START==this->magic_start) && (SA_LOCAL_SER_HEADER_MAGIC_END==this->magic_end));
}


//===========================================================

QDataStream &operator <<(QDataStream &io, const SALocalServeBaseHeader &d)
{
    io.writeRawData((const char*)(&d),sizeof(SALocalServeBaseHeader));
    return io;
}

QDataStream &operator >>(QDataStream &io, SALocalServeBaseHeader &d)
{
    io.readRawData((char*)(&d),sizeof(SALocalServeBaseHeader));
    return io;
}


