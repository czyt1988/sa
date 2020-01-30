#include "SAProtocolHeader.h"

/**
 * @brief 初始化结构体
 */
void SAProtocolHeader::init()
{
    memset(this,0,sizeof(SAProtocolHeader));
    this->magic_start = SA_PROTOCOL_HEADER_MAGIC_START;
    this->magic_end = SA_PROTOCOL_HEADER_MAGIC_END;
}
/**
 * @brief 通过magic数判断表头文件是否有效
 * 
 * 此判断是简易判断
 * @return 
 */
bool SAProtocolHeader::isValid() const
{
    return ((SA_PROTOCOL_HEADER_MAGIC_START==this->magic_start) && (SA_PROTOCOL_HEADER_MAGIC_END==this->magic_end));
}


QDataStream &operator <<(QDataStream &io, const SAProtocolHeader &d)
{
    io.writeRawData((const char*)(&d),sizeof(SAProtocolHeader));
    return io;
}


QDataStream &operator >>(QDataStream &io, SAProtocolHeader &d)
{
    io.readRawData((char*)(&d),sizeof(SAProtocolHeader));
    return io;
}

QDebug& operator<<(QDebug& debug, const SAProtocolHeader &d)
{
    QDebugStateSaver saver(debug);
    debug << "SAProtocolHeader is valid:" << d.isValid()
          << "\n{"
          << "\n magic_start:0x" << hex << d.magic_start
          << "\n token:" << token
          << "\n sequenceID:" << dec << d.sequenceID
          << "\n classID:" << d.classID
          << "\n functionID:" << d.functionID
          << "\n dataSize:" << d.dataSize
          << "\n extendValue:" << d.extendValue
          << "\n dataCrc32:0x" << hex << d.dataCrc32
          << "\n magic_end:0x" << hex << d.magic_end
          << "\n}";
    return debug;
}
/**
 * @brief 用于判断是否是一个正确的协议头，此函数会读取p指针位置后32字节，需要确保字节有效
 * 
 * 此检查仅通过魔数进行校验，会存在巧合
 * 
 * @param p 检查内存起始地址
 * @return 检查正确返回true
 */
bool is_valid_sa_protocol_header(const char *p)
{
    SAProtocolHeader h;
    memcpy((void*)&h,p,sizeof(SAProtocolHeader));
    return h.isValid();
}
