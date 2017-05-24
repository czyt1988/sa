#ifndef SAMARKSYMBOL_H
#define SAMARKSYMBOL_H
#include "SAChartGlobals.h"

#include <qwt_symbol.h>
#include <QColor>
///
/// \brief 标记接口
///
class SA_CHART_EXPORT SAAbstractMarkSymbol : public QwtSymbol
{
public:
    SAAbstractMarkSymbol();
    SAAbstractMarkSymbol(const QPainterPath & path, const QBrush &brush, const QPen &pen);
    virtual ~SAAbstractMarkSymbol();
    enum{
        SymbolType_None = 0///< 无标记
        ,SymbolType_TriangleDataMarker///< 三角数据标记
    };
    virtual int markType() const = 0;
};
///
/// \brief 三角点数据标记
///
class SA_CHART_EXPORT SATriangleMarkSymbol : public SAAbstractMarkSymbol
{
public:
    SATriangleMarkSymbol(QColor clr = Qt::blue
            ,int H = 8,int W = 8);
    virtual ~SATriangleMarkSymbol();
    virtual int markType() const
    {
        return SAAbstractMarkSymbol::SymbolType_TriangleDataMarker;
    }
};
#endif // SAMARKSYMBOL_H
