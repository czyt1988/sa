#include "SAServeShareMemory.h"
#include <QSharedMemory>
#include <QDebug>

struct SAServeShareMemoryPrivateData
{
    int serve_state; //4
    int serve_port; //8
};

#define OFFSET_STATE 0 //状态的内存偏移
#define OFFSET_PORT 4 //端口号的内存偏移

class SAServeShareMemoryPrivate
{
    SA_IMPL_PUBLIC(SAServeShareMemory)
public:
    SAServeShareMemoryPrivate(SAServeShareMemory* p);
    void getFromShareMem(void *des,size_t offset, size_t size);
    void setToShareMem(void const* des,size_t offset,size_t size);
    bool updateDataFromSharedMem();
    bool updateDataToSharedMem();
    QSharedMemory m_sharemem;
    SAServeShareMemoryPrivateData m_data;
};


SAServeShareMemoryPrivate::SAServeShareMemoryPrivate(SAServeShareMemory *p)
    :q_ptr(p)
    ,m_sharemem("signaDataProc.Serve")
{
    FUNCTION_RUN_PRINT();
    memset(&m_data,0,sizeof(SAServeShareMemoryPrivateData));
    if(!m_sharemem.create(sizeof(SAServeShareMemoryPrivateData)))
    {
        // 如果创建失败，说明已经有别的进程创建了该key对应的共享内存，那么本进程直接attach，不需要创建/也不能创建
        if(!m_sharemem.attach())
        {
            qDebug() << ("create share memory failed:") << m_sharemem.errorString() << " err code:" << m_sharemem.error();
        }
    }
    else
    {
        //说明是第一次创建
        updateDataToSharedMem();
    }
}

void SAServeShareMemoryPrivate::getFromShareMem(void *des,size_t offset, size_t size)
{
    m_sharemem.lock();
    memcpy(des,(char*)m_sharemem.data()+offset,size);
    m_sharemem.unlock();
}

void SAServeShareMemoryPrivate::setToShareMem(const void *des, size_t offset, size_t size)
{
    m_sharemem.lock();
    memcpy((char*)m_sharemem.data()+offset,des,size);
    m_sharemem.unlock();
}

bool SAServeShareMemoryPrivate::updateDataFromSharedMem()
{
    if(m_sharemem.isAttached())
    {
        m_sharemem.lock();
        memcpy(&m_data,m_sharemem.data(),sizeof(SAServeShareMemoryPrivateData));
        m_sharemem.unlock();
        return true;
    }
    return false;
}

bool SAServeShareMemoryPrivate::updateDataToSharedMem()
{
    if(m_sharemem.isAttached())
    {
        m_sharemem.lock();
        memcpy(m_sharemem.data(),&m_data,sizeof(SAServeShareMemoryPrivateData));
        m_sharemem.unlock();
        return true;
    }
    return false;
}

SAServeShareMemory::SAServeShareMemory():d_ptr(new SAServeShareMemoryPrivate(this))
{
}

SAServeShareMemory::~SAServeShareMemory()
{
    
}

bool SAServeShareMemory::isAttach() const
{
    return d_ptr->m_sharemem.isAttached();   
}

/**
 * @brief 判断共享内存是否有效
 * @return 有效返回true
 */
bool SAServeShareMemory::isReady() const
{
    return (d_ptr->m_data.serve_state == ServeIsReady);
}

/**
 * @brief 设置服务器状态
 * @param state
 */
void SAServeShareMemory::setServeState(SAServeShareMemory::ServeState state)
{
    if(isAttach())
    {
        d_ptr->m_data.serve_state = (int)state;
        d_ptr->updateDataToSharedMem();
    }
}

/**
 * @brief 获取端口
 * @return
 */
int SAServeShareMemory::getPort() const
{
    return d_ptr->m_data.serve_port;
}

void SAServeShareMemory::setPort(int port)
{
    if(isAttach())
    {
        d_ptr->m_data.serve_port = port;
        d_ptr->updateDataToSharedMem();
    }
}

bool SAServeShareMemory::isListen() const
{
    int islisten = 0;
    if(isReady())
    {
        d_ptr->getFromShareMem(&islisten,OFFSET_STATE,sizeof(int));
    }
    return (0 != islisten);
}

void SAServeShareMemory::setListenState(bool islisten)
{
    int v = (islisten ? 1 : 0);
    if(isReady())
    {
        d_ptr->setToShareMem(&v,OFFSET_STATE,sizeof(int));
    }
}

void SAServeShareMemory::updateFromMem()
{
    d_ptr->updateDataFromSharedMem();
}

