#ifndef SACOLORALGORITHM_H
#define SACOLORALGORITHM_H
#include <QVector>
#include <QColor>
#include <QRgb>

/**
 * @file 用于处理颜色相关的算法
 */

namespace SA {
/**
 * @brief 通过线性梯度计算对应的值
 * @param start 待计算的开始值
 * @param end 待计算的结束值
 * @param percent 取值百分比，percent必须为0~1之间的数值
 * @return 根据百分比返回对应的值
 */
template<typename VALUE>
VALUE line_gradient_value(VALUE start, VALUE end, float percent)
{
    return (start + ((end - start) * percent));
}


/**
 * @brief 根据步数生成线性梯度的序列
 * @param start 开始数据
 * @param end 结束数据
 * @param step 步长
 * @param itebegin 开始迭代器
 * @param iteend 结束迭代器
 * @return
 */
template<typename VALUE, typename ITE_START, typename ITE_END>
VALUE generate_line_gradient_value(VALUE start, VALUE end, unsigned int step, ITE_START itebegin, ITE_END iteend)
{
    double dt = (end - start) / (double)(step);

    if (itebegin < iteend) {
        //第一个是start
        (*itebegin) = start;
        ++itebegin;
    }
    while (itebegin < iteend)
    {
        (*itebegin) = start + dt;
        ++itebegin;
    }
}

/**
 * @brief 根据颜色范围生成颜色序列
 * @param start 开始颜色
 * @param end 结束颜色
 * @param begin 存放颜色的迭代器
 * @param end 存放颜色的结束迭代器
 */
template <typename Ite>
void q_generate_line_gradientcolor(const QColor& startclr, const QColor& endclr,Ite begin,Ite end)
{
    size_t step = std::distance(begin,end);
    qreal dr = (endclr.red()-startclr.red())/(qreal)step;
    qreal dg = (endclr.green()-startclr.green())/(qreal)step;
    qreal db = (endclr.blue()-startclr.blue())/(qreal)step;
    int i = 0;
    if (startclr.alpha() != endclr.alpha()) {
        qreal da = (endclr.alpha()-startclr.alpha())/(qreal)step;
        while(begin < end){
            *begin = QColor(startclr.red()+dr*i, startclr.green()+dg*i, startclr.blue()+db*i, startclr.alpha()+da*i);
            ++begin;
            ++i;
        }
    }else{
        while(begin < end){
            *begin = QColor(startclr.red()+dr*i, startclr.green()+dg*i, startclr.blue()+db*i);
            ++begin;
            ++i;
        }
    }

}

QVector<QColor> q_generate_line_gradientcolor(const QColor& start, const QColor& end, unsigned int step)
{
    QVector<QColor> res;
    res.resize(step);
    q_generate_line_gradientcolor(start,end,res.begin(),res.end());
    return (res);
}


QColor q_generate_line_gradientcolor(const QColor& start, const QColor& end, unsigned int step, double percent)
{
    qreal dr = (end.red()-start.red())/(qreal)step;
    qreal dg = (end.green()-start.green())/(qreal)step;
    qreal db = (end.blue()-start.blue())/(qreal)step;
    qreal da = (end.alpha()-start.alpha())/(qreal)step;

    return (QColor(start.red()+(dr*step*percent)
           , start.green()+dg*step*percent
           , start.blue()+db*step*percent
           , start.alpha()+da*step*percent));
}



} // namespace SA

#endif // SACOLORALGORITHM_H
