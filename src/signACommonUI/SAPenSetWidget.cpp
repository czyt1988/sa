#include "SAPenSetWidget.h"
#include "ui_SAPenSetWidget.h"

SAPenSetWidget::SAPenSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAPenSetWidget)
  ,m_enableEmit(true)
{
    ui->setupUi(this);
    m_enableEmit = false;
    connect(ui->pushButtonColor,&SAColorPickerButton::colorChanged
            ,this,&SAPenSetWidget::onCurrentColorChanged);
    connect(ui->spinBoxPenWidth,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            ,this,&SAPenSetWidget::onCurrentPenWidthChanged);
    connect(ui->comboBoxPenStyle,&SAPenStyleComboBox::penStyleChanged
            ,this,&SAPenSetWidget::onCurrentPenStyleChanged);
    m_enableEmit = true;
}

SAPenSetWidget::~SAPenSetWidget()
{
    delete ui;
}

QPen SAPenSetWidget::getPen() const
{
    return m_curPen;
}

void SAPenSetWidget::setPen(const QPen &pen)
{
    m_curPen = pen;
    m_enableEmit = false;
    ui->pushButtonColor->setCurrentColor(pen.color());
    ui->comboBoxPenStyle->setStyle(pen.style());
    ui->spinBoxPenWidth->setValue(pen.width());
    m_enableEmit = true;
}

void SAPenSetWidget::onCurrentColorChanged(const QColor &clr)
{
    m_curPen.setColor(clr);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

void SAPenSetWidget::onCurrentPenWidthChanged(int v)
{
    m_curPen.setWidth(v);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

void SAPenSetWidget::onCurrentPenStyleChanged(Qt::PenStyle style)
{
    m_curPen.setStyle(style);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

