#ifndef SAPROPERTIES_H
#define SAPROPERTIES_H
#include <QHash>
#include <QVariant>
#include "SAProtocolGlobal.h"

/**
 * @brief 参考Java Properties 类封装的属性类，负责SA的属性传递
 *
 *
 */
class SA_PROTOCOL_EXPORT SAProperties : public QVariantHash
{
public:
    SAProperties() = default;

    //获取属性
    QVariant getProperty(const QString& key) const;
    QVariant getProperty(const QString& key, const QVariant& defaultProperty) const;

    //设置属性
    void setProperty(const QString& key, const QVariant& value);
};


/**
 * @brief 属性组
 *
 */
class SA_PROTOCOL_EXPORT SAPropertiesGroup : public QHash<QString, SAProperties>
{
public:
    SAPropertiesGroup() = default;
    //获取属性
    QVariant getProperty(const QString& group, const QString& key) const;
    QVariant getProperty(const QString& group, const QString& key, const QVariant& defaultProperty) const;

    //获取一组属性，必须先确保有这个分组
    const SAProperties& constProperties(const QString& group) const;

    //获取一组属性的引用，如果没有，会插入一个默认属性
    SAProperties& properties(const QString& group);

    //获取一组属性
    SAProperties getProperties(const QString& group);

    //设置一组属性
    void setProperties(const QString& group, const SAProperties& propertys);

    //设置属性
    void setProperty(const QString& group, const QString& key, const QVariant& value);

    //判断是否存在分组
    bool hasGroup(const QString& group);
};

#endif // SAPROPERTIES_H
