
#include <QApplication>
#include <QTextCodec>
#include <QStringList>
#include <QScopedPointer>
#include "SADataProcServe.h"
#include <QDebug>
#include <QMessageBox>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QDateTime>
#include <QHostInfo>
#include "SAServeShareMemory.h"
#include "SACsvStream.h"
#include "SAServeHandleFun.h"
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  static QFile s_log_file("saDataProcDebug.csv");
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
      fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.function, context.line, context.file);
      break;
  case QtInfoMsg:
      fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.function, context.line, context.file);
      break;
  case QtWarningMsg:
      fprintf(stdout, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.function, context.line, context.file);
      break;
  case QtCriticalMsg:
      fprintf(stdout, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.function, context.line, context.file);
      break;
  case QtFatalMsg:
      fprintf(stdout, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.function, context.line, context.file);
      abort();
  }
  if(!s_log_file.isOpen())
  {
      s_log_file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
  }
  if(s_log_file.isOpen())
  {
      static SACsvStream s_csv(&s_log_file);
      s_csv << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            << msg
            << context.function
            << context.line
            << context.file
            << endl;
      ;
      s_csv.flush();
#ifdef QT_NO_DEBUG_OUTPUT
#endif
  }
  fflush(stdout);
#ifdef QT_NO_DEBUG_OUTPUT
#endif
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    SAServeShareMemory sharemem;
    if(!sharemem.isAttach())
    {
        qDebug() << QStringLiteral("共享内存初始化失败，已经有服务进程在运行，本进程退出");
        return 1;
    }
    //初始化服务
    qDebug() << QStringLiteral("开始初始化服务句柄");
    SA::init_serve_funciotn_handle();

    //
    QApplication a(argc, argv);
    QStringList argsList = a.arguments();
   // QMessageBox::information(nullptr,"pro",argsList.join(','));
    //调用必须后面跟随调用者的pid
    qDebug() << "==============start===================";
    qDebug() << argsList;

    if(argsList.size()<2)
    {
        qDebug() << "arg num invalid ret 1";
        return 2;
    }
    bool isSuccess = false;
    uint pid = argsList[1].toUInt(&isSuccess);
    if(!isSuccess)
    {
        qDebug() << "arg2 invalid ret 2";
        return 3;
    }
    SADataProcServe serve;
    serve.setPid(pid);
    int islisten = 0;
    int port = 10098;
    do
    {
        ++port;
        if(serve.listen(port))
        {
            qDebug() << "listen success,port is:" << port;
            islisten = 1;
            break;
        }
    }while(1 != islisten && port < 65536);
    //监听成功后写入端口
    qDebug() << QStringLiteral("服务器建立完成，服务器状态写入共享内存");
    sharemem.setServeState(SAServeShareMemory::ServeIsReady);
    sharemem.setListenState(1 == islisten);
    sharemem.setPort(port);
    qDebug() << QStringLiteral("服务器正常运行，时间：") << QDateTime::currentDateTime();
    return a.exec();
}


