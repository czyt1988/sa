#ifndef SAMAINWINDOWDELEGATE_H
#define SAMAINWINDOWDELEGATE_H
#include <QObject>
class MainWindow;
class SAMainWindowDelegate : public QObject
{
    Q_OBJECT
public:
    SAMainWindowDelegate(MainWindow* wnd);
    virtual ~SAMainWindowDelegate();
    MainWindow* getMainWindow() const;
private:
    MainWindow* m_wnd;
};

#endif // SAMAINWINDOWDELEGATE_H
