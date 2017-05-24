#ifndef SACSVPARSER_H
#define SACSVPARSER_H
#include "SATextParser.h"

class SACsvParser : public SATextParser
{
public:
    SACsvParser(QObject* par = nullptr);
    SACsvParser(const QString& fileName);
    SACsvParser(QFile* device);
    static QString toCsvString(const QString &rawStr);
    static QStringList fromCsvLine(const QString &lineStr);
    QString separator() const{return ",";}

    SACsvParser &append(QChar ch);
    SACsvParser &append(char ch);
    SACsvParser &append(const QString& s);
    SACsvParser &append(QLatin1String s);
    SACsvParser &append(const QByteArray &array);
    SACsvParser &append(const char *c);

    SACsvParser &append(signed short i);
    SACsvParser &append(unsigned short i);
    SACsvParser &append(signed int i);
    SACsvParser &append(unsigned int i);
    SACsvParser &append(signed long i);
    SACsvParser &append(unsigned long i);
    SACsvParser &append(qlonglong i);
    SACsvParser &append(qulonglong i);
    SACsvParser &append(float f);
    SACsvParser &append(double f);
    SACsvParser &append(const QList<QVariant>& rowData);
    SACsvParser &append(const QStringList& rowData);
    SACsvParser &endLine();

    SACsvParser &addSep();

    QStringList readFmtLine(qint64 maxlen=0);

    bool isAtEnd() const;
private:
    static int advquoted(const QString &s, QString &fld, int i);
    static int advplain(const QString &s, QString &fld, int i);
};

#endif // SACSVPARSER_H
