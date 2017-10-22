#ifndef SARIBBONDRAWHELPER_H
#define SARIBBONDRAWHELPER_H
#include <QIcon>
#include <QPainter>
#include <QStyleOption>
///
/// \brief 绘图辅助
///
class SARibbonDrawHelper
{
public:
    SARibbonDrawHelper();
    static void drawIcon(const QIcon &icon, QPainter *painter, const QStyleOption &opt
                         ,int x,int y,int width,int height);
};

#endif // SARIBBONDRAWHELPER_H
