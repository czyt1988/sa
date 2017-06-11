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
public:
    SAConfigXMLReadWriter(SAGlobalConfig* config,QObject* par = nullptr);
    static QString getConfigXMLFileFullPath();
public slots:
    void startRead();
    void startWrite();
signals:
    void readComplete(bool isSuccess);
    void writeComplete(bool isSuccess);
    void message(const QString& info,SA::MeaasgeType type);
private:
    void readConfigSection(QXmlStreamReader* xml);
    void writeContent(QXmlStreamWriter* xml,const QString& content);
    void writeKey(QXmlStreamWriter* xml,const QString& key,const QVariant& var);
private:
    SAGlobalConfig* m_config;
};

#endif // SACONFIGXMLREADWRITER_H
