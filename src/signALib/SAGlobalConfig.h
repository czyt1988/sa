#ifndef SAGLOBALCONFIG_H
#define SAGLOBALCONFIG_H

#include <QObject>
#include "SALibGlobal.h"
class SAGlobalConfigPrivate;
///
/// \brief sa的全局设置
/// \note 此类为单例,非线程安全
///
class SALIB_EXPORT SAGlobalConfig
{
private:
    explicit SAGlobalConfig();
    ~SAGlobalConfig();
    Q_DISABLE_COPY(SAGlobalConfig)
    void init();
public:
    static SAGlobalConfig* getInstance();
    //根据绘图点数获取绘图的曲线的线框
    int getPlotCurWidth(unsigned int dataSize);
private:
    SAGlobalConfigPrivate* m_d;///<
    static SAGlobalConfig* s_instance;
};

#ifndef saConfig
#define saConfig SAGlobalConfig::getInstance()
#endif

#endif // SAGLOBALCONFIG_H
