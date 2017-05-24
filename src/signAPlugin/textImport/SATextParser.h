#ifndef SATEXTPARSER_H
#define SATEXTPARSER_H
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <memory>
///
/// \brief 文本解析Parse
///
/// 内部自动包含QFile，包含写文件和读文件操作
///
class SATextParser : public QObject
{
    Q_OBJECT
public:
    SATextParser(QObject* par = nullptr);
    //
    SATextParser(const QString& fileName);
    //
    SATextParser(QFile* device);
    //
    bool open(QIODevice::OpenMode openMode = QIODevice::ReadOnly|QIODevice::Text);
    bool open(const QString& filePath,QIODevice::OpenMode openMode = QIODevice::ReadOnly|QIODevice::Text);
    //设置device此时将拥有device的所有权SATextParser have the device owner ship
    void setDevice(QFile* device);
    //放弃QFile的所有权
    QFile* takeDevice();
    //
    QString getFileName() const;
    //
    void setFileName(const QString& fileName);

    //读取一行
    QString readLine(qint64 maxlen = 0);
    //对上一个读取的行进行切分
    virtual QStringList readAndSplitLine(const QString& split);

    //
    bool isOpen() const;
    //
    QFile* getFilePtr();
    //
    QStringList readAllLine();
    //
    void	 setCodec(QTextCodec * codec);

    QTextCodec* getCodec() const;
    //
    bool seek(qint64 pos);
    qint64 pos() const;
    //
    bool isValid() const;

    //
    QTextStream& getStream();
    //
    const QTextStream& getStream() const;

private:

    std::unique_ptr<QFile> m_file;
    QTextStream m_txt; 
    qint64 m_prePos;
};

#endif // SATEXTPARSER_H
