#include "SARibbonComboBox.h"
#include <QPainter>
#include <QPaintEvent>
#include <QStylePainter>
#include <QDebug>
class SARibbonComboBoxPrivate
{
public:
    SARibbonComboBox* Parent;
    QComboBox* comboxWidget;
    SARibbonComboBoxPrivate(SARibbonComboBox* p)
    {
        Parent = p;
    }
    void init()
    {
        comboxWidget = new QComboBox(Parent);
        comboxWidget->setObjectName(QStringLiteral("SARibbonComboBox-QComboBox"));
        Parent->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    }
};

SARibbonComboBox::SARibbonComboBox(QWidget *parent):QWidget(parent)
  ,m_d(new SARibbonComboBoxPrivate(this))
{
    m_d->init();
}

QSize SARibbonComboBox::sizeHint() const
{
    QSize comboSizeHint = m_d->comboxWidget->sizeHint();
    QIcon icon = windowIcon();
    if(!icon.isNull())
    {
        comboSizeHint.setWidth(comboSizeHint.width() + comboSizeHint.height());
    }
    QString text = windowTitle();
    if(!text.isEmpty())
    {
        int textWidth = fontMetrics().width(text);
        comboSizeHint.setWidth(comboSizeHint.width() + textWidth);
    }
    return comboSizeHint;
}

QSize SARibbonComboBox::minimumSizeHint() const
{
    QSize comboSizeHint = m_d->comboxWidget->minimumSizeHint();
    QIcon icon = windowIcon();
    if(!icon.isNull())
    {
        comboSizeHint.setWidth(comboSizeHint.width() + comboSizeHint.height());
    }
    QString text = windowTitle();
    if(!text.isEmpty())
    {
        int textWidth = fontMetrics().width(text.at(0));
        comboSizeHint.setWidth(comboSizeHint.width() + (2*textWidth));
    }
    return comboSizeHint;
}

QComboBox *SARibbonComboBox::comboBox()
{
    return m_d->comboxWidget;
}

void SARibbonComboBox::paintEvent(QPaintEvent *e)
{
    QStylePainter painter(this);

    // draw the combobox frame, focusrect and selected etc.
    QStyleOption opt;
    initStyleOption(&opt);
    //绘制图标
    QIcon icon = windowIcon();
    const int widgetHeight = height();
    int x = 0;
    if(!icon.isNull())
    {
        //绘制图标
        QSize iconSize = icon.actualSize(QSize(widgetHeight,widgetHeight),
                                (opt.state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                                (opt.state & QStyle::State_Selected) ? QIcon::On : QIcon::Off  );
        icon.paint(&painter,x,0,widgetHeight,widgetHeight,Qt::AlignCenter
                   ,(opt.state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                   (opt.state & QStyle::State_Selected) ? QIcon::On : QIcon::Off);
        x += iconSize.width() + 4;
    }
    //绘制文字
    QString text = windowTitle();
    if(!text.isEmpty())
    {
        int textWidth = opt.fontMetrics.width(text);
        if(textWidth > (opt.rect.width() - widgetHeight - x))
        {
            textWidth = opt.rect.width() - widgetHeight - x;
            text = opt.fontMetrics.elidedText(text,Qt::ElideRight,textWidth);
        }
        if(textWidth > 0)
        {
            painter.drawItemText(QRect(x,0,textWidth,opt.rect.height())
                                 ,Qt::AlignLeft|Qt::AlignVCenter
                                 ,opt.palette
                                 ,opt.state & QStyle::State_Enabled
                                 ,text
                                 );
        }

    }
    QWidget::paintEvent(e);
}

void SARibbonComboBox::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    QStyleOption opt;
    initStyleOption(&opt);
    QIcon icon = windowIcon();
    int x = 0;
    const int widgetHeight = height();
    if(!icon.isNull())
    {
        QSize iconSize = icon.actualSize(QSize(widgetHeight,widgetHeight),
                                (opt.state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                                (opt.state & QStyle::State_Selected) ? QIcon::On : QIcon::Off  );
        x += iconSize.width();
        x += 4;
    }
    //绘制文字
    QString text = windowTitle();
    if(!text.isEmpty())
    {
        int textWidth = opt.fontMetrics.width(text);
        if(textWidth > (opt.rect.width() - widgetHeight - x))
        {
            textWidth = opt.rect.width() - widgetHeight - x;
            text = opt.fontMetrics.elidedText(text,Qt::ElideRight,textWidth);
        }
        if(textWidth > 0)
        {
            x += textWidth;
            x += 2;
        }
    }
    m_d->comboxWidget->setGeometry(x,0,width()-x,height());
}

void SARibbonComboBox::initStyleOption(QStyleOption *opt)
{
    opt->initFrom(this);
}

