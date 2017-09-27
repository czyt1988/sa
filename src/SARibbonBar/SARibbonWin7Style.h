#ifndef SARIBBONWIN7STYLE_H
#define SARIBBONWIN7STYLE_H
#include <QProxyStyle>
#include <QPalette>
QT_BEGIN_NAMESPACE
class QPainterPath;
QT_END_NAMESPACE
class SARibbonWin7Style : public QProxyStyle
{
    Q_OBJECT
public:
    SARibbonWin7Style(QStyle *style);
    int styleHint(StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData) const Q_DECL_OVERRIDE;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const Q_DECL_OVERRIDE;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const Q_DECL_OVERRIDE;


};

#endif // SARIBBONWIN7STYLE_H
