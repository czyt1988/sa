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
    m_contextCategory = ribbon->addContextCategory(tr("context"),Qt::red,1);
    SARibbonCategory* contextCategoryPage1 = m_contextCategory->addCategoryPage(tr("page 1"));
    SARibbonCategory* contextCategoryPage2 = m_contextCategory->addCategoryPage(tr("page 2"));

}

void MainWindow::onShowContextCategory(bool on)
{
    ribbonBar()->setContextCategoryVisible(m_contextCategory,on);
}

void MainWindow::createCategoryMain(SARibbonCategory *page)
{
    QPushButton* button = new QPushButton(page);
    button->setCheckable(true);
    button->setGeometry(0,0,100,25);
    connect(button,&QPushButton::toggled
            ,this,&MainWindow::onShowContextCategory);
}

void MainWindow::createCategoryOther(SARibbonCategory *page)
{

}

