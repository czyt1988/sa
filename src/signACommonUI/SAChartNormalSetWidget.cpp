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
#include <QHash>
#include <functional>
#include <array>
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
    SAAligmentPropertyItem* labelAligment;
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
    enum AxisIndex
    {
        xBottom = 0
        ,xTop = 1
        ,yLeft = 2
        ,yRight = 3
    };
    std::array<AxisPropertyItems,4> axisSet;

    void setupUI(SAChartNormalSetWidget* par)
    {
        par->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        par->setMinimumSize(150,500);
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
        par->connect(footerEdit,&SALineEditPropertyItem::textChanged
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
        setupAxisSet(par,axisSet.at(xBottom),QwtPlot::xBottom);
        setupAxisSet(par,axisSet.at(yLeft),QwtPlot::yLeft);
        setupAxisSet(par,axisSet.at(xTop),QwtPlot::xTop);
        setupAxisSet(par,axisSet.at(yRight),QwtPlot::yRight);


        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        verticalLayout->addWidget(titleEdit);
        verticalLayout->addWidget(footerEdit);
        verticalLayout->addWidget(canvasBackgroundEdit);
        verticalLayout->addWidget(borderRadiusEdit);
        verticalLayout->addWidget(axisSet.at(xBottom).group);
        verticalLayout->addWidget(axisSet.at(yLeft).group);
        verticalLayout->addWidget(axisSet.at(xTop).group);
        verticalLayout->addWidget(axisSet.at(yRight).group);
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
        axisSet.at(xBottom).group->setTitle(QApplication::translate("SAChartNormalSetWidget", "X Bottom", 0));
        setAxisSetText(axisSet.at(xBottom));
        axisSet.at(yLeft).group->setTitle(QApplication::translate("SAChartNormalSetWidget", "Y Left", 0));
        setAxisSetText(axisSet.at(yLeft));
        axisSet.at(xTop).group->setTitle(QApplication::translate("SAChartNormalSetWidget", "X Top", 0));
        setAxisSetText(axisSet.at(xTop));
        axisSet.at(yRight).group->setTitle(QApplication::translate("SAChartNormalSetWidget", "Y Right", 0));
        setAxisSetText(axisSet.at(yRight));
    } // retranslateUi

    void setChart(QwtPlot *c,SAChartNormalSetWidget* par)
    {
        this->chart = c;
        if(nullptr == c)
        {
            return;
        }
        par->connect(c,&QObject::destroyed,par,&SAChartNormalSetWidget::onChartDestroy);
        QwtScaleWidget* aw = nullptr;

        aw = this->chart->axisWidget(QwtPlot::xBottom);
        if(aw)
            par->connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedXBottom);

        aw = this->chart->axisWidget(QwtPlot::xTop);
        if(aw)
            connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedXTop);

        aw = this->chart->axisWidget(QwtPlot::yLeft);
        if(aw)
            connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedYLeft);

        aw = this->chart->axisWidget(QwtPlot::yRight);
        if(aw)
            connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedYRight);

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
        updateChartAxisValue(QwtPlot::xBottom);
        updateChartAxisValue(QwtPlot::xTop);
        updateChartAxisValue(QwtPlot::yLeft);
        updateChartAxisValue(QwtPlot::yRight);
    }


    void updateChartAxisValue(int axisID)
    {
        if(nullptr == this->chart)
        {
            return;
        }
        int axisIndex = xBottom;
        switch(axisID)
        {
            case QwtPlot::xBottom:axisIndex = xBottom;break;
            case QwtPlot::xTop:axisIndex = xTop;break;
            case QwtPlot::yLeft:axisIndex = yLeft;break;
            case QwtPlot::yRight:axisIndex = yRight;break;
            default:axisIndex = xBottom;
        }
        AxisPropertyItems& axSet = axisSet.at(axisIndex);
        bool b = this->chart->axisEnabled(axisID);

        axSet.group->setChecked(b);
        if(b)
        {
            axSet.title->setEditText(this->chart->axisTitle(axisID).text());
            axSet.font->setCurrentFont(this->chart->axisFont(axisID));
            QwtInterval inv = this->chart->axisInterval(axisID);
            axSet.scaleMin->setValue(inv.minValue());
            axSet.scaleMax->setValue(inv.maxValue());
            QwtScaleWidget * ax = this->chart->axisWidget(axisID);
            if(ax)
            {
                QwtScaleDraw * sd = ax->scaleDraw();
                if(sd)
                {
                    axSet.labelRotation->setValue(sd->labelRotation());
                    axSet.labelAligment->setAlignment(sd->labelAlignment());
                }
                axSet.margin->setValue(ax->margin());
                axSet.spacing->setValue(ax->spacing());
            }
        }
    }
private:
    void setAxisSetText(AxisPropertyItems& axisSets)
    {
        axisSets.title->setText(QApplication::translate("SAChartNormalSetWidget", "Title", 0));
        axisSets.font->setText(QApplication::translate("SAChartNormalSetWidget", "Font", 0));
        axisSets.labelAligment->setText(QApplication::translate("SAChartNormalSetWidget", "Text Aligment", 0));
        axisSets.labelRotation->setText(QApplication::translate("SAChartNormalSetWidget", "Label Rotation", 0));
        axisSets.scaleMin->setText(QApplication::translate("SAChartNormalSetWidget", "Scale Min", 0));
        axisSets.scaleMax->setText(QApplication::translate("SAChartNormalSetWidget", "Scale Max", 0));
        axisSets.margin->setText(QApplication::translate("SAChartNormalSetWidget", "Margin", 0));
        axisSets.spacing->setText(QApplication::translate("SAChartNormalSetWidget", "spacing", 0));
    }

    void setupAxisSet(SAChartNormalSetWidget* par
                      ,AxisPropertyItems& axisSets
                      ,int axisID)
    {
        axisSets.group = new SAVGroupBoxPropertyItem(par);
        //scale min
        axisSets.scaleMin = new SADoubleSpinBoxPropertyItem(axisSets.group);
        //scale max
        axisSets.scaleMax = new SADoubleSpinBoxPropertyItem(axisSets.group);
        //Axis title
        axisSets.title = new SALineEditPropertyItem(axisSets.group);
        //Axis Font
        axisSets.font = new SAFontComboBoxPropertyItem(axisSets.group);
        //label Aligment
        axisSets.labelAligment = new SAAligmentPropertyItem(axisSets.group);
        //scale min
        axisSets.labelRotation = new SADoubleSpinBoxPropertyItem(axisSets.group);
        //margin
        axisSets.margin = new SASpinBoxPropertyItem(axisSets.group);
        axisSets.margin->setMinimum(0);
        //spacing
        axisSets.spacing = new SASpinBoxPropertyItem(axisSets.group);

        axisSets.group->addWidget(axisSets.scaleMin);
        axisSets.group->addWidget(axisSets.scaleMax);
        axisSets.group->addWidget(axisSets.title);
        axisSets.group->addWidget(axisSets.font);
        axisSets.group->addWidget(axisSets.labelAligment);
        axisSets.group->addWidget(axisSets.labelRotation);

        axisSets.group->addWidget(axisSets.margin);
        axisSets.group->addWidget(axisSets.spacing);

        SAVGroupBoxPropertyItem* gitem = axisSets.group;
        par->connect(axisSets.group,&SAVGroupBoxPropertyItem::clicked
                  ,par,[axisID,this,gitem,par](bool b){
            par->setMinimumSize(par->minimumSizeHint());
            SAChartNormalSetWidget::setAxisEnable(this->chart,axisID,b);
        });
        par->connect(axisSets.title,&SALineEditPropertyItem::textChanged
                  ,par,[axisID,this](const QString& text){SAChartNormalSetWidget::setAxisTitle(this->chart,axisID,text);});
        par->connect(axisSets.font,&SAFontComboBoxPropertyItem::currentFontChanged
                  ,par,[axisID,this](const QFont &font){SAChartNormalSetWidget::setAxisFont(this->chart,axisID,font);});
        par->connect(axisSets.labelAligment,&SAAligmentPropertyItem::stateChanged
                  ,par,[axisID,this](Qt::Alignment v){SAChartNormalSetWidget::setAxisLabelAlignment(this->chart,axisID,v);});
        par->connect(axisSets.labelRotation,&SADoubleSpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](double v){SAChartNormalSetWidget::setAxisLabelRotation(this->chart,axisID,v);});
        par->connect(axisSets.scaleMin,&SADoubleSpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](double v){SAChartNormalSetWidget::setAxisScaleMin(this->chart,axisID,v);});
        par->connect(axisSets.scaleMax,&SADoubleSpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](double v){SAChartNormalSetWidget::setAxisScaleMax(this->chart,axisID,v);});
        par->connect(axisSets.margin,&SASpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](int v){SAChartNormalSetWidget::setAxisMargin(this->chart,axisID,v);});
        par->connect(axisSets.spacing,&SASpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](int v){SAChartNormalSetWidget::setAxisSpacing(this->chart,axisID,v);});

    }
};

SAChartNormalSetWidget::SAChartNormalSetWidget(QWidget *par):QWidget(par)
  ,ui(new SAChartNormalSetWidget::UI)
{
    ui->setupUI(this);
    setMinimumSize(minimumSizeHint());
}

SAChartNormalSetWidget::~SAChartNormalSetWidget()
{
    delete ui;
}

void SAChartNormalSetWidget::setChart(QwtPlot *chart)
{
    ui->setChart(chart,this);
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

void SAChartNormalSetWidget::onChartDestroy(QObject *o)
{
    ui->setChart(nullptr,this);
}

void SAChartNormalSetWidget::onScaleDivChangedXBottom()
{
    ui->updateChartAxisValue(QwtPlot::xBottom);
}

void SAChartNormalSetWidget::onScaleDivChangedXTop()
{
    ui->updateChartAxisValue(QwtPlot::xTop);
}

void SAChartNormalSetWidget::onScaleDivChangedYLeft()
{
    ui->updateChartAxisValue(QwtPlot::yLeft);
}

void SAChartNormalSetWidget::onScaleDivChangedYRight()
{
    ui->updateChartAxisValue(QwtPlot::yRight);
}



void SAChartNormalSetWidget::setAxisEnable(QwtPlot *chart, int axisID, bool b)
{
    if(chart)
    {
        chart->enableAxis(axisID,b);
        if(!chart->axisAutoScale(axisID))
        {
            chart->setAxisAutoScale(axisID);
        }
        QwtScaleWidget * ax = chart->axisWidget(axisID);
        if(nullptr == ax)
        {
            return;
        }
        //ax->setScaleDiv(chart->d);
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

void SAChartNormalSetWidget::setAxisLabelAlignment(QwtPlot *chart, int axisID, Qt::Alignment v)
{
    if(nullptr == chart)
    {
        return;
    }
    QwtScaleWidget * ax = chart->axisWidget(axisID);
    if(ax)
    {
        ax->setLabelAlignment(v);
    }
}



