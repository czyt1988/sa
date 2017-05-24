#ifndef SARANDCOLORMAKER_H
#define SARANDCOLORMAKER_H
#include "SALibGlobal.h"
#include <QColor>
#include <QList>
class SALIB_EXPORT SARandColorMaker
{
public:
    SARandColorMaker();
    //生成一个start到end范围内的随机数
    static int rand(int start,int end);
    //获取曲线颜色，每次调用会获取不同的颜色
    static QColor getCurveColor();
    //获取默认曲线颜色列表
    static QList<QColor> getRandCurveColorList();
    //默认的颜色列表，此颜色列表是默认定制，不是随机，但随机颜色列表是从这个默认颜色列表中获取的
    static QList<QColor> getDefaultCurveColorList();
    //自动生成一个随机的颜色列表，此颜色列表每次生成都不一样，生成之后可以通过getCurveColor获取
    static void refreshCurveColorRandList();
private:
    static QList<QColor> __init_color_list();
private:
    static QList<QColor> s_rand_curve_color_list;///< 随机的曲线颜色
    static QList<QColor> s_default_curve_color_list;///< 默认的曲线颜色
};

#endif // SARANDCOLORMAKER_H
