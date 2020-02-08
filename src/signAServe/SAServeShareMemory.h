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
public:
    SAServeShareMemory();
    ~SAServeShareMemory();
    enum ServeState{
        ServeNotReady = 0
        ,ServeIsReady = 1
    };

    //是否连接内存
    bool isAttach() const;
    //判断是否准备好
    bool isReady() const;
    //设置服务器状态
    void setServeState(ServeState state);
    //获取端口
    int getPort() const;
    //设置端口
    void setPort(int port);
    //
    bool isListen() const;
    //
    void setListenState(bool islisten);
    //从共享内存中更新数据
    void updateFromMem();
};

#endif // SASERVESHAREMEMORY_H
