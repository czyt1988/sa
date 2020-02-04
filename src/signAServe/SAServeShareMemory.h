#ifndef SASERVESHAREMEMORY_H
#define SASERVESHAREMEMORY_H
#include "SAServeGlobal.h"
class SAServeShareMemoryPrivate;
/**
 * @brief 关于服务器操作的共享内存相关的封装
 * @note 此类为单例
 */
class SASERVE_EXPORT SAServeShareMemory
{
    SA_IMPL(SAServeShareMemory)
protected:
    SAServeShareMemory(bool iscreate);
public:
    ~SAServeShareMemory();
    //判断是否有效
    bool isValid() const;
    //获取端口
    int getPort() const;
    //设置端口
    void setPort(int port);
    bool isListen() const;
    void setListenState(bool islisten);
    static SAServeShareMemory& getInstance(bool iscreate = false);
};

#endif // SASERVESHAREMEMORY_H
