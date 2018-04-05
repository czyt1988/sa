#include "SAWaitCursor.h"
#include <QApplication>
SAWaitCursor::SAWaitCursor()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

SAWaitCursor::~SAWaitCursor()
{
    QApplication::restoreOverrideCursor();
}

void SAWaitCursor::setWaitCursor()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void SAWaitCursor::setCursor(const QCursor& cur)
{
    QApplication::setOverrideCursor(cur);
}

void SAWaitCursor::release()
{
    QApplication::restoreOverrideCursor();
}
