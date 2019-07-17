#include "SALocalServeSocketClineParse.h"

SALocalServeSocketClineParse::SALocalServeSocketClineParse(QLocalSocket* localSocket,QObject* parent)
    :SALocalServeSocketOpt(localSocket,parent)
{

}

SALocalServeSocketClineParse::SALocalServeSocketClineParse(QObject *parent)
    :SALocalServeSocketOpt(parent)
{

}

void SALocalServeSocketClineParse::setToken(uint token)
{
    SALocalServeSocketOpt::setToken(token);
}
