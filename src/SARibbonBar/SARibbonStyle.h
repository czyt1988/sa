#ifndef SARIBBONSTYLE_H
#define SARIBBONSTYLE_H
#include <QProxyStyle>
///
/// \brief ribbon 样式的基本定义
///
class SARibbonStyle : public QProxyStyle
{
    Q_OBJECT
public:
    SARibbonStyle();
    enum RibbonControlElement
    {
        //CE_RibbonToolButton = QStyle::CE_CustomBase + 1000 ///< ribbon基本控件的按钮
    };
    enum RibbonComplexControl
    {
        CC_RibbonToolButton = QStyle::CC_CustomBase + 1000 ///< ribbon基本控件的按钮
    };
    enum RibbonSubControl
    {
        //SC_RibbonToolButton = QStyle::SC_CustomBase + 1000 ///< 对应SC_ToolButton
    };
};

#endif // SARIBBONSTYLE_H
