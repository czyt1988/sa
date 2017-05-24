#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
//==图层的选中状态无法看到颜色，用qss改为外框
//==添加主题管理的类SAThemeManager

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
    MainWindow w;
    w.show();
    
    return a.exec();
}
