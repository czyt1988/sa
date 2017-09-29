#include "mainwindow.h"
#include <QTextEdit>
#include <QAbstractButton>
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include <QPushButton>
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QAction>
#include <QMenu>
#include <QDebug>
MainWindow::MainWindow(QWidget *par):SARibbonMainWindow(par)
{
    setWindowTitle(tr("ribbon test"));
    m_edit = new QTextEdit(this);
    setCentralWidget(m_edit);
    SARibbonBar* ribbon = ribbonBar();
    ribbon->applitionButton()->setText(tr("File"));
    SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("main page"));
    createCategoryMain(categoryMain);
    SARibbonCategory* categoryOther = ribbon->addCategoryPage(tr("other"));
    createCategoryOther(categoryOther);
    m_contextCategory = ribbon->addContextCategory(tr("context"),Qt::red,1);
    SARibbonCategory* contextCategoryPage1 = m_contextCategory->addCategoryPage(tr("page 1"));
    SARibbonCategory* contextCategoryPage2 = m_contextCategory->addCategoryPage(tr("page 2"));
    showMaximized();

}

void MainWindow::onShowContextCategory(bool on)
{
    ribbonBar()->setContextCategoryVisible(m_contextCategory,on);
}

void MainWindow::createCategoryMain(SARibbonCategory *page)
{
    SARibbonPannel* pannel = page->addPannel(tr("pannel 1"));
    QAction* act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/save.png"));
    act->setText(tr("text action"));
    pannel->addLargeAction(act);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/save.png"));
    act->setText(tr("text action"));
    pannel->addSmallAction(act);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/filter.png"));
    act->setText(tr("text action"));
    pannel->addSmallAction(act);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(tr("text action"));
    pannel->addSmallAction(act);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(tr("text action"));
    pannel->addSmallAction(act);

    SARibbonToolButton * btn;
    QMenu* menu = new QMenu(this);
    QAction * item = menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(tr("DelayedPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act,&QAction::triggered,this,&MainWindow::onDelayedPopupCheckabletriggered);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(tr("MenuButtonPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act,&QAction::triggered,this,&MainWindow::onMenuButtonPopupCheckabletriggered);

    act = new QAction(this);
    act->setIcon(QIcon(":/icon/icon/Graph-add.png"));
    act->setText(tr("InstantPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act,&QAction::triggered,this,&MainWindow::onInstantPopupCheckabletriggered);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(tr("DelayedPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setCheckable(true);
    connect(act,&QAction::triggered,this,&MainWindow::onDelayedPopupCheckableTest);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/folder.png"));
    act->setText(tr("MenuButtonPopup\n checkable"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setCheckable(true);
    connect(act,&QAction::triggered,this,&MainWindow::onMenuButtonPopupCheckableTest);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon(":/icon/icon/Graph-add.png"));
    act->setText(tr("InstantPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setCheckable(true);
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act,&QAction::triggered,this,&MainWindow::onInstantPopupCheckableTest);

}

void MainWindow::createCategoryOther(SARibbonCategory *page)
{
    QMenu* menu = new QMenu(this);
    QAction * item = menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));
    menu->addAction(QIcon(":/icon/icon/folder.png"),tr("1111111"));

    SARibbonPannel* pannel = page->addPannel(tr("pannel 1"));
    QToolButton* btn;
    btn = new QToolButton(this);
    btn->setIcon(QIcon(":/icon/icon/folder.png"));
    btn->setText(tr("text \n action"));
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addWidget(btn);

    btn = new QToolButton(this);
    btn->setIcon(QIcon(":/icon/icon/folder.png"));
    btn->setText(tr("text \n action"));
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addWidget(btn);

    btn = new QToolButton(this);
    btn->setIcon(QIcon(":/icon/icon/folder.png"));
    btn->setText(tr("text \n action"));
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addWidget(btn);

}

void MainWindow::onMenuButtonPopupCheckableTest(bool b)
{
    m_edit->append(tr("MenuButtonPopupCheckableTest : %1").arg(b));
}

void MainWindow::onInstantPopupCheckableTest(bool b)
{
    m_edit->append(tr("InstantPopupCheckableTest : %1").arg(b));
}

void MainWindow::onDelayedPopupCheckableTest(bool b)
{
    m_edit->append(tr("DelayedPopupCheckableTest : %1").arg(b));
}

void MainWindow::onMenuButtonPopupCheckabletriggered(bool b)
{
    m_edit->append(tr("MenuButtonPopupCheckabletriggered : %1").arg(b));
}

void MainWindow::onInstantPopupCheckabletriggered(bool b)
{
    m_edit->append(tr("InstantPopupCheckabletriggered : %1").arg(b));
}

void MainWindow::onDelayedPopupCheckabletriggered(bool b)
{
    m_edit->append(tr("DelayedPopupCheckabletriggered : %1").arg(b));
}

