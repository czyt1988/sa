#include "TextImportConfig.h"

#include "SATableVariant.h"
#include "SATableDouble.h"
#include "SAVectorDouble.h"
#include <QFileInfo>
#include "SAValueManager.h"
#include "../../global/SAGlobals.h"
#include "SACsvParser.h"
#include "SALog.h"
#include "SAVectorDouble.h"
TextImportConfig::TextImportConfig(QObject *par)
    :QObject(par)
    ,m_startLine(1)
    ,m_endLine(-1)
    ,m_codec("UTF-8")
    ,m_toOneColumn(false)
    ,m_seperator("")
    ,m_isAutoParser(true)
    ,m_parser(new SATextParser(this))
    ,m_textType(Txt)
{

}

void TextImportConfig::setFile(const QString &filePath)
{
    m_openFilePath = filePath;
    m_parser->setDevice(new QFile(filePath));
}

bool TextImportConfig::open(QIODevice::OpenMode openMode)
{
    return m_parser->open(openMode);
}
///
/// \brief 根据设定创建数据
/// \return
///
QList<std::shared_ptr<SAAbstractDatas> > TextImportConfig::createResultPtr()
{
    QList<std::shared_ptr<SAAbstractDatas> > validDatas;
    for(int i = 0;i<m_data.size();++i)
    {
        if(m_data[i]->getSize() > 0)
        {
            validDatas.append(m_data[i]);
        }
    }
    if(validDatas.size() > 0)
    {
        saValueManager->addDatas(validDatas);
    }
    return validDatas;
}


void TextImportConfig::setRange(int start, int end)
{
   m_startLine = start;
   m_endLine = end;
   if(m_isAutoParser)
        parser();
}

void TextImportConfig::setStartLine(int s)
{
   m_startLine = s;
   if(m_isAutoParser)
   {
       parser();
   }
}

int TextImportConfig::getStartLine() const
{
    return m_startLine;
}

void TextImportConfig::setEndLine(int s)
{
    m_endLine = s;
    if(m_isAutoParser)
    {
        parser();
    }
}

int TextImportConfig::getEndLine() const
{
    return m_endLine;
}

void TextImportConfig::setToOneColumn(bool setOneColumn)
{
     m_toOneColumn = setOneColumn;
     if(m_isAutoParser)
     {
         parser();
     }
}

bool TextImportConfig::isToOneColumn() const
{
    return m_toOneColumn;
}

void TextImportConfig::setCodec(const QString &codex)
{
    m_parser->setCodec(QTextCodec::codecForName(codex.toLocal8Bit()));
    if(m_isAutoParser)
        parser();
}

QString TextImportConfig::getCodec() const
{
    return QString(m_parser->getCodec()->name());
}

bool TextImportConfig::parser()
{

    if(isToOneColumn())
    {
        return parserToOneColumn();
    }
    else
    {
        return parserToMultColumn();
    }
    return false;
}

bool TextImportConfig::parserToMultColumn()
{
    if(!m_parser->isValid())
    {
        return false;
    }
    m_data.clear();
    m_parser->seek(0);
    QString line;
    int lineCount = 0;
    int startLine = getStartLine();
    int endLine = getEndLine();
    bool isOK = false;
    double d;
    saDebug(QString("[start parser] startLine:%1 ,endLine:%2 ").arg(startLine).arg(endLine));
    do{

        line = m_parser->readLine();
        ++lineCount;
        if(startLine > 0)
        {//需要忽略头n行
            if(lineCount<startLine)
                continue;
        }
        if(endLine > 0)
        {//有指定结束范围
            if(lineCount>endLine)
            {
                break;
            }
        }

        QStringList strlist;
        if(Txt == m_textType)
        {
            QString seperator = getSpliter();
            if(seperator.isEmpty ())
            {//没有指定分割
                strlist << line;
            }
            else
            {//指定了分割，先进行分割
                 strlist = line.split (seperator);
            }
        }
        else if(Csv == m_textType)
        {

            strlist =  SACsvParser::fromCsvLine(line);
        }


        int sps = strlist.size ();
        while(m_data.size() < sps)
        {
            m_data.append(
                        std::static_pointer_cast<SAAbstractDatas>(SAValueManager::makeData<SAVectorDouble>()));
        }
        for(int i=0;i<sps;++i)
        {
            d = strlist[i].toDouble(&isOK);
            if(isOK)
            {
                std::static_pointer_cast<SAVectorDouble>(m_data[i])->append(d);
            }
        }
    }while(!line.isNull ());
    emit dataChanged();
    return true;
}

bool TextImportConfig::parserToOneColumn()
{
    if(!m_parser->isValid())
    {
        return false;
    }
    m_data.clear();
    m_parser->seek(0);
    QString line;
    int lineCount = 0;
    int startLine = getStartLine();
    int endLine = getEndLine();
    saDebug(QString("[start parser] startLine:%1 ,endLine:%2 ").arg(startLine).arg(endLine));
    double d = 0;
    bool isOK = false;
    auto pd = SAValueManager::makeData<SAVectorDouble>();
    m_data.append(std::static_pointer_cast<SAAbstractDatas>(pd));
    do{

        line = m_parser->readLine();
        ++lineCount;
        if(startLine > 0)
        {//需要忽略头n行
            if(lineCount<startLine)
                continue;
        }
        if(endLine > 0)
        {//有指定结束范围
            if(lineCount>endLine)
            {
                break;
            }
        }

        QStringList strlist;
        if(Txt == m_textType)
        {
            QString seperator = getSpliter();
            if(seperator.isEmpty ())
            {//没有指定分割
                strlist << line;
            }
            else
            {//指定了分割，先进行分割
                 strlist = line.split (seperator);
            }
        }
        else if(Csv == m_textType)
        {

            strlist =  SACsvParser::fromCsvLine(line);
        }


        int sps = strlist.size ();
        for(int i=0;i<sps;++i)
        {
            d = strlist[i].toDouble(&isOK);
            if(isOK)
            {
                pd->append(d);
            }
        }
    }while(!line.isNull ());
    emit dataChanged();
    return true;
}



TextImportConfig::TextType TextImportConfig::getTextType() const
{
    return m_textType;
}

void TextImportConfig::setTextType(const TextType &textType)
{
    m_textType = textType;
    switch(textType)
    {
        case Txt:m_parser.reset(new SATextParser(this));break;
        case Csv:m_parser.reset(new SACsvParser(this));break;
    }
}

///
/// \brief 获取每列设置的名字
/// \return
///
QStringList TextImportConfig::getColName() const
{
    return m_colName;
}
///
/// \brief 设置每列的，每列的名字
/// isToTable为false和isToOneColumn为false时，应该设置此值，否则会用column%d来替代
/// \param colName
///
void TextImportConfig::setColName(const QStringList &colName)
{
    m_colName = colName;
}

QString TextImportConfig::getRawRow(int row)
{
    if(!m_parser->isValid())
    {
        return QString();
    }
    qint64 prePos = m_parser->pos();
    m_parser->seek(0);
    m_parser->seek(prePos);
    QString line;
    for(int i=0;i<row;++i)
    {
        line = m_parser->readLine();
    }
    line = m_parser->readLine();
    for(int i=0;i < row;++i)
    {
        line = m_parser->readLine();
    }
    return line;

}

QStringList TextImportConfig::getRawConfigRow(int row)
{

    if(!m_parser->isValid())
    {
        return QStringList();
    }
    m_parser->seek(0);
    QString line;
    for(int i=0;i<row;++i)
    {
        line = m_parser->readLine();
    }
    line = m_parser->readLine();

    QStringList strlist;
    if(Txt == m_textType)
    {
        QString seperator = getSpliter();
        if(seperator.isEmpty ())
        {//没有指定分割
            strlist << line;
        }
        else
        {//指定了分割，先进行分割
             strlist = line.split (seperator);
        }
    }
    else if(Csv == m_textType)
    {
        strlist =  SACsvParser::fromCsvLine(line);
    }
    return strlist;
}

bool TextImportConfig::isAutoParser() const
{
    return m_isAutoParser;
}

void TextImportConfig::setAutoParser(bool isAutoParser)
{
    m_isAutoParser = isAutoParser;
}




void TextImportConfig::setColumnSet(QList<bool> colSet)
{
    m_colSet = colSet;
}

QList<bool> TextImportConfig::getColumnSets() const
{
    return m_colSet;
}
///
/// \brief 获取打开的文本文件路径
/// \return
///
QString TextImportConfig::getOpenFilePath() const
{
    return m_openFilePath;
}


TextImportConfig::DataTable TextImportConfig::getData()
{
    return m_data;
}

const TextImportConfig::DataTable TextImportConfig::getData() const
{
    return m_data;
}

const SATextParser *TextImportConfig::getParser() const
{
    return m_parser.get();
}

SATextParser *TextImportConfig::getParser()
{
    return m_parser.get();
}

void TextImportConfig::setSpliter(const QString& spliter)
{
    m_seperator = spliter;
    parser();
}

QString TextImportConfig::getSpliter() const
{
    return m_seperator;
}
