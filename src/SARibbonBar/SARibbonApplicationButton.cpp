#include "SARibbonApplicationButton.h"

SARibbonApplicationButton::SARibbonApplicationButton(QWidget *parent)
    :QPushButton(parent)
{
    setFlat(true);
    setStyleSheet(QString("QPushButton "
                           "{ "
                           "    color : #FFF;"
                           "    background-color: #2B579A; "
                           "    border:none;"
                           "}"
                           "QPushButton:hover {background-color:#58B4EA}"
                           "QPushButton:pressed {background-color:#2B579A}"
                           "QPushButton:focus{outline: none;}"));
}
