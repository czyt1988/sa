#include "SAGlobalConfig.h"
#include <QMutex>
#include <QHash>
#include "SAGlobalConfigDefine.h"
#include <QSet>
#include <QDir>
#include <QFileInfo>
#include "SAConfigXMLReadWriter.h"
#include <QApplication>
SAGlobalConfig* SAGlobalConfig::s_instance = nullptr;

class SAGlobalConfigPrivate
{
public:
    SAGlobalConfigPrivate();
    typedef QHash<QString,QVariant> ConfigDict;
    typedef QHash<QString,ConfigDict> ConfigContent;
    friend class SAGlobalConfig;

    QList<QString> getContentList() const;
    QList<QString> getKeyList(const QString& content) const;
    void init();
    unsigned int pointsCurve2p;///< 曲线点数小于此值，曲线的默认线宽为2

    bool isHasContent(const QString& content) const;
    bool isHasKey(const QString& content,const QString& key) const;
    QVariant getKey(const QString& content,const QString& key) const;
    void setKey(const QString& content,const QString& key,const QVariant& var);
private:
    ConfigContent m_configContentDict;

};

SAGlobalConfigPrivate::SAGlobalConfigPrivate()
{
}

QList<QString> SAGlobalConfigPrivate::getContentList() const
{
    return m_configContentDict.keys();
}

QList<QString> SAGlobalConfigPrivate::getKeyList(const QString &content) const
{
    if(!isHasContent(content))
    {
        return QList<QString>();
    }
    return m_configContentDict[content].keys();
}



bool SAGlobalConfigPrivate::isHasContent(const QString &content) const
{
    return m_configContentDict.contains(content);
}

bool SAGlobalConfigPrivate::isHasKey(const QString &content, const QString &key) const
{
    if(!isHasContent(content))
        return false;
    return m_configContentDict[content].contains(key);
}

QVariant SAGlobalConfigPrivate::getKey(const QString &content, const QString &key) const
{
    if(!isHasContent(content))
        return QVariant();
    return m_configContentDict[content].value(key,QVariant());
}

void SAGlobalConfigPrivate::setKey(const QString &content, const QString &key, const QVariant &var)
{

    m_configContentDict[content][key] = var;
}
void SAGlobalConfigPrivate::init()
{
    m_configContentDict[CFG_CONTENT_ROOT][CFG_ROOT_HomePath]
            = SAGlobalConfig::makeDefaultHomePath();
    m_configContentDict[CFG_CONTENT_ROOT][CFG_ROOT_ConfigFolderPath]
            = SAGlobalConfig::makeDefaultConfigPath();
}

//============================================

SAGlobalConfig::SAGlobalConfig()
    :m_d(new SAGlobalConfigPrivate)
{
    init();
    SAConfigXMLReadWriter xmlReadWriter(this);
    xmlReadWriter.startRead();
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
/// \brief 初始化函数
///
void SAGlobalConfig::init()
{
    m_d->init();
}
///
/// \brief 检测是否存在目录
/// \param content 目录名称
/// \return
///
bool SAGlobalConfig::isHasContent(const QString &content) const
{
    return m_d->isHasContent(content);
}
///
/// \brief 检测是否存在对应索引
/// \param content 目录名称
/// \param key 索引名称
/// \return
///
bool SAGlobalConfig::isHasKey(const QString &content, const QString &key) const
{
    return m_d->isHasKey(content,key);
}
///
/// \brief 获取键值对应的内容
/// \param content 目录名称
/// \param key 索引名称
/// \return 如果没有内容，返回为QVariant(),可以通过isValid判断
///
QVariant SAGlobalConfig::getValue(const QString &content, const QString &key) const
{
    return m_d->getKey(content,key);
}
///
/// \brief 设置内容
/// \param content 目录名称
/// \param key 索引名称
/// \param var 值
///
void SAGlobalConfig::setValue(const QString &content, const QString &key, const QVariant &var)
{
    return m_d->setKey(content,key,var);
}
///
/// \brief 获取所有目录关键字
/// \return 目录关键字列表
///
QList<QString> SAGlobalConfig::getContentList() const
{
    return m_d->getContentList();
}
///
/// \brief 获取目录下对应的所有关键字
/// \param content 目录名
/// \return 关键字列表
///
QList<QString> SAGlobalConfig::getKeyList(const QString &content) const
{
    return m_d->getKeyList(content);
}


#define SET_XX_CONFIG(type,TypeFunName) \
    void SAGlobalConfig::TypeFunName(const QString& content,const QString& key,const type& val)\
    {\
        setValue(content,key,val);\
    }

#define GET_XX_CONFIG(type,TypeFunName,varToFunName) \
    type SAGlobalConfig::TypeFunName(const QString &content, const QString &key,const type& defaultVal) const\
    {\
        if(!isHasKey(content,key))\
        {\
            return defaultVal;\
        }\
        QVariant var = getValue(content,key);\
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
    type SAGlobalConfig::TypeFunName(const QString &content, const QString &key,const type& defaultVal) const\
    {\
        if(!isHasKey(content,key))\
        {\
            return defaultVal;\
        }\
        QVariant var = getValue(content,key);\
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
//GET_XX_CONFIG(int,getIntValue,toInt)
int SAGlobalConfig::getIntValue(const QString &content, const QString &key,const int& defaultVal) const
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
    bool isOK = false;
    int res = var.toInt(&isOK);
    if(!isOK)
    {
        return defaultVal;
    }
    return res;
}

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
/// \brief 获取sa的home page
/// \return 基本目录
///
QString SAGlobalConfig::getHomePath() const
{
    return getStringValue(CFG_CONTENT_ROOT,CFG_ROOT_HomePath,makeDefaultHomePath());
}
///
/// \brief 获取sa的配置文件目录
/// \return 配置文件目录
///
QString SAGlobalConfig::getConfigPath() const
{
    return getStringValue(CFG_CONTENT_ROOT,CFG_ROOT_ConfigFolderPath,makeDefaultConfigPath());
}
///
/// \brief 获取默认home path
/// \return 如果目录没有会尝试创建一个
///
QString SAGlobalConfig::makeDefaultHomePath()
{
    QString saHomePath = QApplication::applicationDirPath();
    saHomePath = saHomePath + QDir::separator() + "SA";
    if(!QFileInfo::exists(saHomePath))
    {
        QDir dir(saHomePath);
        dir.mkpath(saHomePath);
    }
    return saHomePath;
}
///
/// \brief 获取默认config path
/// \return 如果目录没有会尝试创建一个
///
QString SAGlobalConfig::makeDefaultConfigPath()
{
    QString cfgPath = makeDefaultHomePath();
    cfgPath = cfgPath + QDir::separator() + "config";
    if(!QFileInfo::exists(cfgPath))
    {
        QDir dir(cfgPath);
        dir.mkpath(cfgPath);
    }
    return cfgPath;
}




