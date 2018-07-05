#ifndef FUN_DSP_H
#define FUN_DSP_H
#include <memory>
#include <QString>
#include "czyMath_DSP.h"
class SAUIInterface;
class SATimeFrequencyAnalysis;
///
/// \brief 去趋势
/// \param ui ui接口
///
void detrendDirect(SAUIInterface* ui);

///
/// \brief 信号设置窗
/// \param ui ui接口
///
void setWindow(SAUIInterface* ui);


///
/// \brief 频谱分析
/// \param ui ui接口
///
void spectrum(SAUIInterface* ui);

///
/// \brief 功率谱分析
/// \param ui ui接口
///
void powerSpectrum(SAUIInterface *ui);

///
/// \brief 时频分析工具箱
/// \param ui ui接口
///
void tmeFrequency(SAUIInterface *ui);
#endif // FUN_DSP_H
