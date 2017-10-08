#include "SARibbonPannelOptionButton.h"

SARibbonPannelOptionButton::SARibbonPannelOptionButton(QWidget *parent)
    :QToolButton(parent)
{
    setAutoRaise(true);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setFixedSize(16,16);
    setIconSize(QSize(10,10));
    setIcon(QIcon(":/image/resource/ribbonPannelOptionButton.png"));
}
