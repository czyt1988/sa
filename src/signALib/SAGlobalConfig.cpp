#include "SAGlobalConfig.h"
#include <QMutex>

SAGlobalConfig* SAGlobalConfig::s_instance = nullptr;

class SAGlobalConfigPrivate
{
public:
    friend class SAGlobalConfig;
    void init();
    unsigned int pointsCurve2p;///< 曲线点数小于此值，曲线的默认线宽为2
};
void SAGlobalConfigPrivate::init()
{
    this->pointsCurve2p = 1024;
}

//============================================

SAGlobalConfig::SAGlobalConfig()
    :m_d(new SAGlobalConfigPrivate)
{
    m_d->init();
}

SAGlobalConfig::~SAGlobalConfig()
{
    if(NULL != s_instance)
    {
        if(NULL != s_instance)
        {
            delete s_instance;
            s_instance = NULL;
        }
    }
    if(m_d)
    {
        delete m_d;
    }
}
///
/// \brief 获取全局唯一单例
/// \return 全局唯一单例指针
///
SAGlobalConfig *SAGlobalConfig::getInstance()
{
    if(NULL == s_instance)
    {
        QMutex locker;
        locker.lock();
        if(NULL == s_instance)
        {
            s_instance = new SAGlobalConfig();
        }
        locker.unlock();
    }
    return s_instance;
}
///
/// \brief 根据绘图点数获取绘图的曲线的线框
/// \param dataSize 绘图的数据点数
/// \return 线宽
///
int SAGlobalConfig::getPlotCurWidth(unsigned int dataSize)
{
    if(dataSize < m_d->pointsCurve2p)
    {
        return 2;
    }
    return 1;
}




