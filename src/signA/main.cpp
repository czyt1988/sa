#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include <QDateTime>
#include <QLocale>
#include <QTranslator>
#include <QScopedPointer>
#include "SAThemeManager.h"
#include "SACsvStream.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

//日志文件指针
static QFile* g_log_file = nullptr;
//按照QLocal加载语言
void load_local_language();
//获取日志文件夹路径
QString get_log_file_path();
//生成log文件的名字
QString make_log_file_name();
//重定向qdebug的打印
void sa_log_out_put(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#if defined(_MSC_VER) && (_MSC_VER < 1600)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB18030"));
#else
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
#endif
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + QDir::separator() + "libs");
    g_log_file = new QFile(get_log_file_path() + QDir::separator() + make_log_file_name());
    if(!g_log_file->open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        qDebug() << "can not open log file";
    }
    qInstallMessageHandler(sa_log_out_put);
    //
    qDebug() << "libs path:" << QCoreApplication::libraryPaths();

    //加载本地语言
    load_local_language();

    //样式设置
    MainWindow w;
    w.show();

    int r = a.exec();
    return r;
}


///
/// \brief 加载语言
///
void load_local_language()
{
    QScopedPointer<QTranslator> translator(new QTranslator);
    QLocale loc;
    qDebug() << loc.uiLanguages();
    if("zh" == loc.bcp47Name() )
    {
        QFont f = qApp->font();
        f.setFamily(QStringLiteral("微软雅黑"));
        qApp->setFont(f);
    }

    QString lngPath = qApp->applicationDirPath();
    lngPath = lngPath + QDir::separator() + "language";
    if(translator->load(loc,QString(),QString(),lngPath))
    {
        qApp->installTranslator(translator.take());
    }
}


/**
 * @brief 获取日志文件夹路径
 */
QString get_log_file_path()
{
    QString path = QDir::currentPath();
    path += "/log";
    if(!QFile::exists(path))
    {
        //路径不存在先创建
        QDir dir(path);
        if(!dir.mkpath(path))
        {
            qDebug() << "can not make log path:" << path;
        }
    }
    return path;
}

/**
 * @brief 生成log文件的名字
 * @return log文件名字，前面不带/
 */
QString make_log_file_name()
{
    QDateTime datetime = QDateTime::currentDateTime();
    return ("LOG_" + datetime.toString("yyyyMMdd-2") + ".log");
}

/**
 * @brief 重定向qdebug的打印
 * @param type
 * @param context
 * @param msg
 */
void sa_log_out_put(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
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
    if(g_log_file)
    {
        if(g_log_file->isOpen())
        {
          SACsvStream csv(g_log_file);
          csv << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
              << context.function
              << msg
              << context.file
              << endl;
          ;
        }
    }
}
