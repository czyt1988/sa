#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include <QLocale>
#include <QTranslator>
#include "SAThemeManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

//按照QLocal加载语言
void load_local_language();

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
    //
    qDebug() << "libs path:" << QCoreApplication::libraryPaths();

    //加载本地语言
    load_local_language();

    //样式设置
    MainWindow w;
    w.show();

    return a.exec();
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
