#ifndef SACONFIGXMLREADWRITER_H
#define SACONFIGXMLREADWRITER_H
#include <QObject>
#include <SAGlobals.h>
#include <QHash>
#include <functional>
class SAConfig;
class QXmlStreamReader;
///
/// \brief 用于读写SAConfig的xml类，可用于多线程
///
class SAConfigXMLReadWriter : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(SAConfig*,const QString&)> FUN_PTR;
    SAConfigXMLReadWriter(SAConfig* config,QObject* par = nullptr);
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
private:
    SAConfig* m_config;
    static QHash<QString,FUN_PTR> s_element2funptr;///< 记录每个节点关键字对应的处理方式
};

#endif // SACONFIGXMLREADWRITER_H
