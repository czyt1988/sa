#include "SAVariantCaster.h"
#include "tst_SignAProtocol.h"
#include <QDebug>
#include <QtTest>
#include <QVariant>
#include <QByteArray>
#include <QBitArray>
#include <QDataStream>


tst_SAVariantCaster::tst_SAVariantCaster() : QObject()
{
}


void tst_SAVariantCaster::testCase_Invalid()
{
    QVariant v;
    QString str;

    str = SAVariantCaster::variantToString(v);
    qDebug() << "=====" << QString(v.typeName());
    QVariant v2 = SAVariantCaster::stringToVariant(str, "Invalid");

    QCOMPARE(v, v2);
}


void tst_SAVariantCaster::testCase_BitArray()
{
    QBitArray ba;

    ba.resize(3);
    ba[0] = true;
    ba[1] = false;
    ba[2] = true;
    QVariant v(ba);

    qDebug() << "=====" << QString(v.typeName());
    QString str = SAVariantCaster::variantToString(v);
    QVariant v2 = SAVariantCaster::stringToVariant(str, "QBitArray");

    QCOMPARE(v, v2);
}


void tst_SAVariantCaster::testCase_ByteArray()
{
    QString str;
    QByteArray ba(100, 0);

    ba[5] = 'a';
    ba[8] = 'A';
    ba[10] = 'B';
    ba[87] = 'A';
    QVariant v(ba);

    qDebug() << "=====" << QString(v.typeName());
    str = SAVariantCaster::variantToString(v);
    QVariant v2 = SAVariantCaster::stringToVariant(str, "QByteArray");
    QByteArray v3 = v2.value<QByteArray>();

    QCOMPARE(ba, v3);
}
