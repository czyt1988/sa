#ifndef SAPROPERTIES_H
#define SAPROPERTIES_H
#include <QHash>
#include <QVariant>
#include "SALibGlobal.h"

/**
 * @brief 参考Java Properties 类封装的属性类，负责SA的属性传递
 */
class SALIB_EXPORT SAProperties : public QVariantHash
{
public:
    SAProperties();

    //获取属性
    QVariant getProperty(const QString& key) const;
    QVariant getProperty(const QString& key, const QVariant& defaultProperty) const;

    //设置属性
    void setProperty(const QString& key, const QVariant& value);
};



#endif // SAPROPERTIES_H
