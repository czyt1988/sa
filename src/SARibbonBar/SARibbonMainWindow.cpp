#include "SARibbonMainWindow.h"
#include "FramelessHelper.h"
#include "SAWindowButtonGroup.h"
#include "SARibbonBar.h"
#include "QAbstractButton"
#include <QTextEdit>
SARibbonMainWindow::SARibbonMainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    FramelessHelper *pHelper = new FramelessHelper(this);
    SARibbonBar* ribbonBar = new SARibbonBar(this);
    setMenuWidget(ribbonBar);
    new SAWindowButtonGroup(this);
    QTextEdit* edit = new QTextEdit(this);
    setCentralWidget(edit);
    setWindowTitle(tr("ribbon test"));
    ribbonBar->applitionButton()->setText(tr("File"));
}
