#include "SAQwtAxisSetWidget.h"
#include "ui_SAQwtAxisSetWidget.h"
#include "qwt_plot.h"
#include "qwt_scale_div.h"
#include "qwt_scale_draw.h"
#include "qwt_scale_widget.h"
#include "qwt_date_scale_draw.h"
#include <QButtonGroup>
#include "SAChart.h"
SAQwtAxisSetWidget::SAQwtAxisSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAQwtAxisSetWidget)
  ,m_chart(nullptr)
{
    ui->setupUi(this);
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->radioButtonNormal,NormalScale);
    m_buttonGroup->addButton(ui->radioButtonTimeScale,DateTimeScale);
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

void SAQwtAxisSetWidget::onCharDestroy(QObject *obj)
{
    Q_UNUSED(obj);
    m_chart = nullptr;
}

void SAQwtAxisSetWidget::onScaleDivChanged()
{

}

void SAQwtAxisSetWidget::onScaleStyleChanged(int id)
{
    if(NormalScale == id)
    {
        ui->stackedWidget->hide();
        if(m_chart)
        {
            SAChart::setAxisNormalScale(m_chart,m_axisID);
        }
    }
    else
    {
        if(DateTimeScale == id)
        {
            ui->stackedWidget->setCurrentWidget(ui->dateTimeScaleSetWidget);
            if(m_chart)
            {
                QString format = ui->dateTimeScaleSetWidget->text();
                SAChart::setAxisDateTimeScale(m_chart,m_axisID,format);
            }
        }
        ui->stackedWidget->show();
    }
}

void SAQwtAxisSetWidget::connectChartAxis()
{
    connect(m_chart,&QObject::destroyed
               ,this,&SAQwtAxisSetWidget::onCharDestroy);
    QwtScaleWidget* aw  = m_chart->axisWidget(m_axisID);
    if(aw)
    {
        connect(aw,&QwtScaleWidget::scaleDivChanged
                ,this,&SAQwtAxisSetWidget::onScaleDivChanged);
    }
}

void SAQwtAxisSetWidget::disconnectChartAxis()
{
    disconnect(m_chart,&QObject::destroyed
               ,this,&SAQwtAxisSetWidget::onCharDestroy);
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
    if(m_chart)
    {
        disconnectChartAxis();
    }
    m_chart = chart;
    m_axisID = axisID;
    connectChartAxis();
    updateAxisValue();
}

void SAQwtAxisSetWidget::updateAxisValue()
{
    if(nullptr == m_chart)
    {
        return;
    }
    bool b = m_chart->axisEnabled(m_axisID);
    ui->checkBoxEnable->setChecked(b);
    onEnableCheckBoxClicked(b?Qt::Checked:Qt::Unchecked);
    ui->lineEditTitle->setText(m_chart->axisTitle(m_axisID).text());
    ui->fontSetWidget->setCurrentFont(m_chart->axisFont(m_axisID));
    QwtInterval inv = m_chart->axisInterval(m_axisID);
    ui->doubleSpinBoxMin->setValue(inv.minValue());
    ui->doubleSpinBoxMax->setValue(inv.maxValue());
    QwtScaleWidget * ax = m_chart->axisWidget(m_axisID);
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
//        if(!ui->stackedWidget->isVisible())
//        {
//            ui->stackedWidget->setVisible(true);
//        }
//        ui->stackedWidget->setCurrentWidget(ui->dateTimeScaleSetWidget);
        ui->dateTimeScaleSetWidget->setText(dsd->dateFormat(QwtDate::Second));
    }
    else
    {
        ui->radioButtonNormal->setChecked(true);
    }

}
