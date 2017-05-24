#include "SARandColorMaker.h"
#include <QTime>

QList<QColor> SARandColorMaker::s_rand_curve_color_list
    = SARandColorMaker::__init_color_list();

QList<QColor> SARandColorMaker::s_default_curve_color_list
    = SARandColorMaker::__init_color_list();

SARandColorMaker::SARandColorMaker()
{

}
///
/// \brief 生成一个start到end范围内的随机数
/// \param start 随机数的开始位置
/// \param end 随机数的结束位置
/// \return
///
int SARandColorMaker::rand(int start, int end)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int res = qrand();
    return (int) ((end - start) * ((float)res / (RAND_MAX - 0)));
}
///
/// \brief 获取曲线颜色，每次调用会获取不同的颜色
/// 颜色根据曲线列表的颜色进行获取,当列表的所有颜色获取结束后，将重新开始获取
/// 曲线列表的颜色可以使用refreshCurveColorRandList进行刷新
/// \return 颜色
/// \see refreshCurveColorRandList getDefaultCurveColorList
///
QColor SARandColorMaker::getCurveColor()
{
    static int s_curve_color_index = 0;
    int count = s_rand_curve_color_list.size();
    if(count <= 0)
    {
        refreshCurveColorRandList();
        count = s_rand_curve_color_list.size();
    }
    ++s_curve_color_index;
    if(s_curve_color_index >= count)
    {
        s_curve_color_index = 0;
    }
    return s_rand_curve_color_list[s_curve_color_index];
}
///
/// \brief 随机颜色列表
/// \return
///
QList<QColor> SARandColorMaker::getRandCurveColorList()
{
    return s_rand_curve_color_list;
}
///
/// \brief 默认的颜色列表
/// 此颜色列表是默认定制，不是随机，但随机颜色列表是从这个默认颜色列表中获取的
/// \return
///
QList<QColor> SARandColorMaker::getDefaultCurveColorList()
{
    return s_default_curve_color_list;
}
///
/// \brief 自动生成一个随机的颜色列表
/// 此颜色列表每次生成都不一样，生成之后可以通过getCurveColor获取
/// 调用getCurveColor前可以通过此函数刷新颜色列表，getCurve每次调用都会读取此列表的下一个，到尾部后会从头开始继续读取
///
void SARandColorMaker::refreshCurveColorRandList()
{
    s_rand_curve_color_list.clear();
    s_rand_curve_color_list.reserve(s_default_curve_color_list.size());
    QList<QColor> clrs = s_default_curve_color_list;
    while(clrs.size() > 0)
    {
        int index = rand(0,clrs.size()-1);
        s_rand_curve_color_list.append(clrs.takeAt(index));
    }
}

QList<QColor> SARandColorMaker::__init_color_list()
{
    QList<QColor> tmp;
    tmp << 	QColor(240,52,121)//粉红
            << QColor(219,5,178)//紫色
            << QColor(65,152,175)//蓝色
            << QColor(169,155,189)//紫色
            << QColor(245,62,62)//橙色
            << QColor(54,80,234)//蓝色
            << QColor(36,36,36)//黑色
            << QColor(79,129,189)//蓝色
            << QColor(192,80,77)//红色
            << QColor(145,195,213)//蓝色
            << QColor(185,205,150)//绿色
            << QColor(36,183,103)//绿色
            << QColor(249,181,144)//橙色
            << QColor(137,165,78);//绿色
    return tmp;
}
