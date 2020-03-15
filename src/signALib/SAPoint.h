#ifndef SAPOINT_H
#define SAPOINT_H
#include "SALibGlobal.h"
#include <QPoint>

class SALIB_EXPORT SAPoint : public QPoint
{
public:
    SAPoint();
    SAPoint(int xpos, int ypos);
    ///
    /// \brief 重载 < 使得可用于Map
    /// \param a
    /// \param b
    /// \return
    ///
    SALIB_EXPORT friend bool operator<(const SAPoint &a,const SAPoint &b);
};

#endif // SAPOINT_H
