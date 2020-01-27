#ifndef SACONFIGXMLREADWRITER_H
#define SACONFIGXMLREADWRITER_H
#include "SAProtocolGlobal.h"
#include <QObject>
#include <QVariant>
#include <functional>
#include <QIODevice>
class QDomDocument;
class QDomNode;
class SAConfigXMLReadWriterPrivate; 

/**
 * @brief 用于生成xml协议的封装
 */
class SA_PROTOCOL_EXPORT SAConfigXMLReadWriter : public QObject
{
    Q_OBJECT
    SA_IMPL(SAConfigXMLReadWriter)
public:
    SAConfigXMLReadWriter(QObject* par = nullptr);
    SAConfigXMLReadWriter(const QString& filepath,QObject* par = nullptr);
    ~SAConfigXMLReadWriter();
public:
    bool setFilePath(const QString& filePath);
    //检测是否存在目录
    bool isHasGroup(const QString& groupName) const;
    //检测是否存在对应索引
    bool isHasKey(const QString& groupName, const QString& keyName) const;
    //获取键值对应的内容
    QVariant getValue(const QString& groupName, const QString& keyName,const QVariant& defaultVal = QVariant()) const;
    QVariant getValue(const QString& namePath,const QVariant& defaultVal = QVariant()) const;
    //设置键值
    void setValue(const QString& groupName, const QString& keyName, const QVariant& var);
    void setValue(const QString& namePath, const QVariant& var);
    //获取数据，确保数据有默认构造函数
    template<typename T>
    T valueFromKey(const QString& groupName,const QString& keyName,const T& defaultVal = T());
    //获取所有目录关键字
    QStringList getGroupNames() const;
    //获取目录下对应的所有关键字
    QStringList getKeyNames(const QString& groupName) const;
    //判断是否有改动
    bool isDirty() const;
    //保存
    bool save();
    //另存为
    bool saveAs(const QString& filePath);
private:
    static void splitNamePath(const QString& namePaht, QString& groupName, QString& keyName);
};

template<typename T>
T SAConfigXMLReadWriter::valueFromKey(const QString &groupName, const QString &keyName, const T &defaultVal)
{
    QVariant var = getValue(groupName,keyName);
    if(!var.isValid())
        return defaultVal;
    if(!var.canConvert<T>())
        return defaultVal;
    return var.value<T>();
}

#endif // SACONFIGXMLREADWRITER_H
