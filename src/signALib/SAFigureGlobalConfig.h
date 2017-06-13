#ifndef SAFIGUREGLOBALCONFIG_H
#define SAFIGUREGLOBALCONFIG_H

#include "SALibGlobal.h"

class SALIB_EXPORT SAFigureGlobalConfig
{
public:

    SAFigureGlobalConfig();
    //根据点数获取绘图曲线的宽度
    static int getPlotCurWidth(int points);
    //设置大于次点数时，绘图曲线默认宽度为1pix
    static void setPlotCurWidth1(int points);

};

#endif // SAFIGUREGLOBALCONFIG_H
