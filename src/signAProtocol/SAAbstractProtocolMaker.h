#ifndef SAABSTRACTPROTOCOLMAKER_H
#define SAABSTRACTPROTOCOLMAKER_H
#include <QObject>
#include <QVector>
#include <QPointF>
#include <QVariant>
/**
 * @brief sa协议制造基类，描述了各个接口
 *
 * 必须实现如下接口：
 * @code
 * virtual void setValue(const QString& groupName, const QString& keyName, const QVariant& var);
 * virtual QStringList getGroupNames() const;
 * virtual QStringList getKeyNames(const QString& groupName) const;
 * virtual QString toString() const;
 * @endcode
 *
 * @see SAAbstractProtocolParse
 */
class SAAbstractProtocolMaker : public QObject
{
    Q_OBJECT
public:
    SAAbstractProtocolMaker(QObject* par = nullptr);
    virtual ~SAAbstractProtocolMaker();
public:

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
};

#endif // SAABSTRACTPROTOCOLMAKER_H
