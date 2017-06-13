#ifndef SACONFIGXMLREADWRITER_H
#define SACONFIGXMLREADWRITER_H
#include <QObject>
#include <SAGlobals.h>
#include <QHash>
#include <QVariant>
#include <functional>
#include <QIODevice>
class SAGlobalConfig;
class QXmlStreamReader;
class QXmlStreamWriter;
///
/// \brief 用于读写SAConfig的xml类，可用于多线程
///
class SAConfigXMLReadWriter : public QObject
{
    Q_OBJECT
    Q_ENUMS(CompleteInfo)
public:
    SAConfigXMLReadWriter(SAGlobalConfig* config,QObject* par = nullptr);
    static QString getConfigXMLFileFullPath();
    ///
    /// \brief The CompleteInfo enum
    ///
    enum CompleteInfo
    {
        ReadComplete ///< 读取完成
        ,WriteComplete ///< 写入完成
        ,WriteError ///< 写入文件错误
        ,ReadError ///< 读取文件错误
        ,FileOpenError ///< 文件打开错误
    };

public slots:
    bool startRead();
    bool startWrite();
signals:
    void readWriteComplete(SAConfigXMLReadWriter::CompleteInfo info);
private:
    void readContent(QXmlStreamReader* xml);
    void readKey(QXmlStreamReader* xml,const QString& contentName);

    void writeContent(QXmlStreamWriter* xml,const QString& content);
    void writeKey(QXmlStreamWriter* xml,const QString& key,const QVariant& var);
private:
    SAGlobalConfig* m_config;
};

#endif // SACONFIGXMLREADWRITER_H
