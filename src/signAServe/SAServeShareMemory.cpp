#include "SAServeShareMemory.h"
#include <QSharedMemory>
#include <QDebug>

struct SAServeShareMemoryPrivateData
{
    int serve_state; //0
    int serve_port; //4
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
    memset(&m_data,0,sizeof(SAServeShareMemoryPrivateData));
    if(!m_sharemem.create(sizeof(SAServeShareMemoryPrivateData)))
    {
        // 如果创建失败，说明已经有别的进程创建了该key对应的共享内存，那么本进程直接attach，不需要创建/也不能创建
        if(!m_sharemem.attach())
        {
            qDebug() << ("create share memory failed:") << m_sharemem.errorString() << " err code:" << m_sharemem.error();
        }
        updateDataFromSharedMem();
    }
    else
    {
        //说明是第一次创建
        qDebug() << ("create share memory sucess,will update data to share memory");
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
 * @brief 返回服务状态
 * @return
 */
SAServeShareMemory::ServeState SAServeShareMemory::getServeState() const
{
    return static_cast<ServeState>(d_ptr->m_data.serve_state);
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

/**
 * @brief 把共享内存的内容更新到本地内存
 */
void SAServeShareMemory::updateFromMem()
{
    d_ptr->updateDataFromSharedMem();
}

/**
 * @brief 返回描述
 * @return
 */
QString SAServeShareMemory::describe() const
{
    QString str;
    QTextStream io(&str);
    io << (*this);
    io.flush();
    return std::move(str);
}

/**
 * @brief 对SAServeShareMemory进行格式化文本输出
 * @param mem
 * @return
 */
QTextStream &operator <<(QTextStream &io, const SAServeShareMemory &mem)
{
    io << "SAServeShareMemory->"
       << "\n shared memory key:" << mem.d_ptr->m_sharemem.key()
       << "\n is attached:" << mem.d_ptr->m_sharemem.isAttached()
        << "\n size:" << mem.d_ptr->m_sharemem.size()
       << "\n error:" << (int)mem.d_ptr->m_sharemem.error() << " ,error string:" << mem.d_ptr->m_sharemem.errorString()
       << "\n serve_state:" << mem.getServeState()
       << "\n serve_port:" << mem.getPort()
          ;
    return io;
}
