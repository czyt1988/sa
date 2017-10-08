#ifndef SARIBBONPANNEL_H
#define SARIBBONPANNEL_H
#include "SARibbonGlobal.h"
#include <QWidget>
class SARibbonToolButton;
class QGridLayout;
class SARibbonPannelOptionButton;
///
/// \brief The SARibbonPannel class
///
class SA_RIBBON_EXPORT SARibbonPannel : public QWidget
{
    Q_OBJECT
public:
    SARibbonPannel(QWidget* parent = 0);
    using QWidget::addAction;
    SARibbonToolButton* addLargeAction(QAction *action);
    SARibbonToolButton *addSmallAction(QAction *action);
    void addSeparator();
    void addWidget(QWidget* w, int row=0);
    void addOptionAction(QAction* action);
protected:
    static QSize maxHightIconSize(const QSize& size,int height);
    void paintEvent(QPaintEvent *event);
    virtual QSize sizeHint() const;
    virtual void resizeEvent(QResizeEvent *event);
private:
    QGridLayout* m_gridLayout;
    QPoint m_nextElementPosition;
    int m_row;
    SARibbonPannelOptionButton* m_optionActionButton;
    int m_titleOptionButtonSpace;///< 标题和项目按钮的间隔
    int m_titleHeight;///< 标题的高度
    int m_titleY;///< 标题栏的y距离

};

#endif // SARIBBONPANNEL_H
