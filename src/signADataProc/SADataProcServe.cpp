#include "SADataProcServe.h"
#include "SAServerDefine.h"
#include "SAProtocolHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QLocalServer>
#include <QThread>
#include <QApplication>
#include "SAXMLTagDefined.h"
#include <memory>

#define ARG_DES_KEY_ID "key"
#define ARG_DES_TOKEN_ID "token"
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#endif
SADataProcServe::SADataProcServe(QObject *parent, int idealTimeSecond):SATcpServe(parent)
  ,m_pid(0)
  ,m_willBeQuit(false)
  ,m_checkLiveTime(idealTimeSecond)//存活20s
{
    connect(&m_liveChecker,&QTimer::timeout,this,&SADataProcServe::onCkeckLiveTimeout);
}

SADataProcServe::~SADataProcServe()
{

}



uint SADataProcServe::getPid() const
{
    return m_pid;
}

void SADataProcServe::setPid(const uint &pid)
{
    m_pid = pid;
}


void SADataProcServe::onCkeckLiveTimeout()
{

}


