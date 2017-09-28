#include "SARibbonStyle.h"
#include <QStyleFactory>
SARibbonStyle::SARibbonStyle()
    :QProxyStyle(QStyleFactory::create("windowsvista"))
{

}

