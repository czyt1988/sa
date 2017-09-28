#ifndef SARIBBONTOOLBUTTON_H
#define SARIBBONTOOLBUTTON_H

#include <QToolButton>
///
/// \brief Ribbon界面适用的toolButton
///
class SARibbonToolButton : public QToolButton
{
    Q_OBJECT
    Q_ENUMS(RibbonButtonType)
public:
    ///
    /// \brief 按钮样式
    ///
    enum RibbonButtonType{
        LargeButton
        ,SmallButton
    };
    SARibbonToolButton(QWidget *parent = Q_NULLPTR);
    RibbonButtonType buttonType() const;
    void setButtonType(const RibbonButtonType &buttonType);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *e);
    virtual void paintLargeButton(QPaintEvent *e);
    virtual void paintSmallButton(QPaintEvent *e);
private:
    RibbonButtonType m_buttonType;
};

#endif // SARIBBONTOOLBUTTON_H
