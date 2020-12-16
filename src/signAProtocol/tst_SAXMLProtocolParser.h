#ifndef TST_SAXMLPROTOCOLPARSER_H
#define TST_SAXMLPROTOCOLPARSER_H
#include <QObject>
#include "SAXMLProtocolParser.h"

class tst_SAXMLProtocolParser : public QObject
{
    Q_OBJECT
public:
    tst_SAXMLProtocolParser();
private Q_SLOTS:
    void testXMLProtocol();
};

#endif // TST_SAXMLPROTOCOLPARSER_H
