#include "SAVariantHashData.h"

SAVariantHashData::SAVariantHashData()
{

}
/**
 * @brief 获取固定"value"关键字的值
 * @return "value"关键字对应的值
 */
QVariant SAVariantHashData::getValue() const
{
    return this->value("value");
}
/**
 * @brief 设置固定"value"关键字的值
 * @param 值
 */
void SAVariantHashData::setValue(const QVariant &v)
{
    this->insert("value",v);
}
/**
 * @brief 获取固定"value_type"关键字的值
 * @return "value_type"关键字对应的值，此值固定为int类型，辅助判断value的类型
 */
int SAVariantHashData::getValueType() const
{
    QVariant v = this->value("value_type");
    if(v.isValid())
    {
        bool isOK;
        int r = v.toInt(&isOK);
        if(isOK)
        {
            return r;
        }
    }
    return -1;
}
/**
 * @brief 设置固定"value_type"关键字的值
 * @param 值
 */
void SAVariantHashData::setValueType(int type)
{
    this->insert("value",type);
}
