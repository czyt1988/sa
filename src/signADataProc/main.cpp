
#include <QApplication>
#include <QTextCodec>
#include <QStringList>
#include <QScopedPointer>
#include "SADataProcClient.h"
#include <QDebug>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <QFile>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  static QFile s_file("saDataProcDebug.txt");
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
  if(!s_file.isOpen())
  {
      s_file.open(QIODevice::WriteOnly);
  }
  if(s_file.isOpen())
  {
      QTextStream txt(&s_file);
      txt << QString("%1 (%2:%3,%4)").arg(msg).arg(context.file).arg(context.line).arg(context.function)<<endl;

  }
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    QStringList argsList = a.arguments();
   // QMessageBox::information(nullptr,"pro",argsList.join(','));
    //调用必须后面跟随调用者的pid
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
    SADataProcClient client;
    client.setPid(pid);
    client.shakeHand();
    return a.exec();
}
