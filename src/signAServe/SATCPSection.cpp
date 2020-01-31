#include "SATCPSection_private.h"
#include <QMutexLocker>

SATCPSection::SATCPSection(QObject *par)
{

}

void SATCPSection::appendSocket(QTcpSocket *socket)
{
    QMutexLocker locker(&m_lockSocketSets);
    m_socketSets.insert(socket);
}
