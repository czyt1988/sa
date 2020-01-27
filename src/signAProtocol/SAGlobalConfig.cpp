#include "SAGlobalConfig.h"
#include <QMutex>
#include <QHash>
#include "SAGlobalConfigDefine.h"
#include <QSet>
#include <QDir>
#include <QFileInfo>
#include "SAConfigXMLReadWriter.h"

SAGlobalConfig* SAGlobalConfig::s_instance = nullptr;
QString SAGlobalConfig::s_configFilePath = QString();
QString SAGlobalConfig::s_configFileName = "sa.config";
class SAGlobalConfigPrivate
{
    SA_IMPL_PUBLIC(SAGlobalConfig)
public:
    SAGlobalConfigPrivate(SAGlobalConfig* par);
    SAConfigXMLReadWriter m_xmlConfig;
};

SAGlobalConfigPrivate::SAGlobalConfigPrivate(SAGlobalConfig* par):q_ptr(par)
{
    SAGlobalConfig::makeDefaultConfigPath();
    m_xmlConfig.setFilePath(SAGlobalConfig::getConfigFullPath());
}

//============================================

SAGlobalConfig::SAGlobalConfig()
    :d_ptr(new SAGlobalConfigPrivate(this))
{
}

SAGlobalConfig::~SAGlobalConfig()
{
    if(NULL != s_instance)
    {
        delete s_instance;
        s_instance = NULL;
    }
}

///
/// \brief 检测是否存在目录
/// \param content 目录名称
/// \return
///
bool SAGlobalConfig::isHasGroup(const QString &group) const
{
    return d_ptr->m_xmlConfig.isHasGroup(group);
}
///
/// \brief 检测是否存在对应索引
/// \param content 目录名称
/// \param key 索引名称
/// \return
///
bool SAGlobalConfig::isHasKey(const QString &group, const QString &key) const
{
    return d_ptr->m_xmlConfig.isHasKey(group,key);
}

QVariant SAGlobalConfig::getValue(const QString &namePath, const QVariant &defaultVal) const
{
    return d_ptr->m_xmlConfig.getValue(namePath,defaultVal);
}
///
/// \brief 获取键值对应的内容
/// \param content 目录名称
/// \param key 索引名称
/// \return 如果没有内容，返回为QVariant(),可以通过isValid判断
///
QVariant SAGlobalConfig::getValue(const QString &group, const QString &key,const QVariant& defaultVal) const
{
    return d_ptr->m_xmlConfig.getValue(group,key,defaultVal);
}
///
/// \brief 设置内容
/// \param namePath
/// \param var
///
void SAGlobalConfig::setValue(const QString &namePath, const QVariant &var)
{
    d_ptr->m_xmlConfig.setValue(namePath,var);
}
///
/// \brief 设置内容
/// \param content 目录名称
/// \param key 索引名称
/// \param var 值
///
void SAGlobalConfig::setValue(const QString &group, const QString &key, const QVariant &var)
{
    d_ptr->m_xmlConfig.setValue(group,key,var);
}
///
/// \brief 获取所有目录关键字
/// \return 目录关键字列表
///
QStringList SAGlobalConfig::getGroupList() const
{
    return d_ptr->m_xmlConfig.getGroupNames();
}
///
/// \brief 获取目录下对应的所有关键字
/// \param content 目录名
/// \return 关键字列表
///
QStringList SAGlobalConfig::getKeyList(const QString &group) const
{
    return d_ptr->m_xmlConfig.getKeyNames(group);
}
///
/// \brief 保存
/// \return
///
bool SAGlobalConfig::save()
{
    return d_ptr->m_xmlConfig.saveAs(getConfigFullPath());
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
    static SAGlobalConfig s_g_cfg;
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
    if(s_configFilePath.isNull())
    {
        s_configFilePath = QDir::currentPath() + QDir::separator() + "config";
    }
    if(!QFileInfo::exists(s_configFilePath))
    {
        QDir dir(s_configFilePath);
        dir.mkpath(s_configFilePath);
    }
    return s_configFilePath;
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
    if(s_configFilePath.isNull())
    {
        makeDefaultConfigPath();
    }
    return makeDefaultConfigPath() +  QDir::separator() + getConfigFileName();
}




