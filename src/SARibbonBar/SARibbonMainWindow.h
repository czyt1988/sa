#ifndef SARIBBONMAINWINDOW_H
#define SARIBBONMAINWINDOW_H
#include <QMainWindow>
class SARibbonMainWindowPrivate;
class SARibbonBar;
class SARibbonMainWindow : public QMainWindow
{
public:
    SARibbonMainWindow(QWidget* parent = nullptr);
    //
    const SARibbonBar* ribbonBar() const;
    SARibbonBar* ribbonBar();
private:
    SARibbonMainWindowPrivate* m_d;
};

#endif // SARIBBONMAINWINDOW_H
