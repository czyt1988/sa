#include "SACsvParser.h"

SACsvParser::SACsvParser(QObject *par):SATextParser(par)
{

}

SACsvParser::SACsvParser(const QString &fileName)
    :SATextParser(fileName)
{

}

SACsvParser::SACsvParser(QFile *device)
    :SATextParser(device)
{

}

///
/// \brief 把字符串装换为标准csv一个单元得字符串，对应字符串原有的逗号会进行装换
///
/// csv的原则是：
///
/// - 如果字符串有逗号，把整个字符串前后用引号括起来
/// - 如果字符串有引号",引号要用两个引号表示转义""
/// \param rawStr 原有数据
/// \return 标准的csv单元字符串
///
QString SACsvParser::toCsvString(const QString &rawStr)
{
    //首先查找有没有引号,如果有，则把引号替换为两个引号
    QString res = rawStr;
    res.replace("\"","\"\"");
    if(res.contains(','))
    {
        //如果有逗号，在前后把整个句子用引号包含
        res = ('\"' + res + '\"');
    }
    return res;
}
///
/// \brief DCsvStream::fromCsvLine
/// \param line
/// \return
///
QStringList SACsvParser::fromCsvLine(const QString &lineStr)
{
    if(lineStr.isEmpty())
    {
        return QStringList();
    }
    QStringList res;
    QString str;
    int i, j=0;
    int col = 0;
    i = 0;
    do {
        if (i < lineStr.length() && lineStr[i] == '\"')
            j = advquoted(lineStr, str, ++i); // skip quote
        else
            j = advplain(lineStr, str, i);
        res.push_back (str);
        ++col;
        i = j + 1;
    } while (j < lineStr.length());
    return res;
}

int SACsvParser::advquoted(const QString& s, QString& fld, int i)
{
    int j;

    fld = "";
    for (j = i; j < s.length()-1; j++)
    {
        if (s[j] == '\"' && s[++j] != '\"')
        {
            int k = s.indexOf (',', j);
            if (k < 0 ) // no separator found
                k = s.length();
            for (k -= j; k-- > 0; )
                fld += s[j++];
            break;
        }
        fld += s[j];
    }
    return j;
}

int SACsvParser::advplain(const QString& s, QString& fld, int i)
{
    int j;

    j = s.indexOf(',', i); // look for separator
    if (j < 0) // none found
        j = s.length();
    fld = s.mid (i,j-i);//string(s, i, j-i);
    return j;
}

SACsvParser& SACsvParser::append(QChar ch)
{
    if(ch == ',')
    {
        QString str = ch;
        str = toCsvString(str);
        getStream()<<(str);
    }
    getStream()<<(ch);
    return *this;
}

SACsvParser &SACsvParser::append(char ch)
{
    if(ch == ',')
    {
        QString str(ch);
        str = toCsvString(str);
        getStream() <<(str);
    }
    getStream() <<(ch);
    return *this;

}

SACsvParser &SACsvParser::append(const QString &s)
{
    getStream() <<(toCsvString(s));
    return *this;
}

SACsvParser &SACsvParser::append(QLatin1String s)
{
    QString str(s);
    getStream() <<(toCsvString(str));
    return *this;
}

SACsvParser &SACsvParser::append(const QByteArray &array)
{
    QString str(array);
    getStream() <<(toCsvString(str));
    return *this;
}

SACsvParser &SACsvParser::append(const char *c)
{
    QString str(c);
    getStream() <<(toCsvString(str));
    return *this;
}

SACsvParser &SACsvParser::append(signed short i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(unsigned short i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(signed int i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(unsigned int i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(signed long i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(unsigned long i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(qlonglong i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(qulonglong i)
{
    getStream() <<(i);
    return *this;
}

SACsvParser &SACsvParser::append(float f)
{
    getStream() <<(f);
    return *this;
}

SACsvParser &SACsvParser::append(double f)
{
    getStream() <<(f);
    return *this;
}

SACsvParser &SACsvParser::append(const QList<QVariant> &rowData)
{
    const int size_1 = rowData.size()-1;
    for(int i=0;i<size_1;++i)
    {
        const QVariant& var = rowData.at(i);
        append(var.toString()).addSep();
    }
    if(size_1>=0)
    {
        append(rowData.back().toString());
    }
    return *this;
}

SACsvParser &SACsvParser::append(const QStringList &rowData)
{
    const int size_1 = rowData.size()-1;
    for(int i=0;i<size_1;++i)
    {
        append(rowData.at(i)).addSep();
    }
    if(size_1>=0)
    {
        append(rowData.back());
    }
    return *this;
}

SACsvParser &SACsvParser::endLine()
{
    getStream()<<endl;
//#ifdef Q_OS_WIN
//    getStream() << ("\r\n");
//#else
//    getStream() <<(QLatin1Char('\n'));
//#endif
//    getStream().flush();
    return *this;
}

SACsvParser &SACsvParser::addSep()
{
    getStream() << separator();
    return *this;
}

QStringList SACsvParser::readFmtLine(qint64 maxlen)
{
    return fromCsvLine(SATextParser::readLine(maxlen));
}

bool SACsvParser::isAtEnd() const
{
   if(isValid())
   {
       return getStream().atEnd();
   }
   return false;
}
