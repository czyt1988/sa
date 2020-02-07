#ifndef QCSVSTREAM_H
#define QCSVSTREAM_H

class QTextStream;
class QFile;
class SACsvStreamPrivate;
#include "SALibGlobal.h"
#include <QString>
///
/// \brief 写csv文件类支持
/// \author czy
/// \date 2016-08-10
///
class SALIB_EXPORT SACsvStream
{
public:
    SACsvStream(QTextStream* txt);
    SACsvStream(QFile* txt);
    virtual ~SACsvStream();
    //转换为标识csv字符
    static QString toCsvString(const QString& rawStr);
    //把一行要用逗号分隔的字符串转换为一行标准csv字符串
    static QString toCsvStringLine(const QStringList& sectionLine);
    //解析一行csv字符
    static QStringList fromCsvLine(const QString &lineStr);
    SACsvStream & operator << (const QString &str);
    SACsvStream & operator << (int d);
    SACsvStream & operator << (double d);
    SACsvStream & operator << (float d);
    //另起一行
    void newLine();
    //获取输入输出流
    QTextStream* streamPtr() const;
    QTextStream& stream();
    const QTextStream& stream() const;
    //读取并解析一行csv字符串
    QStringList readCsvLine();
    //判断是否到文件末端
    bool atEnd() const;
    void flush();
private:
    static int advquoted(const QString &s, QString &fld, int i);
    static int advplain(const QString &s, QString &fld, int i);
private:

    QScopedPointer<SACsvStreamPrivate> d_p;
};
typedef SACsvStream & (*SACsvWriterFunction)(SACsvStream &);
inline SACsvStream &operator<<(SACsvStream &s, SACsvWriterFunction f)
{
    return (*f)(s);
}
SALIB_EXPORT SACsvStream &endl(SACsvStream &s);

#endif // QCSVSTREAM_H
