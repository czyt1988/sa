#ifndef SALINEGRADIENTCOLORLIST_H
#define SALINEGRADIENTCOLORLIST_H
#include "SALibGlobal.h"
#include <QVector>
#include <QColor>

/**
 * @brief 用于生成简单的线性颜色序列
 */
class SALIB_EXPORT SALineGradientColorList
{
public:
    SALineGradientColorList();
    SALineGradientColorList(const QColor& start, const QColor& end, unsigned int step);
    //设置颜色范围
    void setColorRange(const QColor& start, const QColor& end, unsigned int step);

    //根据比例获取颜色 percent必须在0~1之间
    QColor getColor(double percent);

    //通过设定的范围和步长，得到对应百分比的色阶
    static QColor getColor(const QColor& start, const QColor& end, unsigned int step, double percent);

    //获取颜色列表
    QVector<QColor>& colorlist() { return (m_colorlist); }
    //获取颜色列表
    const QVector<QColor>& colorlist() const { return (m_colorlist); }

private:
    QVector<QColor> m_colorlist;///< 存放ColorList
};

#endif // SALINEGRADIENTCOLORLIST_H
