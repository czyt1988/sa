#ifndef SAABSTRACTPROTOCOLPARSER_H
#define SAABSTRACTPROTOCOLPARSER_H
#include <QObject>
#include <QVariant>
/**
 * @brief sa协议解析基类
 * @see SAAbstractProtocolMaker
 */
class SAAbstractProtocolParser : public QObject
{
    Q_OBJECT
public:
    SAAbstractProtocolParser(QObject* par = nullptr);
    virtual ~SAAbstractProtocolParser();
public:
    /**
     * @brief 设置协议的内容
     * @param data
     * @return
     */
    virtual bool setProtocolData(const QByteArray& data) = 0;
    /**
     * @brief 设置键值
     * @param groupName 分组名
     * @param keyName 键值名
     * @param var 值
     * @note 斜杠是分组和键值的分割符，因此键值不应该存在斜杠，否则会有无法预料的结果
     */
    virtual void setValue(const QString& groupName, const QString& keyName, const QVariant& var) = 0;
    /**
     * @brief 获取所有目录关键字
     * @return
     */
    virtual QStringList getGroupNames() const = 0;
    /**
     * @brief 获取目录下对应的所有关键字
     * @param groupName
     * @return
     */
    virtual QStringList getKeyNames(const QString& groupName) const = 0;
    /**
     * @brief 转换为文本
     * @return utf8编码的文本
     */
    virtual QString toString() const = 0;
    /**
     * @brief 检测是否存在分组
     * @param groupName 分组名
     * @return
     */
    virtual bool isHasGroup(const QString& groupName) const = 0;
    /**
     * @brief 检查在分组名下是否存在对应的键值
     * @param groupName
     * @param keyName
     * @return
     */
    virtual bool isHasKey(const QString& groupName, const QString& keyName) const = 0;
    /**
     * @brief 获取键值对应的内容
     * @param groupName 分组名
     * @param keyName 键值名
     * @param defaultVal 默认值（如果键值不存在，返回默认值）
     * @return 如果键值不存在，返回默认值，如果存在返回键值对应的内容
     * @note 斜杠是分组和键值的分割符，因此键值不应该存在斜杠，否则会有无法预料的结果
     */
    virtual QVariant getValue(const QString& groupName, const QString& keyName,const QVariant& defaultVal = QVariant()) const = 0;

};

#endif // SAABSTRACTPROTOCOLPARSE_H
