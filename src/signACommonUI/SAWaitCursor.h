#ifndef SAWAITCURSOR_H
#define SAWAITCURSOR_H
#include <QApplication>
#include "SACommonUIGlobal.h"
class SA_COMMON_UI_EXPORT SAWaitCursor
{
public:
    SAWaitCursor()
    {
         QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    }
    ~SAWaitCursor()
    {
        QApplication::restoreOverrideCursor();
    }
    void setWaitCursor()
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    }
    void setCursor(QCursor cur = QCursor(Qt::WaitCursor))
    {
        QApplication::setOverrideCursor(cur);
    }
    void release()
    {
        QApplication::restoreOverrideCursor();
    }
};
#ifndef SA_SET_AUTO_WAIT_CURSOR
#define SA_SET_AUTO_WAIT_CURSOR() \
    SAWaitCursor __sa__wait__cursor;\
    Q_UNUSED(__sa__wait__cursor);
#endif
#endif // SAWAITCURSOR_H
