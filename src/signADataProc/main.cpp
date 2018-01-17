
#include <QApplication>
#include <QTextCodec>
#include <QStringList>
#include <QScopedPointer>
#include "SADataProcServe.h"
#include <QDebug>
#include <QMessageBox>
#include <QSharedMemory>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QDateTime>
#include "SACsvWriter.h"
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  static QFile s_log_file("saDataProcDebug.csv");
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
      fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtInfoMsg:
      fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtWarningMsg:
      fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtCriticalMsg:
      fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtFatalMsg:
      fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      abort();
  }
  if(!s_log_file.isOpen())
  {
      s_log_file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
  }
  if(s_log_file.isOpen())
  {
      QTextStream txt(&s_log_file);
      SACsvWriter csv(&txt);
      csv << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
          << context.function
          << msg
          << context.file
      ;
      csv.endLine(context.line);
  }
}


int main(int argc, char *argv[])
{
    QSharedMemory share("signaDataProc.Main");
    if(share.attach())
    {
        return 0;
    }
    share.create(1);
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    QStringList argsList = a.arguments();
   // QMessageBox::information(nullptr,"pro",argsList.join(','));
    //调用必须后面跟随调用者的pid
    qDebug() << "==============start===================";
    qDebug() << argsList;

    if(argsList.size()<2)
    {
        qDebug() << "arg num invalid ret 1";
        return 1;
    }
    bool isSuccess = false;
    uint pid = argsList[1].toUInt(&isSuccess);
    if(!isSuccess)
    {
        qDebug() << "arg2 invalid ret 2";
        return 2;
    }
    SADataProcServe client;
    client.setPid(pid);
    return a.exec();
}
