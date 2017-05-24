#include "SAMainWindowDelegate.h"

SAMainWindowDelegate::SAMainWindowDelegate(MainWindow* wnd):m_wnd(wnd)
{

}

SAMainWindowDelegate::~SAMainWindowDelegate()
{

}

MainWindow*SAMainWindowDelegate::getMainWindow() const
{
    return m_wnd;
}
