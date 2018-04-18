#ifndef SAGLOBALCONFIG_H
#define SAGLOBALCONFIG_H

#include <QObject>
#include <QVariant>
#include <memory>
#include "SALibGlobal.h"
class SAGlobalConfigPrivate;



///
/// \brief sa的全局设置
/// \note 此类为单例,非线程安全
///
class SALIB_EXPORT SAGlobalConfig
{
    SA_IMPL(SAGlobalConfig)
private:
    explicit SAGlobalConfig();
    ~SAGlobalConfig();
    Q_DISABLE_COPY(SAGlobalConfig)
public:
    //检测是否存在目录
    bool isHasGroup(const QString& group) const;
    //检测是否存在对应索引
    bool isHasKey(const QString& group, const QString& key) const;
    //获取键值对应的内容
    QVariant getValue(const QString& namePath,const QVariant& defaultVal = QVariant()) const;
    QVariant getValue(const QString& group, const QString& key,const QVariant& defaultVal = QVariant()) const;
    //设置键值
    void setValue(const QString& namePath, const QVariant& var);
    void setValue(const QString& group, const QString& key, const QVariant& var);
    //获取数据，确保数据有默认构造函数
    template<typename T>
    T valueFromKey(const QString& content,const QString& key,const T& defaultVal = T());
    //获取所有目录关键字
    QStringList getGroupList() const;
    //获取目录下对应的所有关键字
    QStringList getKeyList(const QString& group) const;
    //保存
    bool save();
public:
    //获取uint型的配置
    unsigned int getUIntValue(const QString& content,const QString& key,const unsigned int& defaultVal = -1) const;
    void setUIntValue(const QString& content,const QString& key,const unsigned int& val);
    //获取int型的配置
    int getIntValue(const QString& content,const QString& key,const int& defaultVal = -1) const;
    void setIntValue(const QString& content,const QString& key,const int& val);
    //获取double型的配置
    double getDoubleValue(const QString& content,const QString& key,const double& defaultVal = 0) const;
    void setDoubleValue(const QString& content,const QString& key,const double& val);
    //获取float型的配置
    float getFloatValue(const QString& content,const QString& key,const float& defaultVal = 0) const;
    void setFloatValue(const QString& content,const QString& key,const float& val);
    //获取qlonglong型的配置
    qlonglong getLongLongValue(const QString& content,const QString& key,const qlonglong& defaultVal = 0) const;
    void setLongLongValue(const QString& content,const QString& key,const qlonglong& val);
    //获取QString型的配置
    QString getStringValue(const QString& content,const QString& key,const QString& defaultVal = QString()) const;
    void setStringValue(const QString& content,const QString& key,const QString& val);
public:
    //获取sa的配置文件目录
    static QString getConfigPath();
    //获取默认config path
    static QString makeDefaultConfigPath();
    //获取sa的配置文件名
    static QString getConfigFileName();
    //获取sa的配置文件名
    static QString getConfigFullPath();
public:
    static SAGlobalConfig& getInstance();
private:
    static SAGlobalConfig* s_instance;
    static QString s_configFilePath;///< 配置文件路径
    static QString s_configFileName;///< 配置文件名
};

template<typename T>
T SAGlobalConfig::valueFromKey(const QString &content, const QString &key,const T& defaultVal)
{
    if(!isHasKey(content,key))
    {
        return defaultVal;
    }
    QVariant var = getValue(content,key);
    if(!var.isValid())
    {
        return defaultVal;
    }
    if(!var.canConvert<T>())
    {
        return defaultVal;
    }
    return var.value<T>();
}

#ifndef saConfig
#define saConfig SAGlobalConfig::getInstance()
#endif

#endif // SAGLOBALCONFIG_H
