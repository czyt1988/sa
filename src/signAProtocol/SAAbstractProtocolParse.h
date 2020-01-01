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
    virtual bool setProtocolData(const QByteArray& data);
signals:
    /**
     * @brief 解析出2维点群的统计结果
     */
    void parse2DPointsStatistic(int key,);
};

#endif // SAABSTRACTPROTOCOLPARSE_H
