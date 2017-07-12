#include "SAChartNormalSetWidget.h"
#include <QApplication>
#include <QVBoxLayout>

#include "qwt_plot.h"
#include "SALineEditPropertyItem.h"
#include "SAColorSetPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include "SAVGroupBoxPropertyItem.h"
#include "SAFontComboBoxPropertyItem.h"
#include "SAAligmentPropertyItem.h"
#include <QMap>
#include <functional>
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "qwt_scale_draw.h"
class AxisPropertyItems
{
public:
    SAVGroupBoxPropertyItem* group;
    SALineEditPropertyItem* title;
    SAFontComboBoxPropertyItem* font;
    SADoubleSpinBoxPropertyItem* labelRotation;
    SADoubleSpinBoxPropertyItem* scaleMin;
    SADoubleSpinBoxPropertyItem* scaleMax;
    SASpinBoxPropertyItem* margin;
    SASpinBoxPropertyItem* spacing;

};

class SAChartNormalSetWidget::UI
{
public:

    QwtPlot* chart;
    QVBoxLayout *verticalLayout;
    SALineEditPropertyItem* titleEdit;
    SALineEditPropertyItem* footerEdit;
    SAColorSetPropertyItem* canvasBackgroundEdit;
    SADoubleSpinBoxPropertyItem* borderRadiusEdit;
    //x axis
    AxisPropertyItems xBottomSets;
    void setupUI(SAChartNormalSetWidget* par)
    {
        par->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        par->setMinimumSize(150,300);
        chart = nullptr;
        par->setObjectName(QStringLiteral("SAChartNormalSetWidget"));
        //Title
        titleEdit = new SALineEditPropertyItem(par);
        titleEdit->setObjectName("titleEdit");
        par->connect(titleEdit,&SALineEditPropertyItem::textChanged
                     ,par,&SAChartNormalSetWidget::onTitleTextChanged);
        //Footer
        footerEdit = new SALineEditPropertyItem(par);
        footerEdit->setObjectName("footerEdit");
        par->connect(titleEdit,&SALineEditPropertyItem::textChanged
                     ,par,&SAChartNormalSetWidget::onFooterTextChanged);
        //Canvas
        canvasBackgroundEdit = new SAColorSetPropertyItem(par);
        par->connect(canvasBackgroundEdit,&SAColorSetPropertyItem::colorChanged
                     ,par,&SAChartNormalSetWidget::onCanvasBackgroundColorChanged);
        //
        borderRadiusEdit = new SADoubleSpinBoxPropertyItem(par);
        par->connect(borderRadiusEdit,&SADoubleSpinBoxPropertyItem::valueChanged
                     ,par,&SAChartNormalSetWidget::onBorderRadiusChanged);
        //Axes
        setupAxisSet(par,xBottomSets,QwtPlot::xBottom);



        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        verticalLayout->addWidget(titleEdit);
        verticalLayout->addWidget(footerEdit);
        verticalLayout->addWidget(canvasBackgroundEdit);
        verticalLayout->addWidget(borderRadiusEdit);
        verticalLayout->addWidget(xBottomSets.group);
        verticalLayout->addStretch();
        par->setLayout(verticalLayout);
        retranslateUi(par);
    }
    void retranslateUi(SAChartNormalSetWidget *w)
    {
        w->setWindowTitle(QApplication::translate("SAChartNormalSetWidget", "normal set", 0));
        titleEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Title", 0));
        footerEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Footer", 0));
        canvasBackgroundEdit->setText(QApplication::translate("SAChartNormalSetWidget", "canvas Background", 0));
        borderRadiusEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Border Radius", 0));
        //x axis
        xBottomSets.group->setTitle(QApplication::translate("SAChartNormalSetWidget", "X Bottom", 0));
        xBottomSets.title->setText(QApplication::translate("SAChartNormalSetWidget", "Title", 0));
        xBottomSets.font->setText(QApplication::translate("SAChartNormalSetWidget", "Font", 0));
        xBottomSets.labelRotation->setText(QApplication::translate("SAChartNormalSetWidget", "Label Rotation", 0));
        xBottomSets.scaleMin->setText(QApplication::translate("SAChartNormalSetWidget", "Scale Min", 0));
        xBottomSets.scaleMax->setText(QApplication::translate("SAChartNormalSetWidget", "Scale Max", 0));
        xBottomSets.margin->setText(QApplication::translate("SAChartNormalSetWidget", "Margin", 0));
        xBottomSets.spacing->setText(QApplication::translate("SAChartNormalSetWidget", "spacing", 0));
    } // retranslateUi

    void setChart(QwtPlot *c)
    {
        this->chart = c;
        titleEdit->setEditText(c->title().text());
        footerEdit->setEditText(c->footer().text());
        canvasBackgroundEdit->setCurrentColor(c->canvasBackground().color());
        QWidget* w = c->canvas();
        QwtPlotCanvas* canvas = qobject_cast<QwtPlotCanvas*>(w);
        if(canvas)
        {
            borderRadiusEdit->setValue(canvas->borderRadius());
        }
        //Axes
        bool b = c->axisEnabled(QwtPlot::xBottom);
        xBottomSets.group->setChecked(b);
        if(b)
        {
            xBottomSets.title->setEditText(c->axisTitle(QwtPlot::xBottom).text());
            xBottomSets.font->setCurrentFont(c->axisFont(QwtPlot::xBottom));
            QwtInterval inv = c->axisInterval(QwtPlot::xBottom);
            xBottomSets.scaleMin->setValue(inv.minValue());
            xBottomSets.scaleMax->setValue(inv.maxValue());
            QwtScaleWidget * ax = c->axisWidget(QwtPlot::xBottom);
            if(ax)
            {
                QwtScaleDraw * sd = ax->scaleDraw();
                if(sd)
                {
                    xBottomSets.labelRotation->setValue(sd->labelRotation());

                }
                xBottomSets.margin->setValue(ax->margin());
                xBottomSets.spacing->setValue(ax->spacing());
            }
        }
    }

private:
    void setupAxisSet(SAChartNormalSetWidget* par
                      ,AxisPropertyItems& axisSets
                      ,int axisID)
    {
        axisSets.group = new SAVGroupBoxPropertyItem(par);
        //Axis title
        axisSets.title = new SALineEditPropertyItem(axisSets.group);
        //Axis Font
        axisSets.font = new SAFontComboBoxPropertyItem(axisSets.group);
        //scale min
        axisSets.labelRotation = new SADoubleSpinBoxPropertyItem(axisSets.group);
        //scale min
        axisSets.scaleMin = new SADoubleSpinBoxPropertyItem(axisSets.group);
        //scale max
        axisSets.scaleMax = new SADoubleSpinBoxPropertyItem(axisSets.group);
        //margin
        axisSets.margin = new SASpinBoxPropertyItem(axisSets.group);
        axisSets.margin->setMinimum(0);
        //spacing
        axisSets.spacing = new SASpinBoxPropertyItem(axisSets.group);

        axisSets.group->addWidget(axisSets.title);
        axisSets.group->addWidget(axisSets.font);
        axisSets.group->addWidget(axisSets.labelRotation);
        axisSets.group->addWidget(axisSets.scaleMin);
        axisSets.group->addWidget(axisSets.scaleMax);

        axisSets.group->addWidget(axisSets.margin);
        axisSets.group->addWidget(axisSets.spacing);
        switch(axisID)
        {
        case QwtPlot::xBottom:
        {
            par->connect(axisSets.group,&SAVGroupBoxPropertyItem::clicked
                    ,par,&SAChartNormalSetWidget::onEnableXAxisChanged);
            par->connect(axisSets.title,&SALineEditPropertyItem::textChanged
                    ,par,&SAChartNormalSetWidget::onXBottomTitleChanged);
            par->connect(axisSets.font,&SAFontComboBoxPropertyItem::currentFontChanged
                    ,par,&SAChartNormalSetWidget::onXAxisFontChanged);
            par->connect(axisSets.labelRotation,&SADoubleSpinBoxPropertyItem::valueChanged
                    ,par,&SAChartNormalSetWidget::onXAxisLabelRotationChanged);
            par->connect(axisSets.scaleMin,&SADoubleSpinBoxPropertyItem::valueChanged
                    ,par,&SAChartNormalSetWidget::onXAxisScaleMinChanged);
            par->connect(axisSets.scaleMax,&SADoubleSpinBoxPropertyItem::valueChanged
                    ,par,&SAChartNormalSetWidget::onXAxisScaleMaxChanged);
            par->connect(axisSets.margin,&SASpinBoxPropertyItem::valueChanged
                    ,par,&SAChartNormalSetWidget::onXAxisMarginChanged);
            par->connect(axisSets.spacing,&SASpinBoxPropertyItem::valueChanged
                    ,par,&SAChartNormalSetWidget::onXAxisSpacingChanged);
         }
            break;

        }

    }
};

SAChartNormalSetWidget::SAChartNormalSetWidget(QWidget *par):QWidget(par)
  ,ui(new SAChartNormalSetWidget::UI)
{
    ui->setupUI(this);
}

SAChartNormalSetWidget::~SAChartNormalSetWidget()
{
    delete ui;
}

void SAChartNormalSetWidget::setChart(QwtPlot *chart)
{
    ui->setChart(chart);
}

void SAChartNormalSetWidget::onTitleTextChanged(const QString &text)
{
    if(ui->chart)
    {
        ui->chart->setTitle(text);
        emit chartTitleChanged(text);
    }
}

void SAChartNormalSetWidget::onFooterTextChanged(const QString &text)
{
    if(ui->chart)
    {
        ui->chart->setFooter(text);
    }
}

void SAChartNormalSetWidget::onCanvasBackgroundColorChanged(const QColor &clr)
{
    if(ui->chart)
    {
        ui->chart->setCanvasBackground(clr);
        QwtPlotCanvas* canvas = qobject_cast<QwtPlotCanvas*>(ui->chart->canvas());
        if(canvas)
        {
            canvas->update();
        }
        ui->chart->update();
    }
}

void SAChartNormalSetWidget::onBorderRadiusChanged(double v)
{
    if(ui->chart)
    {
        QWidget* w = ui->chart->canvas();
        QwtPlotCanvas* canvas = qobject_cast<QwtPlotCanvas*>(w);
        if(canvas)
        {
            canvas->setBorderRadius(v);
        }
    }
}

void SAChartNormalSetWidget::onEnableXAxisChanged(bool b)
{
    setAxisEnable(ui->chart,QwtPlot::xBottom,b);
}

void SAChartNormalSetWidget::onXBottomTitleChanged(const QString &text)
{
    setAxisTitle(ui->chart,QwtPlot::xBottom,text);
}

void SAChartNormalSetWidget::onXAxisFontChanged(const QFont &font)
{
    setAxisFont(ui->chart,QwtPlot::xBottom,font);
}

void SAChartNormalSetWidget::onXAxisLabelRotationChanged(double v)
{
    setAxisLabelRotation(ui->chart,QwtPlot::xBottom,v);
}

void SAChartNormalSetWidget::onXAxisScaleMinChanged(double v)
{
    setAxisScaleMin(ui->chart,QwtPlot::xBottom,v);
}

void SAChartNormalSetWidget::onXAxisScaleMaxChanged(double v)
{
    setAxisScaleMax(ui->chart,QwtPlot::xBottom,v);
}

void SAChartNormalSetWidget::onXAxisMarginChanged(int v)
{
    setAxisMargin(ui->chart,QwtPlot::xBottom,v);
}

void SAChartNormalSetWidget::onXAxisSpacingChanged(int v)
{
    setAxisSpacing(ui->chart,QwtPlot::xBottom,v);
}



void SAChartNormalSetWidget::setAxisEnable(QwtPlot *chart, int axisID, bool b)
{
    if(chart)
    {
        chart->enableAxis(axisID,b);
    }
}

void SAChartNormalSetWidget::setAxisTitle(QwtPlot*chart,int axisID,const QString &text)
{
    if(chart)
    {
        chart->setAxisTitle(axisID,text);
    }
}

void SAChartNormalSetWidget::setAxisFont(QwtPlot*chart,int axisID, const QFont &font)
{
    if(chart)
    {
        chart->setAxisFont(axisID,font);
    }
}

void SAChartNormalSetWidget::setAxisLabelRotation(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        chart->setAxisLabelRotation(axisID,v);
    }
}

void SAChartNormalSetWidget::setAxisScaleMin(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        QwtInterval inv = chart->axisInterval(axisID);
        chart->setAxisScale(axisID,v,inv.maxValue());
    }
}

void SAChartNormalSetWidget::setAxisScaleMax(QwtPlot *chart, int axisID, double v)
{
    if(chart)
    {
        QwtInterval inv = chart->axisInterval(axisID);
        chart->setAxisScale(axisID,inv.minValue(),v);
    }
}

void SAChartNormalSetWidget::setAxisBorderDistStart(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setBorderDist(v,ax->endBorderDist());
    }
}

void SAChartNormalSetWidget::setAxisBorderDistEnd(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setBorderDist(ax->startBorderDist(),v);
    }
}

void SAChartNormalSetWidget::setAxisMargin(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setMargin(v);
    }
}

void SAChartNormalSetWidget::setAxisSpacing(QwtPlot *chart, int axisID, int v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setSpacing(v);
    }
}



