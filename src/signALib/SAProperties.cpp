#include "SAProperties.h"

SAProperties::SAProperties()
{
}


QVariant SAProperties::getProperty(const QString& key) const
{
    return (value(key));
}


QVariant SAProperties::getProperty(const QString& key, const QVariant& defaultProperty) const
{
    return (value(key, defaultProperty));
}


void SAProperties::setProperty(const QString& key, const QVariant& value)
{
    insert(key, value);
}
