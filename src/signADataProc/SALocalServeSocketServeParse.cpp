#include "SALocalServeSocketServeParse.h"

SALocalServeSocketServeParse::SALocalServeSocketServeParse(QLocalSocket *localSocket, QObject *parent)
    :SALocalServeSocketOpt(localSocket,parent)
{

}

SALocalServeSocketServeParse::SALocalServeSocketServeParse(QObject *parent)
    :SALocalServeSocketOpt(parent)
{

}

void SALocalServeSocketServeParse::setToken(uint token)
{
    SALocalServeSocketOpt::setToken(token);
}
