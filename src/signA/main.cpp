#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "SAThemeManager.h"
#include <QDebug>
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

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
    QString styleString = SAThemeManager::getDefaultStyleString();
    a.setStyleSheet(styleString);
    //样式设置
    MainWindow w;
    //w.SARibbonMainWindow::setStyleSheet(styleString);
    w.show();
    
    return a.exec();
}
