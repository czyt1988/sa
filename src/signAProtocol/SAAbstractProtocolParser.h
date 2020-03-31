#ifndef SAABSTRACTPROTOCOLPARSER_H
#define SAABSTRACTPROTOCOLPARSER_H
#include <QVariant>
/**
 * @brief sa协议解析基类
 * @see SAAbstractProtocolMaker
 */
class SAAbstractProtocolParser
{
public:
    SAAbstractProtocolParser();
    virtual ~SAAbstractProtocolParser();
public:
    /**
     * @brief setFunctionID 设置协议功能号
     * @param funid 功能号
     */
    virtual void setFunctionID(int funid) = 0;
    /**
     * @brief getFunctionID 获取协议功能号
     * @return
     */
    virtual int getFunctionID() const = 0;
    /**
     * @brief setClassID 设置协议类号
     * @param classid
     */
    virtual void setClassID(int classid) = 0;
    /**
     * @brief getClassID 获取协议类号
     * @return
     */
    virtual int getClassID() const = 0;
    /**
     * @brief 设置协议的内容
     * @param data
     * @return
     * @see toByteArray
     */
    virtual bool fromByteArray(const QByteArray& data) = 0;
    /**
     * @brief 从字符串转换到协议
     * @param str
     * @return
     * @see toString
     */
    virtual bool fromString(const QString& str) = 0;
    /**
     * @brief 设置键值
     * @param groupName 分组名
     * @param keyName 键值名
     * @param var 值
     * @note 斜杠是分组和键值的分割符，因此键值不应该存在斜杠，否则会有无法预料的结果
     */
    virtual void setValue(const QString& groupName, const QString& keyName, const QVariant& var) = 0;
    virtual void setValue(const QString& keyName, const QVariant& var) = 0;
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
     * @brief 转换为bytearray
     * @return
     * @see fromByteArray
     */
    virtual QByteArray toByteArray() const = 0;
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
     */
    virtual QVariant getValueInGroup(const QString& groupName, const QString& keyName,const QVariant& defaultVal = QVariant()) const = 0;
    /**
     * @brief 从默认分组获取键值对应的内容
     * @param keyName 键值名
     * @param defaultVal 默认值（如果键值不存在，返回默认值）
     * @return
     */
    virtual QVariant getValue(const QString& keyName,const QVariant& defaultVal = QVariant()) const = 0;

};

#endif // SAABSTRACTPROTOCOLPARSE_H
