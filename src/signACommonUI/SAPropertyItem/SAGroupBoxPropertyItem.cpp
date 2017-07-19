#include "SAGroupBoxPropertyItem.h"
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


SAGroupBoxPropertyItem::SAGroupBoxPropertyItem(QWidget* par)
    :ctkCollapsibleGroupBox(par)
{
}

SAGroupBoxPropertyItem::SAGroupBoxPropertyItem(const QString &title, QWidget *par)
    :ctkCollapsibleGroupBox(title,par)
{

}

SAGroupBoxPropertyItem::~SAGroupBoxPropertyItem()
{

}

void SAGroupBoxPropertyItem::setTreeMode(bool b)
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

