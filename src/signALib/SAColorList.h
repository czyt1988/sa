#ifndef SACOLORLIST_H
#define SACOLORLIST_H
#include "SALibGlobal.h"
#include <QVector>
#include <QColor>

/**
 * @brief 用于生成简单的线性颜色序列
 */
class SALIB_EXPORT SAColorList : public QVector<QColor>
{
public:
    SAColorList();
    SAColorList(int s);
    ~SAColorList();
    SAColorList(const QColor& startclr, const QColor& endclr, unsigned int step);
    //设置颜色范围
    void setColorRange(const QColor& startclr, const QColor& endclr, unsigned int step);

    //根据比例获取颜色 percent必须在0~1之间
    QColor getColor(double percent) const;


    //通过设定的范围和步长，得到对应百分比的色阶
    static QColor getColor(const QColor& startclr, const QColor& endclr, unsigned int step, double percent);
};

#endif // SALINEGRADIENTCOLORLIST_H
