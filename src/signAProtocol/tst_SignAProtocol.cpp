#include <QString>
#include <QtTest>
#include <QVariant>
#include <QByteArray>
#include <QBitArray>
#include <QDataStream>
#include "tst_SignAProtocol.h"
#include "tst_SAXMLProtocolParser.h"

int main(int argc, char *argv[])
{
    tst_SAVariantCaster savc;
    QTEST_SET_MAIN_SOURCE_PATH
    QTest::qExec(&savc, argc, argv);

    tst_SAXMLProtocolParser xmlpro;

    QTest::qExec(&xmlpro, argc, argv);
    return (0);
}
