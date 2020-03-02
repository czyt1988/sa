#ifndef TEXTIMPORTCONFIG_H
#define TEXTIMPORTCONFIG_H
#include <QString>
#include <QList>
#include "SATextParser.h"
#include "SATableVariant.h"
class SATextParser;
class SAAbstractDatas;
class QFile;

class TextImportConfig : public QObject
{
    Q_OBJECT
public:
    TextImportConfig(QObject* par = nullptr);
    typedef QList<std::shared_ptr<SAAbstractDatas> >  DataTable;
    //set device,it will delete old device
    void setFile(const QString& filePath);
    bool open(QIODevice::OpenMode openMode = QIODevice::ReadWrite|QIODevice::Text);
    QList<std::shared_ptr<SAAbstractDatas> > createResultPtr();
    enum TextType
    {
        Txt
        ,Csv
    };

public:
    //设置忽略的
     void setRange(int start,int end);
     //
     void setStartLine(int s);
     int getStartLine() const;
     //
     void setEndLine(int s);
     int getEndLine() const;
     //设置读取为1行
     void setToOneColumn(bool setOneColumn);
     bool isToOneColumn() const;
     //设置编码格式
     void setCodec(const QString& codex);
     QString getCodec() const;

     //设置导出系列的列，某些不导出
     void setColumnSet(QList<bool> colSet);
     QList<bool> getColumnSets() const;

     //获取打开的文件路径
     QString getOpenFilePath() const;

     virtual DataTable getData();
     virtual const DataTable getData() const;

     SATextParser *getParser();
     const SATextParser *getParser() const;

     void setSpliter(const QString& spliter);
     QString getSpliter() const;


     bool isAutoParser() const;
     void setAutoParser(bool isAutoParser);

     //设置每列的名字
     QStringList getColName() const;
     void setColName(const QStringList &colName);

     //
     QString getRawRow(int row);
     QStringList getRawConfigRow(int row);
     TextType getTextType() const;
     void setTextType(const TextType &textType);

public slots:
     virtual bool parser();
private:
     bool parserToMultColumn();
     bool parserToOneColumn();
signals:
     void dataChanged();
private:
     int m_startLine;///< 开始行，若为-1，则代表从第一行开始
     int m_endLine;///< 结束行，若为-1,则代表最后一行
     QString m_codec;///<  编码
     bool m_toOneColumn;///< 转换为一列

     QList<bool> m_colSet;
     QString m_openFilePath;///< 打开的文件路径
     QString m_seperator;///< 分隔符
     bool m_isAutoParser;///< 是否自动刷新
     QStringList m_colName;///< 每列的名字，在setColumnSets后需要设置此值
private:
    // DataTable m_data;
     QList<std::shared_ptr<SAAbstractDatas> > m_data;
     std::unique_ptr< SATextParser > m_parser;
     TextType m_textType;///<
};

#endif // TEXTIMPORTCONFIG_H
