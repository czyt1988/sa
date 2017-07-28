#include "SAQwtSymbolSetWidget.h"
#include "ui_SAQwtSymbolSetWidget.h"
#include <QPen>
#include <QPainter>
#include <QDebug>

SAQwtSymbolSetWidget::SAQwtSymbolSetWidget(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::SAQwtSymbolSetWidget)
  ,m_signalLock(false)
{
    ui->setupUi(this);
    QPalette pl = palette();
    pl.setBrush(QPalette::Window,pl.window());
    pl.setColor(QPalette::Window,pl.window().color());
    setPalette(pl);
    init();
}

SAQwtSymbolSetWidget::SAQwtSymbolSetWidget(const QwtSymbol *symbol, QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAQwtSymbolSetWidget)
    ,m_signalLock(false)
{
    ui->setupUi(this);
    m_symbol.setStyle(symbol->style());
    m_symbol.setPen(symbol->pen());
    m_symbol.setBrush(symbol->brush());
    m_symbol.setSize(symbol->size());
    init();
}

SAQwtSymbolSetWidget::~SAQwtSymbolSetWidget()
{
    delete ui;
}

const QwtSymbol &SAQwtSymbolSetWidget::getSymbol()
{
    return m_symbol;
}



void SAQwtSymbolSetWidget::init()
{
    ui->colorPushButtonBrush->setStandardColors();
    updateSymbol();

    connect(ui->comboBoxSymbolStyle,&SAQwtSymbolComboBox::symbolSelectChanged
            ,this,&SAQwtSymbolSetWidget::onSymbolSelectChanged);
    connect(ui->spinBoxSizeX,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            ,this,&SAQwtSymbolSetWidget::onSpinBoxSizeXValueChanged);
    connect(ui->spinBoxSizeY,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            ,this,&SAQwtSymbolSetWidget::onSpinBoxSizeYValueChanged);
    connect(ui->colorPushButtonBrush,&SAColorPickerButton::colorChanged
            ,this,&SAQwtSymbolSetWidget::onBrushColorChanged);
    connect(ui->edgetPenSet,&SAPenSetWidget::penChanged
            ,this,&SAQwtSymbolSetWidget::onEdgePenChanged);
    connect(ui->horizontalSliderBrushAlpha,&QSlider::valueChanged
            ,this,&SAQwtSymbolSetWidget::onBrushAlphaSliderValueChanged);
}

void SAQwtSymbolSetWidget::updateSymbol()
{
    QPixmap pixmap(ui->labelPreview->size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    m_symbol.drawSymbol(&painter,pixmap.rect());
    ui->labelPreview->setPixmap(pixmap);
    ui->labelPreview->repaint();
}

void SAQwtSymbolSetWidget::onSymbolSelectChanged(QwtSymbol::Style style)
{
    m_symbol.setStyle(style);
    updateSymbol();
    emit symbolSetChanged(m_symbol);
}

void SAQwtSymbolSetWidget::onSpinBoxSizeXValueChanged(int v)
{
    if(m_signalLock)
    {
        return;
    }
    QSize symbolSize;
    if(ui->toolButtonFixWidthHeight->isChecked())
    {
        symbolSize.setWidth(v);
        symbolSize.setHeight(v);
        m_signalLock = true;
        ui->spinBoxSizeY->setValue(v);
        m_signalLock = false;
    }
    else
    {
        symbolSize.setWidth(ui->spinBoxSizeX->value());
        symbolSize.setHeight(ui->spinBoxSizeY->value());
    }
    m_symbol.setSize(symbolSize);
    updateSymbol();
    emit symbolSetChanged(m_symbol);
}

void SAQwtSymbolSetWidget::onSpinBoxSizeYValueChanged(int v)
{
    if(m_signalLock)
    {
        return;
    }
    QSize symbolSize;
    if(ui->toolButtonFixWidthHeight->isChecked())
    {
        symbolSize.setWidth(v);
        symbolSize.setHeight(v);
        m_signalLock = true;
        ui->spinBoxSizeX->setValue(v);
        m_signalLock = false;
    }
    else
    {
        symbolSize.setWidth(ui->spinBoxSizeX->value());
        symbolSize.setHeight(ui->spinBoxSizeY->value());
    }
    m_symbol.setSize(symbolSize);
    updateSymbol();
    emit symbolSetChanged(m_symbol);
}

void SAQwtSymbolSetWidget::onEdgePenChanged(const QPen &pen)
{
    m_symbol.setPen(pen);
    updateSymbol();
    emit symbolSetChanged(m_symbol);
}


void SAQwtSymbolSetWidget::onBrushColorChanged(const QColor &clr)
{
    QBrush symbolBrush = m_symbol.brush();
    symbolBrush.setColor(clr);
    m_symbol.setBrush(symbolBrush);
    updateSymbol();
    emit symbolSetChanged(m_symbol);
}


void SAQwtSymbolSetWidget::onBrushAlphaSliderValueChanged(int v)
{
    QBrush symbolBrush = m_symbol.brush();
    QColor clr = symbolBrush.color();
    clr.setAlpha(v);
    symbolBrush.setColor(clr);
    m_symbol.setBrush(symbolBrush);
    updateSymbol();
    emit symbolSetChanged(m_symbol);
}



