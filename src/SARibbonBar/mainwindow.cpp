#include "mainwindow.h"
#include <QTextEdit>
#include <QAbstractButton>
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include <QPushButton>
MainWindow::MainWindow(QWidget *par):SARibbonMainWindow(par)
{
    setWindowTitle(tr("ribbon test"));
    QTextEdit* edit = new QTextEdit(this);
    setCentralWidget(edit);
    SARibbonBar* ribbon = ribbonBar();
    ribbon->applitionButton()->setText(tr("File"));
    SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("main page"));
    createCategoryMain(categoryMain);
    SARibbonCategory* categoryOther = ribbon->addCategoryPage(tr("other"));
    createCategoryOther(categoryOther);
}

void MainWindow::createCategoryMain(SARibbonCategory *page)
{
    QPushButton* button = new QPushButton(page);
    button->setGeometry(0,0,100,25);
}

void MainWindow::createCategoryOther(SARibbonCategory *page)
{

}

