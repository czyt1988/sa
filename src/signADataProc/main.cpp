
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

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  static QFile s_log_file("saDataProcDebug.txt");
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
      s_log_file.open(QIODevice::WriteOnly);
  }
  if(s_log_file.isOpen())
  {
      QTextStream txt(&s_log_file);
      txt << QString("[%1:%2,%3]:").arg(context.file).arg(context.line).arg(context.function)<<endl;
      txt << msg << endl;
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
    qDebug() << argsList;

    if(argsList.size()<2)
    {
        return 1;
    }
    bool isSuccess = false;
    uint pid = argsList[1].toUInt(&isSuccess);
    if(!isSuccess)
    {
        return 2;
    }
    SADataProcServe client;
    client.setPid(pid);
    return a.exec();
}
