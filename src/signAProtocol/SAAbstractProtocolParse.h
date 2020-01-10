#ifndef SAABSTRACTPROTOCOLPARSE_H
#define SAABSTRACTPROTOCOLPARSE_H
#include <QObject>
/**
 * @brief sa协议解析基类
 * @see SAAbstractProtocolMaker
 */
class SAAbstractProtocolParse : public QObject
{
    Q_OBJECT
public:
    SAAbstractProtocolParse(QObject* par = nullptr);
    virtual ~SAAbstractProtocolParse();
public:
    virtual bool setProtocolData(const QByteArray& data) = 0;

};

#endif // SAABSTRACTPROTOCOLPARSE_H
