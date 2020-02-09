#include "SADataProcServe.h"
#include "SAServerDefine.h"
#include "SAProtocolHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QLocalServer>
#include <QThread>
#include <QApplication>
#include "SAPointSeriesStatisticProcess.h"
#include "SAXMLTagDefined.h"
#include <memory>

#define ARG_DES_KEY_ID "key"
#define ARG_DES_TOKEN_ID "token"
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#endif
SADataProcServe::SADataProcServe(QObject *parent):SATCPAssignServe(parent)
  ,m_pid(0)
  ,m_willBeQuit(false)
  ,m_checkLiveTime(10000)//存活20s
{
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


