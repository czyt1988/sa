#ifndef MDIAREA_H
#define MDIAREA_H
#include <QMdiArea>
#include <SAMdiSubWindow.h>
class SAMdiArea : public QMdiArea
{
    Q_OBJECT
public:
    SAMdiArea(QWidget * parent = 0);
    ///
    /// \brief addSubWindow的重写
    /// \param widget
    /// \param windowFlags
    /// \return
    ///
    QMdiSubWindow *	addSubWindow(QWidget * widget, Qt::WindowFlags windowFlags = 0);
signals:
    void subWindowsClosed(SAMdiSubWindow *sub);
    void subWindowsHidden(SAMdiSubWindow *sub);
    void subWindowsShowContextMenu();
private slots:
    void onSubWindowClosed(SAMdiSubWindow *sub);
    void onSubWindowHidden(SAMdiSubWindow *sub);
    void onSubWindowShowContextMenu();
};

#endif // MDIAREA_H
