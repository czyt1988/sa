#ifndef SAGLOBALCONFIG_H
#define SAGLOBALCONFIG_H

#include <QObject>
#include <QVariant>
#include <memory>
#include "SAProtocolGlobal.h"
#include "SAXMLConfig.h"
class SAGlobalConfigPrivate;


/**
 * @brief sa的全局设置
 * @note 此类为单例,非线程安全
 */
class SA_PROTOCOL_EXPORT SAGlobalConfig : public SAXMLConfig
{
    SA_IMPL(SAGlobalConfig)
private:
    explicit SAGlobalConfig(const QString& cfgPath = QString());
    ~SAGlobalConfig();
    Q_DISABLE_COPY(SAGlobalConfig)
public:
    //获取uint型的配置
    unsigned int getUIntValue(const QString& content, const QString& key, const unsigned int& defaultVal = -1) const;
    void setUIntValue(const QString& content, const QString& key, const unsigned int& val);

    //获取int型的配置
    int getIntValue(const QString& content, const QString& key, const int& defaultVal = -1) const;
    void setIntValue(const QString& content, const QString& key, const int& val);

    //获取double型的配置
    double getDoubleValue(const QString& content, const QString& key, const double& defaultVal = 0) const;
    void setDoubleValue(const QString& content, const QString& key, const double& val);

    //获取float型的配置
    float getFloatValue(const QString& content, const QString& key, const float& defaultVal = 0) const;
    void setFloatValue(const QString& content, const QString& key, const float& val);

    //获取qlonglong型的配置
    qlonglong getLongLongValue(const QString& content, const QString& key, const qlonglong& defaultVal = 0) const;
    void setLongLongValue(const QString& content, const QString& key, const qlonglong& val);

    //获取QString型的配置
    QString getStringValue(const QString& content, const QString& key, const QString& defaultVal = QString()) const;
    void setStringValue(const QString& content, const QString& key, const QString& val);

public:
    //获取sa的配置文件目录
    static QString getConfigPath();

    //获取默认config path
    static QString makeDefaultConfigPath();

    //获取sa的配置文件名
    static QString getConfigFileName();

    //获取sa的配置文件名
    static QString getConfigFullPath();

    //获取自定义界面路径
    static QString getRibbonCustomizeFilePath();
public:
    static SAGlobalConfig& getInstance();

private:
    static QString s_configFilePath;        ///< 配置文件路径
    static QString s_configFileName;        ///< 配置文件名
};


#ifndef saConfig
#define saConfig    SAGlobalConfig::getInstance()
#endif

#endif // SAGLOBALCONFIG_H
