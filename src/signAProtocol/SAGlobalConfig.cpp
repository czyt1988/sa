#include "SAGlobalConfig.h"
#include <QMutex>
#include <QHash>
#include "SAGlobalConfigDefine.h"
#include <QSet>
#include <QDir>
#include <QFileInfo>
#include "SAXMLConfig.h"

QString SAGlobalConfig::s_configFilePath = SAGlobalConfig::makeDefaultConfigPath();
QString SAGlobalConfig::s_configFileName = "sa.config";
class SAGlobalConfigPrivate
{
    SA_IMPL_PUBLIC(SAGlobalConfig)
public:
    SAGlobalConfigPrivate(SAGlobalConfig* par);
    SAXMLConfig m_xmlConfig;
};

SAGlobalConfigPrivate::SAGlobalConfigPrivate(SAGlobalConfig* par):q_ptr(par)
{
    SAGlobalConfig::makeDefaultConfigPath();
    m_xmlConfig.setFilePath(SAGlobalConfig::getConfigFullPath());
}

//============================================

SAGlobalConfig::SAGlobalConfig(const QString &cfgPath):SAXMLConfig(cfgPath)
  ,d_ptr(new SAGlobalConfigPrivate(this))
{
}

SAGlobalConfig::~SAGlobalConfig()
{
}


#define SET_XX_CONFIG(type,TypeFunName) \
    void SAGlobalConfig::TypeFunName(const QString& groupName,const QString& keyName,const type& val)\
    {\
        setValue(groupName,keyName,val);\
    }

#define GET_XX_CONFIG(type,TypeFunName,varToFunName) \
    type SAGlobalConfig::TypeFunName(const QString &groupName, const QString &keyName,const type& defaultVal) const\
    {\
        QVariant var = getValue(groupName,keyName);\
        if(!var.isValid())\
        {\
            return defaultVal;\
        }\
        bool isOK = false;\
        int res = var.varToFunName(&isOK);\
        if(!isOK)\
        {\
            return defaultVal;\
        }\
        return res;\
    }

#define GET_XX_CONFIG_Arg0(type,TypeFunName,varToFunName) \
    type SAGlobalConfig::TypeFunName(const QString &groupName, const QString &keyName,const type& defaultVal) const\
    {\
        QVariant var = getValue(groupName,keyName);\
        if(!var.isValid())\
        {\
            return defaultVal;\
        }\
        return var.varToFunName();\
    }
///
/// \brief 获取uint值
/// \param content 目录
/// \param key 索引
/// \param defaultVal 默认值
/// 如果没有对应的索引或目录将返回默认值
/// \return 获取对应的设定值，如果没有对应的索引或索引对应的值无法转为int，将返回默认值
///
GET_XX_CONFIG(unsigned int,getUIntValue,toUInt)
///
/// \brief 设置int值
/// \param content 目录
/// \param key 索引
/// \param var 值
///
SET_XX_CONFIG(unsigned int,setUIntValue)

///
/// \brief 获取int值
/// \param content 目录
/// \param key 索引
/// \param defaultVal 默认值
/// 如果没有对应的索引或目录将返回默认值
/// \return 获取对应的设定值，如果没有对应的索引或索引对应的值无法转为int，将返回默认值
///
GET_XX_CONFIG(int,getIntValue,toInt)


///
/// \brief 设置int值
/// \param content 目录
/// \param key 索引
/// \param var 值
///
SET_XX_CONFIG(int,setIntValue)

///
/// \brief 获取double值
/// \param content 目录
/// \param key 索引
/// \param defaultVal 默认值
/// 如果没有对应的索引或目录将返回默认值
/// \return 获取对应的设定值，如果没有对应的索引或索引对应的值无法转为double，将返回默认值
///
GET_XX_CONFIG(double,getDoubleValue,toDouble)
///
/// \brief 设置int值
/// \param content 目录
/// \param key 索引
/// \param var 值
///
SET_XX_CONFIG(double,setDoubleValue)

///
/// \brief 获取double值
/// \param content 目录
/// \param key 索引
/// \param defaultVal 默认值
/// 如果没有对应的索引或目录将返回默认值
/// \return 获取对应的设定值，如果没有对应的索引或索引对应的值无法转为double，将返回默认值
///
GET_XX_CONFIG(float,getFloatValue,toFloat)
///
/// \brief 设置int值
/// \param content 目录
/// \param key 索引
/// \param var 值
///
SET_XX_CONFIG(float,setFloatValue)

///
/// \brief 获取double值
/// \param content 目录
/// \param key 索引
/// \param defaultVal 默认值
/// 如果没有对应的索引或目录将返回默认值
/// \return 获取对应的设定值，如果没有对应的索引或索引对应的值无法转为double，将返回默认值
///
GET_XX_CONFIG(qlonglong,getLongLongValue,toLongLong)
///
/// \brief 设置int值
/// \param content 目录
/// \param key 索引
/// \param var 值
///
SET_XX_CONFIG(qlonglong,setLongLongValue)

///
/// \brief 获取QString值
/// \param content 目录
/// \param key 索引
/// \param defaultVal 默认值
/// 如果没有对应的索引或目录将返回默认值
/// \return 获取对应的设定值，如果没有对应的索引或索引对应的值无法转为double，将返回默认值
///
GET_XX_CONFIG_Arg0(QString,getStringValue,toString)
///
/// \brief 设置QString值
/// \param content 目录
/// \param key 索引
/// \param var 值
///
SET_XX_CONFIG(QString,setStringValue)



///
/// \brief 获取全局唯一单例
/// \return 全局唯一单例指针
///
SAGlobalConfig& SAGlobalConfig::getInstance()
{
    static SAGlobalConfig s_g_cfg(getConfigFullPath());
    return s_g_cfg;
}


///
/// \brief 获取sa的配置文件目录
/// \return 配置文件目录
///
QString SAGlobalConfig::getConfigPath()
{
    return s_configFilePath;
}

///
/// \brief 获取默认config path
/// \return 如果目录没有会尝试创建一个
///
QString SAGlobalConfig::makeDefaultConfigPath()
{
    QString path = QDir::currentPath() + QDir::separator() + "config";
    if(!QFileInfo::exists(path))
    {
        QDir dir(path);
        dir.mkpath(path);
    }
    return std::move(path);
}
///
/// \brief 获取配置文件名
/// \return
///
QString SAGlobalConfig::getConfigFileName()
{
    return s_configFileName;
}

QString SAGlobalConfig::getConfigFullPath()
{
    return s_configFilePath +  QDir::separator() + s_configFileName;
}




