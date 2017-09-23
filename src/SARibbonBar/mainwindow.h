#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "SARibbonMainWindow.h"
class SARibbonCategory;
class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* par=nullptr);
private:
    void createCategoryMain(SARibbonCategory* page);
    void createCategoryOther(SARibbonCategory* page);
private:

};

#endif // MAINWINDOW_H
