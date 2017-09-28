#include "SARibbonWin7Style.h"
#include <QStyleOptionComplex>
#include <QDebug>
SARibbonWin7Style::SARibbonWin7Style()
    :SARibbonStyle()
    ,m_isDrawingRibbonToolButton(false)
{

}

int SARibbonWin7Style::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    return SARibbonStyle::styleHint(hint,option,widget,returnData);
}

void SARibbonWin7Style::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    SARibbonStyle::drawPrimitive(element,option,painter,widget);
}

void SARibbonWin7Style::drawControl(QStyle::ControlElement control, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    qDebug() << "drawControl"<<control;
    SARibbonStyle::drawControl(control,option,painter,widget);
}

void SARibbonWin7Style::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    qDebug() << "drawComplexControl"<<control;
    switch(control)
    {
    case SARibbonStyle::CC_RibbonToolButton:
    {
        m_isDrawingRibbonToolButton = true;
        SARibbonStyle::drawComplexControl(CC_ToolButton,option,painter,widget);
        m_isDrawingRibbonToolButton = false;
        break;
    }
    default:
        SARibbonStyle::drawComplexControl(control,option,painter,widget);
    }


}

QRect SARibbonWin7Style::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *option, QStyle::SubControl sc, const QWidget *widget) const
{ 
    switch(cc)
    {
    case SARibbonStyle::CC_ToolButton:
    {
        if(m_isDrawingRibbonToolButton)
        {
            if (const QStyleOptionToolButton *tb = qstyleoption_cast<const QStyleOptionToolButton *>(option))
            {
                int mbi = proxy()->pixelMetric(PM_MenuButtonIndicator, tb, widget);
                QRect ret = tb->rect;
                switch (sc)
                {
                case SC_ToolButton:
                    if ((tb->features
                         & (QStyleOptionToolButton::MenuButtonPopup | QStyleOptionToolButton::PopupDelay))
                        == QStyleOptionToolButton::MenuButtonPopup)
                        ret.adjust(0, 0, 0, -mbi);
                    break;
                case SC_ToolButtonMenu:
                    if ((tb->features
                         & (QStyleOptionToolButton::MenuButtonPopup | QStyleOptionToolButton::PopupDelay))
                        == QStyleOptionToolButton::MenuButtonPopup)
                        ret.adjust(0, ret.height() -mbi , 0, 0);
                    break;
                default:
                    break;
                }
                ret = visualRect(tb->direction, tb->rect, ret);
                return ret;
            }
        }
    }
    break;
    }
    return SARibbonStyle::subControlRect(cc,option,sc,widget);
}

///
/// \brief 绘制CC_RibbonToolButton
/// \param option
/// \param painter
/// \param widget
///
void SARibbonWin7Style::drawComplexControlRibbonToolButton(ComplexControl control, const QStyleOptionToolButton *toolbutton, QPainter *p, const QWidget *widget) const
{
    State flags = toolbutton->state;
    SubControls sub = toolbutton->subControls;
    QRect r = toolbutton->rect;

    int partId = 0;
    int stateId = 0;
    if (widget && widget->testAttribute(Qt::WA_UnderMouse) && widget->isActiveWindow())
        flags |= State_MouseOver;

    QRect button, menuarea;
    button = proxy()->subControlRect(control, toolbutton, SC_ToolButton, widget);
    menuarea = proxy()->subControlRect(control, toolbutton, SC_ToolButtonMenu, widget);
    State bflags = toolbutton->state & ~State_Sunken;
    State mflags = bflags;
    bool autoRaise = flags & State_AutoRaise;
    if (autoRaise) {
        if (!(bflags & State_MouseOver) || !(bflags & State_Enabled)) {
            bflags &= ~State_Raised;
        }
    }

    if (toolbutton->state & State_Sunken) {
        if (toolbutton->activeSubControls & SC_ToolButton) {
            bflags |= State_Sunken;
            mflags |= State_MouseOver | State_Sunken;
        } else if (toolbutton->activeSubControls & SC_ToolButtonMenu) {
            mflags |= State_Sunken;
            bflags |= State_MouseOver;
        }
    }

//    QStyleOption tool(0);
//    tool.palette = toolbutton->palette;
//    if (toolbutton->subControls & SC_ToolButton) {
//        if (flags & (State_Sunken | State_On | State_Raised) || !autoRaise) {
//            if (toolbutton->features & QStyleOptionToolButton::MenuButtonPopup && autoRaise) {
//                XPThemeData theme(widget, p, QWindowsXPStylePrivate::ToolBarTheme);
//                theme.partId = TP_SPLITBUTTON;
//                theme.rect = button;
//                if (!(bflags & State_Enabled))
//                    stateId = TS_DISABLED;
//                else if (bflags & State_Sunken)
//                    stateId = TS_PRESSED;
//                else if (bflags & State_MouseOver || !(flags & State_AutoRaise))
//                    stateId = flags & State_On ? TS_HOTCHECKED : TS_HOT;
//                else if (bflags & State_On)
//                    stateId = TS_CHECKED;
//                else
//                    stateId = TS_NORMAL;
//                if (option->direction == Qt::RightToLeft)
//                    theme.mirrorHorizontally = true;
//                theme.stateId = stateId;
//                d->drawBackground(theme);
//            } else {
//                tool.rect = option->rect;
//                tool.state = bflags;
//                if (autoRaise) // for tool bars
//                    proxy()->drawPrimitive(PE_PanelButtonTool, &tool, p, widget);
//                else
//                    proxy()->drawPrimitive(PE_PanelButtonBevel, &tool, p, widget);
//            }
//        }
//    }
}

