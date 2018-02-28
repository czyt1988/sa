#ifndef QCSVWRITER_H
#define QCSVWRITER_H

class QTextStream;
class QFile;
class SACsvWriterPrivate;
#include "SALibGlobal.h"
#include <QString>
///
/// \brief 写csv文件类支持
/// \author czy
/// \date 2016-08-10
///
class SALIB_EXPORT SACsvWriter
{
public:
    SACsvWriter(QTextStream* txt);
    SACsvWriter(QFile* txt);
    virtual ~SACsvWriter();
    static QString toCsvString(const QString& rawStr);
    static QString toCsvStringLine(const QStringList& sectionLine);
    SACsvWriter & operator << (const QString &str);
    SACsvWriter & operator << (int d);
    SACsvWriter & operator << (double d);
    SACsvWriter & operator << (float d);
    void newLine();
    QTextStream* stream() const;
private:

    QScopedPointer<SACsvWriterPrivate> d_p;
};
typedef SACsvWriter & (*SACsvWriterFunction)(SACsvWriter &);
inline SACsvWriter &operator<<(SACsvWriter &s, SACsvWriterFunction f)
{
    return (*f)(s);
}
SALIB_EXPORT SACsvWriter &endl(SACsvWriter &s);

#endif // QCSVWRITER_H
