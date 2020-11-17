#include "SAProperties.h"

/**
 * @brief 获取属性
 * @param key 键值
 * @return 如果不存在键值返回QVariant(),如果存在返回对应属性
 */
QVariant SAProperties::getProperty(const QString& key) const
{
    return (value(key, QVariant()));
}


/**
 * @brief 获取属性
 * @param key 键值
 * @param defaultProperty 默认值
 * @return 如果不存在键值返回defaultProperty,如果存在返回对应属性
 */
QVariant SAProperties::getProperty(const QString& key, const QVariant& defaultProperty) const
{
    return (value(key, defaultProperty));
}


/**
 * @brief 设置属性
 * @param key 键值
 * @param value 值
 */
void SAProperties::setProperty(const QString& key, const QVariant& value)
{
    insert(key, value);
}


/**
 * @brief 获取属性
 * @param group 分组
 * @param key 键值
 * @return 如果不存在分组或者键值返回QVariant(),如果存在返回对应属性
 */
QVariant SAPropertiesGroup::getProperty(const QString& group, const QString& key) const
{
    return (getProperty(group, key, QVariant()));
}


/**
 * @brief 获取属性
 * @param group 分组
 * @param key 键值
 * @param defaultProperty 默认值
 * @return 如果不存在分组或者键值返回defaultProperty,如果存在返回对应属性
 */
QVariant SAPropertiesGroup::getProperty(const QString& group, const QString& key, const QVariant& defaultProperty) const
{
    auto i = find(group);

    if (i == end()) {
        return (defaultProperty);
    }
    return (i.value().getProperty(key, defaultProperty));
}


/**
 * @brief 获取一组属性
 * @param group 组
 * @return 一组属性的引用，如果不存在这个分组，会触发断言
 * @sa hasGroup
 */
const SAProperties& SAPropertiesGroup::constProperties(const QString& group) const
{
    auto i = find(group);

    Q_ASSERT_X(i != end(), "getProperties", "getProperties receive invalid group");
    return (i.value());
}


/**
 * @brief 获取一组属性的引用，如果没有，会插入一个默认属性
 * @param group 组
 * @return 一组属性的引用，如果不存在这个分组，会插入一个默认属性
 * @sa hasGroup
 */
SAProperties& SAPropertiesGroup::properties(const QString& group)
{
    auto i = find(group);

    if (i == end()) {
        i = insert(group, SAProperties());
    }
    return (i.value());
}


/**
 * @brief 获取一组属性
 * @param group 属性组
 * @return 如果没有，返回一个SAProperties()
 */
SAProperties SAPropertiesGroup::getProperties(const QString& group)
{
    return (value(group));
}


/**
 * @brief 设置一组属性
 * @param group 分组名
 * @param propertys 属性值
 */
void SAPropertiesGroup::setProperties(const QString& group, const SAProperties& propertys)
{
    insert(group, propertys);
}


/**
 * @brief 设置属性
 *
 * 会自动创建不存在的组
 * @param group 组
 * @param key 键值
 * @param value 值
 */
void SAPropertiesGroup::setProperty(const QString& group, const QString& key, const QVariant& value)
{
    properties(group).insert(key, value);
}


/**
 * @brief 判断是否存在分组
 * @param group 分组名
 * @return
 */
bool SAPropertiesGroup::hasGroup(const QString& group)
{
    return (contains(group));
}
