#include "SARibbonDrawHelper.h"

SARibbonDrawHelper::SARibbonDrawHelper()
{

}

void SARibbonDrawHelper::drawIcon(const QIcon &icon, QPainter *painter, const QStyleOption &opt
                                  ,int x,int y,int width,int height)
{
    icon.paint(painter,x,y,width,height,Qt::AlignCenter
               ,(opt.state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
               (opt.state & QStyle::State_Selected) ? QIcon::On : QIcon::Off);
}
