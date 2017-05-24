#include "SAUIInterface.h"
#include <QMdiSubWindow>
#include <SAFigureWindow.h>
SAUIInterface::SAUIInterface()
{

}

SAUIInterface::~SAUIInterface()
{

}
///
/// \brief 从subwindow指针中查找是否含有SAFigureWindow
/// \param sub subwindow指针
/// \return 存在返回指针，否则返回nullptr
///
SAFigureWindow *SAUIInterface::getFigureWidgetFromMdiSubWindow(QMdiSubWindow *sub)
{
    return qobject_cast<SAFigureWindow*>(sub->widget());
}




