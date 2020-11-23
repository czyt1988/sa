#include "tst_SAXMLProtocolParser.h"
#include <QtTest/QtTest>
#include "SAVariantCaster.h"
#include <QPoint>
#include <QPointF>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QTextStream>
tst_SAXMLProtocolParser::tst_SAXMLProtocolParser() : QObject()
{
}


void tst_SAXMLProtocolParser::testXMLProtocol()
{
    QString str;
    SAXMLProtocolParser write;
    int classid = 123;
    int funid = 4565;

    write.setClassID(classid);
    write.setFunctionID(funid);
    QString name = "signa analysis";

    write.setValue("sa", "name", name);
    int inttest = 1;

    write.setValue("sa", "int test", inttest);
    int inttest2 = 10000;

    write.setValue("sa", "int test2", inttest2);

    double dtest1 = 1.1, dtest2 = 1019299.11232
    , dtestlarge = 2323123456789.123445698098, dtestsmall = 2.12321232980093e-45;

    write.setValue("sa", "double test", dtest1);
    write.setValue("sa", "double test2", dtest2);
    write.setValue("sa", "double large", dtestlarge);
    write.setValue("sa", "double small", dtestsmall);
    write.setValue("sa", "point test", QPoint(123, 456));
    write.setValue("sa", "pointf test", QPointF(123.456, 7890.123));
    QStringList strlist = QStringList()<< "1" << "2" << "3" << "word" << QStringLiteral("中文");

    write.setValue("sa", "stringlist test", strlist);
    QHash<QString, QVariant> hashtest;

    hashtest["int"] = 12;
    hashtest["float"] = 12.23;
    hashtest["string"] = "sa";
    hashtest["point"] = QPoint(123, 23);
    hashtest["stringlist"] = strlist;
    write.setValue("sa", "hash test", hashtest);
    QHash<QString, QVariant> hashtest2;

    hashtest2 = hashtest;
    hashtest2["hash"] = hashtest;
    write.setValue("sa", "hash test2", hashtest2);

    QVector<QPointF> vpf;

    for (int i = 0; i < 100; ++i)
    {
        vpf.append(QPointF(i*0.1, i*i*0.3));
    }
    write.setValue("sa", "QVector<QPointF>", QVariant::fromValue(vpf));

    write.setValue("default int", 123);
    write.setValue("default hashtest", hashtest);

    QString protocolstring = write.toString();

    QFile file("./tst_SAXMLProtocolParser.xml");

    file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    QTextStream txt(&file);

    txt << protocolstring;
    txt.flush();

    qDebug() << "protocol xml save to:" << file.fileName();
    file.close();

    SAXMLProtocolParser reader;

    if (!reader.fromString(protocolstring)) {
        qDebug() << "invalid read";
        return;
    }
    QCOMPARE(reader.getClassID(), classid);
    QCOMPARE(reader.getFunctionID(), funid);
    QCOMPARE(reader.getValue("sa", "name").toString(), name);
    QCOMPARE(reader.getValue("sa", "int test").toInt(), inttest);
    QCOMPARE(reader.getValue("sa", "int test2").toInt(), inttest2);
    QVERIFY(saFuzzyCompare(reader.getValue("sa", "double test").toDouble(), dtest1));
    double b = reader.getValue("sa", "double test2").toDouble();

    qDebug() << "double test2" << dtest2;
    qDebug() << "double test2 read:" << b;
    qDebug() << "qAbs(dtest2 - b)" << qAbs(dtest2-b);
    qDebug() << "(qAbs(p1 - p2) * 1000000000000.:" << qAbs(dtest2 - b) * 1000000000000.;
    QVERIFY(saFuzzyCompare(reader.getValue("sa", "double test2").toDouble(), dtest2));
    b = reader.getValue("sa", "double large").toDouble();
    QVERIFY(qFuzzyCompare(reader.getValue("sa", "double large").toDouble(), dtestlarge));
    QVERIFY(saFuzzyCompare(reader.getValue("sa", "double small").toDouble(), dtestsmall));
    QCOMPARE(reader.getValue("sa", "stringlist test").toStringList(), strlist);
}
