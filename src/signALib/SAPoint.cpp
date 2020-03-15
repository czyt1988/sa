#include "SAPoint.h"

SAPoint::SAPoint():QPoint()
{

}

SAPoint::SAPoint(int xpos, int ypos):QPoint(xpos,ypos)
{

}

bool operator<(const SAPoint &a, const SAPoint &b)
{
    if(a.x() < b.x() && a.y() <= b.y())
        return true;
    if(a.x() == b.x() && a.y() < b.y()  )
        return true;
    if(a.x() > b.x() && a.y() < b.y()  )
        return true;
    return false;
}
