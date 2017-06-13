#include "SAFigureGlobalConfig.h"
#include "SAGlobalConfig.h"
#include "SAGlobalConfigDefine.h"
SAFigureGlobalConfig::SAFigureGlobalConfig()
{
}

///
/// \brief 根据点数获取线宽
/// \param points 点数
/// \return 线宽
/// \see setPlotCurWidth1
///
int SAFigureGlobalConfig::getPlotCurWidth(int points)
{
    if(points > saConfig->getIntValue(CFG_CONTENT_FIG,CFG_FIG_PlotCurWidthAdded,1000))
        return 1;
    return 2;
}
///
/// \brief 设置大于次点数时，绘图曲线默认宽度为1pix
/// \param points 绘图点数
/// \see getPlotCurWidth
///
void SAFigureGlobalConfig::setPlotCurWidth1(int points)
{
    saConfig->setIntValue(CFG_CONTENT_FIG,CFG_FIG_PlotCurWidthAdded,points);
}
