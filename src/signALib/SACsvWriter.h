#ifndef QCSVWRITER_H
#define QCSVWRITER_H

class QTextStream;
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
    static QString toCsvString(const QString& rawStr);
    static QString toCsvStringLine(const QStringList& sectionLine);
    SACsvWriter & operator << (const QString &str);
    SACsvWriter & operator << (int d);
    SACsvWriter & operator << (double d);
    SACsvWriter & operator << (float d);
    void endLine();
    void endLine(const QString& str);
    void endLine(int d);
    void endLine(double d);
    void endLine(float d);
private:
    QTextStream* m_txt;
};

#endif // QCSVWRITER_H
