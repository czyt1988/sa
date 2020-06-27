#ifndef SAWAITCURSOR_H
#define SAWAITCURSOR_H
#include <QCursor>
#include "SACommonUIGlobal.h"

class SA_COMMON_UI_EXPORT SAWaitCursor
{
public:
    SAWaitCursor();
    ~SAWaitCursor();
    void setWaitCursor();
    void setCursor(const QCursor &cur = QCursor(Qt::WaitCursor));
    void release();
};


#ifndef SA_SET_AUTO_WAIT_CURSOR
#define SA_SET_AUTO_WAIT_CURSOR \
    SAWaitCursor __sa__wait__cursor;\
    Q_UNUSED(__sa__wait__cursor);
#endif
#endif // SAWAITCURSOR_H
