#include "SACollapsibleGroupBox.h"
#include <QApplication>

const QString groupbox_tree_mode_style =
        "QGroupBox::indicator {"
            "width: 16px;"
            "height: 16px;"
        "}"
        "QGroupBox::indicator:unchecked {"
            "image: url(:/icon/icons/colled.png);"
        "}"
        "QGroupBox::indicator:checked {"
            "image: url(:/icon/icons/expand.png);"
        "}"
    ;


SACollapsibleGroupBox::SACollapsibleGroupBox(QWidget* par)
    :ctkCollapsibleGroupBox(par)
{
}

SACollapsibleGroupBox::SACollapsibleGroupBox(const QString &title, QWidget *par)
    :ctkCollapsibleGroupBox(title,par)
{

}

SACollapsibleGroupBox::~SACollapsibleGroupBox()
{

}

void SACollapsibleGroupBox::setTreeMode(bool b)
{
    if(b)
    {
        setStyleSheet(groupbox_tree_mode_style);
    }
    else
    {
        setStyleSheet("");
    }
}

