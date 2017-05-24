#include "SAMdiArea.h"

SAMdiArea::SAMdiArea(QWidget * parent):QMdiArea(parent)
{

}

QMdiSubWindow *	SAMdiArea::addSubWindow(QWidget * widget, Qt::WindowFlags windowFlags)
{
    SAMdiSubWindow* msw = qobject_cast<SAMdiSubWindow*>(widget);
    if(msw)
    {
        connect(msw,&SAMdiSubWindow::closedWindow,this,&SAMdiArea::onSubWindowClosed);
        connect(msw,&SAMdiSubWindow::hiddenWindow,this,&SAMdiArea::onSubWindowHidden);
        connect(msw,&SAMdiSubWindow::showContextMenu,this,&SAMdiArea::onSubWindowShowContextMenu);
        return QMdiArea::addSubWindow(widget,windowFlags);
    }
    else
    {
        return QMdiArea::addSubWindow(widget,windowFlags);
    }
    return nullptr;
}
///
/// \brief 子窗口关闭时触发的槽
/// \param sub
///
void SAMdiArea::onSubWindowClosed(SAMdiSubWindow *sub)
{
    emit subWindowsClosed(sub);
}
void SAMdiArea::onSubWindowHidden(SAMdiSubWindow *sub)
{
    emit subWindowsHidden(sub);
}

void SAMdiArea::onSubWindowShowContextMenu()
{
    emit subWindowsShowContextMenu();
}
