#include "SAXMLConfig.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QDomNode>
#include <QDebug>
#include "SAVariantCaster.h"
#define CONFIG_FILE_NAME		"saconfig.cfg"

#define CONFIG_ROOT_NAME		"config"
#define CONFIG_GROUP_NAME		"group"
#define CONFIG_CONTENT_PROP_NAME	"name"
#define CONFIG_KEY_NAME			"key"
#define CONFIG_KEY_PROP_NAME_NAME	"name"
#define CONFIG_KEY_PROP_TYPE_NAME	"type"
#define CONFIG_KEY_LIST_NAME		"list"


class SAXMLConfigPrivate
{
    SA_IMPL_PUBLIC(SAXMLConfig)
public:
    SAXMLConfigPrivate(SAXMLConfig *par);
    SAXMLConfigPrivate(SAXMLConfig *par, const QString& cfgPath);
    SAXMLConfigPrivate(const SAXMLConfigPrivate& other, SAXMLConfig *p);
    bool setCfgFile(const QString& cfgPath);
    bool save(const QString& saveFilePath);

public:
    QString m_cfgPath;
    bool m_isDirty;
};


SAXMLConfigPrivate::SAXMLConfigPrivate(SAXMLConfig *par)
    : q_ptr(par)
    , m_isDirty(false)
{
}


SAXMLConfigPrivate::SAXMLConfigPrivate(SAXMLConfig *par, const QString& cfgPath)
    : q_ptr(par)
    , m_isDirty(false)
{
    setCfgFile(cfgPath);
}


SAXMLConfigPrivate::SAXMLConfigPrivate(const SAXMLConfigPrivate& other, SAXMLConfig *p)
{
    this->q_ptr = p;
    this->m_cfgPath = other.m_cfgPath;
    this->m_isDirty = other.m_isDirty;
}


bool SAXMLConfigPrivate::setCfgFile(const QString& cfgPath)
{
    m_cfgPath = cfgPath;
    QFile file(cfgPath);

    if (!file.open(QIODevice::ReadWrite)) {
        return (false);
    }
    if (q_ptr->fromByteArray(file.readAll())) {
        return (true);
    }
    return (false);
}


bool SAXMLConfigPrivate::save(const QString& saveFilePath)
{
    QFile file(saveFilePath);

    if (!file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate)) {//覆盖务必加上QIODevice::Truncate
        return (false);
    }
    QTextStream txt(&file);

    txt.setCodec("UTF-8");
    txt << q_ptr->toString();
    txt.flush();
    file.close();
    m_isDirty = false;
    return (true);
}


SAXMLConfig::SAXMLConfig() : SAXMLProtocol()
    , d_ptr(new SAXMLConfigPrivate(this))
{
}


SAXMLConfig::SAXMLConfig(const QString& filepath) : SAXMLProtocol()
    , d_ptr(new SAXMLConfigPrivate(this, filepath))
{
}


SAXMLConfig::SAXMLConfig(const SAXMLConfig& other) : SAXMLProtocol()
{
    *this = other;
}


SAXMLConfig::SAXMLConfig(SAXMLConfig&& other) : SAXMLProtocol()
{
    this->d_ptr.reset(other.d_ptr.take());
    d_ptr->q_ptr = this;//这个尤为关键
}


SAXMLConfig& SAXMLConfig::operator =(const SAXMLConfig& other)
{
    //SAXMLProtocolParser::operator =(*(static_cast<const SAXMLProtocolParser*>(&other)));
    if (this == (&other)) {
        return (*this);
    }
    SAXMLProtocol::operator =(other);
    this->d_ptr.reset(new SAXMLConfigPrivate(*(other.d_ptr.data()), this));
    //this->d_ptr->q_ptr = this;//这个尤为关键
    return (*this);
}


SAXMLConfig::~SAXMLConfig()
{
}


/**
 * @brief 设置配置文件路径
 * @param filePath
 * @return
 */
bool SAXMLConfig::setFilePath(const QString& filePath)
{
    return (d_ptr->setCfgFile(filePath));
}


/**
 * @brief 获取配置文件路径
 * @return
 */
QString SAXMLConfig::getFilePath() const
{
    return (d_ptr->m_cfgPath);
}


/**
 * @brief 设置内容，调用此函数会使dirty为true
 * @param groupName
 * @param keyName
 * @param var
 */
void SAXMLConfig::setValue(const QString& groupName, const QString& keyName, const QVariant& var)
{
    d_ptr->m_isDirty = true;
    SAXMLProtocol::setValue(groupName, keyName, var);
}


/**
 * @brief 设置内容，调用此函数会使dirty为true
 * @param keyName
 * @param var
 */
void SAXMLConfig::setValue(const QString& keyName, const QVariant& var)
{
    d_ptr->m_isDirty = true;
    SAXMLProtocol::setValue(keyName, var);
}


/**
 * @brief 判断是否有改变
 * @return
 */
bool SAXMLConfig::isDirty() const
{
    return (d_ptr->m_isDirty);
}


/**
 * @brief 保存修改
 * 如果有打开文件，会保存到已有文件路径，如果没有打开文件，此函数不做任何动作
 * @return 成功返回true
 */
bool SAXMLConfig::save()
{
    if (d_ptr->m_cfgPath.isEmpty()) {
        return (false);
    }
    return (saveAs(d_ptr->m_cfgPath));
}


/**
 * @brief 另存为
 * @param filePath 文件路径
 * @return 成功返回true
 */
bool SAXMLConfig::saveAs(const QString& filePath)
{
    return (d_ptr->save(filePath));
}


/**
 * @brief 保护的setProtocolData函数，此函数在此类不能使用，用setFilePath代替
 * @param data
 * @return
 */
bool SAXMLConfig::fromByteArray(const QByteArray& data)
{
    return (SAXMLProtocol::fromByteArray(data));
}


/**
 * @brief SAXMLConfigParser::splitNamePath
 * @param namePath
 * @param groupName
 * @param keyName
 */
void SAXMLConfig::splitNamePath(const QString& namePath, QString& groupName, QString& keyName)
{
    QStringList pl = namePath.split("/");

    if (1 == pl.size()) {
        groupName = "";
        keyName = pl[0];
    }else if (2 == pl.size()) {
        groupName = pl[0];
        keyName = pl[1];
    }else {
        groupName = "";
        keyName = namePath;
    }
}
