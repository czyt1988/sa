#include "SAQwtAxisSetWidget.h"
#include "ui_SAQwtAxisSetWidget.h"
#include "qwt_plot.h"
#include "qwt_scale_div.h"
#include "qwt_scale_draw.h"
#include "qwt_scale_widget.h"
#include "qwt_date_scale_draw.h"
#include <QButtonGroup>
#include "SAChart.h"
#include <QDebug>
SAQwtAxisSetWidget::SAQwtAxisSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAQwtAxisSetWidget)
  ,m_chart(nullptr)
  ,m_axisID(QwtPlot::axisCnt)
{
    ui->setupUi(this);
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->radioButtonNormal,NormalScale);
    m_buttonGroup->addButton(ui->radioButtonTimeScale,DateTimeScale);
    ui->radioButtonNormal->setChecked(true);

    connect(ui->checkBoxEnable,&QCheckBox::stateChanged
            ,this,&SAQwtAxisSetWidget::onEnableCheckBoxClicked);
    connect(ui->lineEditTitle,&QLineEdit::textChanged
            ,this,&SAQwtAxisSetWidget::onLineEditTextChanged);
    connect(ui->fontSetWidget,&SAFontSetWidget::fontChanged
            ,this,&SAQwtAxisSetWidget::onAxisFontChanged);
    connect(ui->aligmentSetWidget,&SAAligmentSetWidget::alignmentStateChanged
            ,this,&SAQwtAxisSetWidget::onAxisLabelAligmentChanged);
    connect(ui->doubleSpinBoxRotation,static_cast<void(QDoubleSpinBox::*)(double v)>(QDoubleSpinBox::valueChanged)
            ,this,&SAQwtAxisSetWidget::onAxisLabelRotationChanged);
    connect(ui->spinBoxMargin,static_cast<void(QSpinBox::*)(int v)>(QSpinBox::valueChanged)
            ,this,&SAQwtAxisSetWidget::onAxisMarginValueChanged);
    connect(ui->doubleSpinBoxMax,static_cast<void(QDoubleSpinBox::*)(double v)>(QDoubleSpinBox::valueChanged)
            ,this,&SAQwtAxisSetWidget::onAxisMaxScaleChanged);
    connect(ui->doubleSpinBoxMin,static_cast<void(QDoubleSpinBox::*)(double v)>(QDoubleSpinBox::valueChanged)
            ,this,&SAQwtAxisSetWidget::onAxisMinScaleChanged);
    connect(m_buttonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAQwtAxisSetWidget::onScaleStyleChanged);
    enableWidget(false);
}

SAQwtAxisSetWidget::~SAQwtAxisSetWidget()
{
    delete ui;
}

void SAQwtAxisSetWidget::onEnableCheckBoxClicked(int state)
{
    bool enable = (Qt::Checked == state);
    foreach (QObject* obj, children())
    {
        if(obj->isWidgetType())
        {
            qobject_cast<QWidget*>(obj)->setEnabled(enable);
        }
    }
    ui->checkBoxEnable->setEnabled(true);
    if(m_chart)
    {
        m_chart->enableAxis(m_axisID,enable);
    }
    emit enableAxis(Qt::Checked == state,m_axisID);
}

void SAQwtAxisSetWidget::onLineEditTextChanged(const QString &text)
{
    if(m_chart)
    {
        SAChart::setAxisTitle(m_chart,m_axisID,text);
    }
}

void SAQwtAxisSetWidget::onAxisFontChanged(const QFont &font)
{
    if(m_chart)
    {
        SAChart::setAxisFont(m_chart,m_axisID,font);
    }
}

void SAQwtAxisSetWidget::onAxisLabelAligmentChanged(Qt::Alignment al)
{
    if(m_chart)
    {
        SAChart::setAxisLabelAlignment(m_chart,m_axisID,al);
    }
}

void SAQwtAxisSetWidget::onAxisLabelRotationChanged(double v)
{
    if(m_chart)
    {
        SAChart::setAxisLabelRotation(m_chart,m_axisID,v);
    }
}

void SAQwtAxisSetWidget::onAxisMarginValueChanged(int v)
{
    if(m_chart)
    {
        SAChart::setAxisMargin(m_chart,m_axisID,v);
    }
}

void SAQwtAxisSetWidget::onAxisMaxScaleChanged(double v)
{
    if(m_chart)
    {
        SAChart::setAxisScaleMax(m_chart,m_axisID,v);
    }
}

void SAQwtAxisSetWidget::onAxisMinScaleChanged(double v)
{
    if(m_chart)
    {
        SAChart::setAxisScaleMin(m_chart,m_axisID,v);
    }
}


void SAQwtAxisSetWidget::onScaleDivChanged()
{
    if(nullptr == m_chart)
    {
        return;
    }
    QwtInterval inv = m_chart->axisInterval(m_axisID);
    ui->doubleSpinBoxMin->setValue(inv.minValue());
    ui->doubleSpinBoxMax->setValue(inv.maxValue());
}

void SAQwtAxisSetWidget::onScaleStyleChanged(int id)
{
    if(NormalScale == id)
    {
        if(m_chart)
        {
            SAChart::setAxisNormalScale(m_chart,m_axisID);
        }
    }
    else
    {
        if(DateTimeScale == id)
        {
            if(m_chart)
            {
                QString format = ui->dateTimeScaleSetWidget->getTimeFormat();
                SAChart::setAxisDateTimeScale(m_chart,m_axisID,format);
            }
        }

    }
}



void SAQwtAxisSetWidget::updateUI()
{
    bool enable = ui->checkBoxEnable->isChecked();
    foreach (QObject* obj, children())
    {
        if(obj->isWidgetType())
        {
            qobject_cast<QWidget*>(obj)->setEnabled(enable);
        }
    }
    ui->checkBoxEnable->setEnabled(true);
}


void SAQwtAxisSetWidget::updateAxisValue(QwtPlot *chart,int axisID)
{
    enableWidget(nullptr != chart);
    if(nullptr == chart)
    {
        resetAxisValue();
        return;
    }
    bool b = chart->axisEnabled(axisID);
    ui->checkBoxEnable->setChecked(b);
    onEnableCheckBoxClicked(b?Qt::Checked:Qt::Unchecked);
    ui->lineEditTitle->setText(chart->axisTitle(axisID).text());
    ui->fontSetWidget->setCurrentFont(chart->axisFont(axisID));

    QwtInterval inv = chart->axisInterval(axisID);
    ui->doubleSpinBoxMin->setValue(inv.minValue());
    ui->doubleSpinBoxMax->setValue(inv.maxValue());
    ui->doubleSpinBoxMin->setDecimals(inv.minValue()<0.01?5:2);//显示小数点的位数调整
    ui->doubleSpinBoxMax->setDecimals(inv.minValue()<0.01?5:2);

    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(nullptr == ax)
    {
        ui->radioButtonNormal->setChecked(true);
        return;
    }
    QwtScaleDraw * sd = ax->scaleDraw();
    if(sd)
    {
        ui->doubleSpinBoxRotation->setValue(sd->labelRotation());
        ui->labelAligment->setAlignment(sd->labelAlignment());
    }
    ui->spinBoxMargin->setValue(ax->margin());
    QwtDateScaleDraw* dsd = dynamic_cast<QwtDateScaleDraw*>(sd);

    if(dsd)
    {
        ui->radioButtonTimeScale->setChecked(true);
        ui->dateTimeScaleSetWidget->setTimeFormatText(dsd->dateFormat(QwtDate::Second));
    }
    else
    {
        ui->radioButtonNormal->setChecked(true);
    }
}

void SAQwtAxisSetWidget::resetAxisValue()
{
    ui->checkBoxEnable->setChecked(false);
    ui->lineEditTitle->setText("");
    ui->fontSetWidget->setCurrentFont(QFont());
    ui->doubleSpinBoxMin->setValue(0);
    ui->doubleSpinBoxMax->setValue(0);
    ui->radioButtonNormal->setChecked(true);
    ui->doubleSpinBoxRotation->setValue(0);
    ui->labelAligment->setAlignment(Qt::AlignLeft);
    ui->spinBoxMargin->setValue(0);
    ui->radioButtonTimeScale->setChecked(false);
    ui->dateTimeScaleSetWidget->setTimeFormatText("");


}

void SAQwtAxisSetWidget::enableWidget(bool enable)
{
    ui->checkBoxEnable->setEnabled(enable);
    ui->lineEditTitle->setEnabled(enable);
    ui->fontSetWidget->setEnabled(enable);
    ui->doubleSpinBoxMin->setEnabled(enable);
    ui->doubleSpinBoxMax->setEnabled(enable);
    ui->radioButtonNormal->setEnabled(enable);
    ui->doubleSpinBoxRotation->setEnabled(enable);
    ui->labelAligment->setEnabled(enable);
    ui->spinBoxMargin->setEnabled(enable);
    ui->radioButtonTimeScale->setEnabled(enable);
    ui->dateTimeScaleSetWidget->setEnabled(enable);
}

void SAQwtAxisSetWidget::connectChartAxis()
{
    if(nullptr == m_chart)
    {
        return;
    }
    QwtScaleWidget* aw  = m_chart->axisWidget(m_axisID);
    if(aw)
    {
        connect(aw,&QwtScaleWidget::scaleDivChanged
                ,this,&SAQwtAxisSetWidget::onScaleDivChanged);
    }
}

void SAQwtAxisSetWidget::disconnectChartAxis()
{
    if(nullptr == m_chart)
    {
        return;
    }
    QwtScaleWidget* aw  = m_chart->axisWidget(m_axisID);
    if(aw)
    {
        disconnect(aw,&QwtScaleWidget::scaleDivChanged
                ,this,&SAQwtAxisSetWidget::onScaleDivChanged);
    }
}

QwtPlot *SAQwtAxisSetWidget::getChart() const
{
    return m_chart;
}

void SAQwtAxisSetWidget::setChart(QwtPlot *chart, int axisID)
{
    if(chart && m_chart && (m_chart != chart))
    {
        disconnectChartAxis();
    }

    m_chart = nullptr;//先设置为null，使得槽函数不动作
    updateAxisValue(chart,axisID);
    m_chart = chart;
    m_axisID = axisID;
    connectChartAxis();
}

void SAQwtAxisSetWidget::updateAxisValue()
{
    updateAxisValue(m_chart,m_axisID);

}


