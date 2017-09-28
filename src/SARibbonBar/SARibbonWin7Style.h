#ifndef SARIBBONWIN7STYLE_H
#define SARIBBONWIN7STYLE_H
#include "SARibbonStyle.h"
#include <QPalette>
#include <QRect>
QT_BEGIN_NAMESPACE
class QPainterPath;
QT_END_NAMESPACE
class QStyleOptionToolButton;
class SARibbonWin7Style : public SARibbonStyle
{
    Q_OBJECT
public:
    SARibbonWin7Style();
    int styleHint(StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData) const Q_DECL_OVERRIDE;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const Q_DECL_OVERRIDE;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const Q_DECL_OVERRIDE;


    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option
                                    , QPainter *painter, const QWidget *widget = Q_NULLPTR) const Q_DECL_OVERRIDE;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *option
                         , SubControl sc, const QWidget *widget) const Q_DECL_OVERRIDE;
private:
    void drawComplexControlRibbonToolButton(QStyle::ComplexControl control, const QStyleOptionToolButton* toolbutton
                                            , QPainter *p, const QWidget *widget) const;
private:
    mutable bool m_isDrawingRibbonToolButton;
};

#endif // SARIBBONWIN7STYLE_H
