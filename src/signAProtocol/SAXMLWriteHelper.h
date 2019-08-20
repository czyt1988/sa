#ifndef SAXMLWRITEHELPER_H
#define SAXMLWRITEHELPER_H
#include "SALibGlobal.h"
#include <QDomDocument>
#include <QVariant>

class SALIB_EXPORT SAXMLWriteHelper
{
public:
    SAXMLWriteHelper(const QString& type);
    ~SAXMLWriteHelper();


    //
    void startHeadWriteGroup(const QString& name);
    void endHeadWriteGroup();
    void writeHeadValue(const QVariant &d, const QString& name);

    //
    void startContentWriteGroup(const QString& name);
    void endContentWriteGroup();
    void writeContentValue(const QVariant &d, const QString& name);
    QString toString(int indent = 1) const;
private:
    QDomDocument m_doc;
    QDomElement m_rootEle;
    QDomElement m_headerEle;
    QDomElement m_contentEle;
    QDomElement m_tmpHeadEle;
    QDomElement m_tmpContentEle;
};

#endif // SAXMLWRITEHELPER_H
