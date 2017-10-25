#include "SARibbonButtonGroupWidget.h"
#include <QHBoxLayout>
class SARibbonButtonGroupWidgetPrivate
{
public:
    SARibbonButtonGroupWidget* Parent;
    SARibbonButtonGroupWidgetPrivate(SARibbonButtonGroupWidget* p)
    {
        Parent = p;
    }
    void init()
    {
        QHBoxLayout *layout = new QHBoxLayout;
        Parent->setLayout(layout);
    }
};

SARibbonButtonGroupWidget::SARibbonButtonGroupWidget(QWidget *parent)
    :QFrame(parent)
    ,m_d(new SARibbonButtonGroupWidgetPrivate(this))
{
    m_d->init();
}

SARibbonButtonGroupWidget::~SARibbonButtonGroupWidget()
{
    delete m_d;
}

void SARibbonButtonGroupWidget::addButton(QAbstractButton *btn)
{
    layout()->addWidget(btn);
}

QSize SARibbonButtonGroupWidget::sizeHint() const
{
    return layout()->sizeHint();
}

QSize SARibbonButtonGroupWidget::minimumSizeHint() const
{
    return layout()->minimumSize();
}
