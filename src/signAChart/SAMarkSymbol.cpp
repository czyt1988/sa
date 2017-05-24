#include "SAMarkSymbol.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>

SAAbstractMarkSymbol::SAAbstractMarkSymbol()
{

}

SAAbstractMarkSymbol::SAAbstractMarkSymbol(const QPainterPath &path, const QBrush &brush, const QPen &pen)
    :QwtSymbol(path,brush,pen)
{

}

SAAbstractMarkSymbol::~SAAbstractMarkSymbol()
{

}

SATriangleMarkSymbol::SATriangleMarkSymbol(QColor clr, int H, int W):SAAbstractMarkSymbol()
{
    QPen pen(clr,1);
    pen.setJoinStyle( Qt::MiterJoin);
    setPen(pen);
    setBrush(QColor(clr.red(),clr.green(),clr.blue(),30));
    QPainterPath path;

    path.moveTo(0,-H);
    path.lineTo(-W/2,-H);
    path.lineTo(0,0);
    path.lineTo(W/2,-H);
    path.lineTo(0,-H);

    setPath(path);
    setPinPointEnabled(true);
    setSize(W,H);
    setPinPoint(QPointF(0,0));//设置文字的坐标点，-6是偏移
}

SATriangleMarkSymbol::~SATriangleMarkSymbol()
{

}

