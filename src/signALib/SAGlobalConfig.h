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
private:
    explicit SAGlobalConfig();
    ~SAGlobalConfig();
    Q_DISABLE_COPY(SAGlobalConfig)
    //

    void init();
    //检测是否存在目录
    bool isHasContent(const QString& content) const;
    //检测是否存在对应索引
    bool isHasKey(const QString& content,const QString& key) const;
    //获取键值对应的内容
    QVariant getKey(const QString& content,const QString& key) const;
    //设置键值
    void setKey(const QString& content,const QString& key,const QVariant& var);
    //获取数据，确保数据有默认构造函数
    template<typename T>
    T valueFromKey(const QString& content,const QString& key,const T& defaultVal = T());
    //获取所有目录关键字
    QList<QString> getContentList() const;
    //获取目录下对应的所有关键字
    QList<QString> getKeyList(const QString& content) const;
public:
    //获取uint型的配置
    unsigned int getUIntConfig(const QString& content,const QString& key,const unsigned int& defaultVal = -1) const;
    void setUIntConfig(const QString& content,const QString& key,const unsigned int& val);
    //获取int型的配置
    int getIntConfig(const QString& content,const QString& key,const int& defaultVal = -1) const;
    void setIntConfig(const QString& content,const QString& key,const int& val);
    //获取double型的配置
    double getDoubleConfig(const QString& content,const QString& key,const double& defaultVal = 0) const;
    void setDoubleConfig(const QString& content,const QString& key,const double& val);
    //获取float型的配置
    float getFloatConfig(const QString& content,const QString& key,const float& defaultVal = 0) const;
    void setFloatConfig(const QString& content,const QString& key,const float& val);
    //获取qlonglong型的配置
    qlonglong getLongLongConfig(const QString& content,const QString& key,const qlonglong& defaultVal = 0) const;
    void setLongLongConfig(const QString& content,const QString& key,const qlonglong& val);
    //获取QString型的配置
    QString getStringConfig(const QString& content,const QString& key,const QString& defaultVal = QString()) const;
    void setStringConfig(const QString& content,const QString& key,const QString& val);
public:
    //获取sa的home page
    QString getHomePath() const;
    //获取sa的配置文件目录
    QString getConfigPath() const;
    //获取默认home path
    static QString makeDefaultHomePath();
    //获取默认config path
    static QString makeDefaultConfigPath();
public:
    static SAGlobalConfig* getInstance();
private:
    std::unique_ptr<SAGlobalConfigPrivate> m_d;///<
    static SAGlobalConfig* s_instance;
};

template<typename T>
T SAGlobalConfig::valueFromKey(const QString &content, const QString &key,const T& defaultVal)
{
    if(!isHasKey(content,key))
    {
        return defaultVal;
    }
    QVariant var = getKey(content,key);
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
