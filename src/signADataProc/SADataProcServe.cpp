#include "SADataProcServe.h"
#include "SAServerDefine.h"
#include "SAProtocolHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QLocalServer>
#include <QDebug>
#include <QApplication>
#include "SAXMLTagDefined.h"
#include <memory>
#include "SADataProcSocket.h"


#define ARG_DES_KEY_ID		"key"
#define ARG_DES_TOKEN_ID	"token"
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#endif


SATcpSocket *create_socket();

SATcpSocket *create_socket()
{
    qDebug() << "create SADataProcSocket";
    return (new SADataProcSocket());
}


SADataProcServe::SADataProcServe(QObject *parent, int idealTimeSecond) : SATcpServe(parent)
    , m_pid(0)
    , m_willBeQuit(false)
    , m_checkLiveTime(idealTimeSecond*1000)//存活20s
{
    connect(&m_liveChecker, &QTimer::timeout, this, &SADataProcServe::onCkeckLiveTimeout);
    connect(this, &SATcpServe::newConnection, this, &SADataProcServe::onNewConnection);
    registSocketFactory(create_socket);
}


SADataProcServe::~SADataProcServe()
{
}


uint SADataProcServe::getPid() const
{
    return (m_pid);
}


void SADataProcServe::setPid(const uint& pid)
{
    m_pid = pid;
}


void SADataProcServe::onCkeckLiveTimeout()
{
    if (getSockets().size() <= 0) {
        //说明没有连接
        //退出程序
        QTimer::singleShot(0, qApp, &QApplication::quit);
    }
}


void SADataProcServe::onNewConnection()
{
    m_liveChecker.start(m_checkLiveTime);
}
