#ifndef SAVARIANTHASHDATA_H
#define SAVARIANTHASHDATA_H
#include "SAProcessGlobal.h"
#include <QVariant>

/**
 * @brief 用于存储结果
 *
 * 此类用于存储无法预知内容的结果
 * 其中已有关键字“value”，“value_type”,通过 @sa getValue setValue
 */
class SA_PROCESS_EXPORT SAVariantHashData : public QVariantHash
{
public:
    SAVariantHashData();
    //固定键值value
    QVariant getValue() const;
    void setValue(const QVariant& v);
    //固定键值value_type
    int getValueType() const;
    void setValueType(int type);

};

Q_DECLARE_METATYPE(SAVariantHashData)
#endif // SAVARIANTHASHDATA_H
