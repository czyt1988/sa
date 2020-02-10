#ifndef TSTSIGNAPROTOCOL_H
#define TSTSIGNAPROTOCOL_H
#include <QObject>

class tst_SAVariantCaster : public QObject
{
    Q_OBJECT
public:
    tst_SAVariantCaster();
private Q_SLOTS:
    void testCase_Invalid();
    void testCase_BitArray();
    void testCase_ByteArray();
};

#endif // TSTSIGNAPROTOCOL_H
