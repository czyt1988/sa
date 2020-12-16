#ifndef SAXMLCONFIG_H
#define SAXMLCONFIG_H
#include "SAProtocolGlobal.h"
#include <QVariant>
#include "SAXMLProtocol.h"
class SAXMLConfigPrivate;

/**
 * @brief 用于生成xml配置文件的类
 */
class SA_PROTOCOL_EXPORT SAXMLConfig : public SAXMLProtocol
{
    SA_IMPL(SAXMLConfig)
public:
    SAXMLConfig();
    SAXMLConfig(const QString& filepath);
    SAXMLConfig(const SAXMLConfig& other);
    SAXMLConfig(SAXMLConfig&& other);
    SAXMLConfig& operator =(const SAXMLConfig& other);

    ~SAXMLConfig();
public:
    bool setFilePath(const QString& filePath);
    QString getFilePath() const;

    //设置内容，调用此函数会使dirty为true
    virtual void setValue(const QString& groupName, const QString& keyName, const QVariant& var) override;
    virtual void setValue(const QString& keyName, const QVariant& var) override;

    //判断是否有改动
    bool isDirty() const;

    //保存
    bool save();

    //另存为
    bool saveAs(const QString& filePath);

protected:
    // 设置协议的内容
    virtual bool fromByteArray(const QByteArray& data) override;

private:
    static void splitNamePath(const QString& namePaht, QString& groupName, QString& keyName);
};

#endif // SACONFIGXMLREADWRITER_H
