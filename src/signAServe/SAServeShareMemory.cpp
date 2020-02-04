#include "SAServeShareMemory.h"
#include <QSharedMemory>
#include <QDebug>

const int SHARE_MEM_SIZE = 8;

#define OFFSET_STATE 0 //状态的内存偏移
#define OFFSET_PORT 4 //端口号的内存偏移


class SAServeShareMemoryPrivate
{
    SA_IMPL_PUBLIC(SAServeShareMemory)
public:
    SAServeShareMemoryPrivate(SAServeShareMemory* p,bool iscreate);
    void getFromShareMem(void *des,size_t offset, size_t size);
    void setToShareMem(void const* des,size_t offset,size_t size);
    QSharedMemory m_sharemem;
    bool m_state;
};


SAServeShareMemoryPrivate::SAServeShareMemoryPrivate(SAServeShareMemory *p,bool iscreate)
    :q_ptr(p)
    ,m_sharemem("signaDataProc.Main")
{
    if(iscreate)
    {
        m_state = m_sharemem.create(SHARE_MEM_SIZE);//创建8字节共享内存，端口号将写入这个共享内存中
    }
    else
    {
        m_state = m_sharemem.attach();
    }
    if(!m_state)
    {
        qDebug() << "share mem attach or create failed:" << m_sharemem.error();
    }
}

void SAServeShareMemoryPrivate::getFromShareMem(void *des,size_t offset, size_t size)
{
    m_sharemem.lock();
    char* p = (char*)m_sharemem.data();
    memcpy(des,p+offset,size);
    m_sharemem.unlock();
}

void SAServeShareMemoryPrivate::setToShareMem(const void *des, size_t offset, size_t size)
{
    m_sharemem.lock();
    char* p = (char*)m_sharemem.data();
    memcpy(p+offset,des,size);
    m_sharemem.unlock();
}

SAServeShareMemory::SAServeShareMemory(bool iscreate):d_ptr(new SAServeShareMemoryPrivate(this,iscreate))
{
}

SAServeShareMemory::~SAServeShareMemory()
{

}

/**
 * @brief 判断共享内存是否有效
 * @return 有效返回true
 */
bool SAServeShareMemory::isValid() const
{
    return d_ptr->m_state;
}

/**
 * @brief 获取端口
 * @return
 */
int SAServeShareMemory::getPort() const
{
    int port = -1;
    if(isValid())
    {
        d_ptr->getFromShareMem(&port,OFFSET_PORT,sizeof(int));
    }
    return port;
}

void SAServeShareMemory::setPort(int port)
{
    if(isValid())
    {
        d_ptr->setToShareMem(&port,OFFSET_PORT,sizeof(int));
    }
}

bool SAServeShareMemory::isListen() const
{
    int islisten = 0;
    if(isValid())
    {
        d_ptr->getFromShareMem(&islisten,OFFSET_STATE,sizeof(int));
    }
    return (0 != islisten);
}

void SAServeShareMemory::setListenState(bool islisten)
{
    int v = (islisten ? 1 : 0);
    if(isValid())
    {
        d_ptr->setToShareMem(&v,OFFSET_STATE,sizeof(int));
    }
}

SAServeShareMemory &SAServeShareMemory::getInstance(bool iscreate)
{
    static SAServeShareMemory s_sharemem(iscreate);
    return s_sharemem;
}

