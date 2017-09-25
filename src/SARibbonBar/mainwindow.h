#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "SARibbonMainWindow.h"
class SARibbonCategory;
class SARibbonContextCategory;
class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* par=nullptr);
private slots:
    void onShowContextCategory(bool on);
private:
    void createCategoryMain(SARibbonCategory* page);
    void createCategoryOther(SARibbonCategory* page);
private:
    SARibbonContextCategory* m_contextCategory;
};

#endif // MAINWINDOW_H
