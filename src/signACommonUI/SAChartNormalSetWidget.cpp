#include "SAChartNormalSetWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include "SAChart2D.h"
#include "SALineEditPropertyItem.h"
#include "SAColorSetPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include "SAVGroupBoxPropertyItem.h"
#include "SAFontComboBoxPropertyItem.h"
#include "SAAligmentPropertyItem.h"
#include "SACheckBoxPropertyItem.h"
#include "SAComboBoxPropertyItem.h"
#include "SACheckBoxPropertyItem.h"
#include <QMap>
#include <QHash>
#include <functional>
#include <array>
#include "SAChart.h"
#include "qwt_plot_canvas.h"
#include "qwt_scale_widget.h"
#include "qwt_scale_draw.h"
#include "qwt_date_scale_draw.h"
class AxisPropertyItems
{
public:
    SAVGroupBoxPropertyItem* group;
    SACheckBoxPropertyItem* enableAxis;
    SALineEditPropertyItem* title;
    SAFontComboBoxPropertyItem* font;
    SADoubleSpinBoxPropertyItem* labelRotation;
    SADoubleSpinBoxPropertyItem* scaleMin;
    SADoubleSpinBoxPropertyItem* scaleMax;
    SASpinBoxPropertyItem* margin;
    SASpinBoxPropertyItem* spacing;
    SAAligmentPropertyItem* labelAligment;
    //
    SAVGroupBoxPropertyItem* timeScaleGroup;
    SACheckBoxPropertyItem* enabletimeScale;
    SAComboBoxPropertyItem* timeScaleFormat;
    //SACheckBoxPropertyItem* isFollowOtherAxis;///< 是否跟随对应的坐标轴
    void setAxisVisible(bool b)
    {
        title->setVisible(b);
        font->setVisible(b);
        labelRotation->setVisible(b);
        scaleMin->setVisible(b);
        scaleMax->setVisible(b);
        margin->setVisible(b);
        spacing->setVisible(b);
        labelAligment->setVisible(b);
        timeScaleGroup->setVisible(b);
    }
    void setAxisTimeScaleMode(bool enable)
    {
        timeScaleFormat->setEnabled(enable);
    }
};

class SAChartNormalSetWidget::UI
{
public:

    SAChart2D* chart;
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
        par->setMinimumSize(150,250);
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

    void setChart(SAChart2D *c,SAChartNormalSetWidget* par)
    {
        SAChart2D* oldChart = this->chart;
        if(nullptr != oldChart)
        {
            disconnect(oldChart,par);
        }
        this->chart = c;
        if(nullptr == c)
        {
            setVisible(false,c,par);
            return;
        }
        setVisible(true,c,par);
        connect(c,par);
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

    void connect(QwtPlot *c,SAChartNormalSetWidget* par)
    {
        par->connect(c,&QObject::destroyed,par,&SAChartNormalSetWidget::onChartDestroy);
        QwtScaleWidget* aw = nullptr;

        aw = this->chart->axisWidget(QwtPlot::xBottom);
        if(aw)
        {
            par->connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedXBottom);
        }

        aw = this->chart->axisWidget(QwtPlot::xTop);
        if(aw)
        {
            par->connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedXTop);
        }

        aw = this->chart->axisWidget(QwtPlot::yLeft);
        if(aw)
        {
            par->connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedYLeft);
        }

        aw = this->chart->axisWidget(QwtPlot::yRight);
        if(aw)
        {
            par->connect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedYRight);
        }
    }

    void disconnect(QwtPlot *c,SAChartNormalSetWidget* par)
    {
        par->disconnect(c,&QObject::destroyed,par,&SAChartNormalSetWidget::onChartDestroy);
        QwtScaleWidget* aw = nullptr;

        aw = this->chart->axisWidget(QwtPlot::xBottom);
        if(aw)
        {
            par->disconnect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedXBottom);
        }

        aw = this->chart->axisWidget(QwtPlot::xTop);
        if(aw)
        {
            par->disconnect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedXTop);
        }

        aw = this->chart->axisWidget(QwtPlot::yLeft);
        if(aw)
        {
            par->disconnect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedYLeft);
        }

        aw = this->chart->axisWidget(QwtPlot::yRight);
        if(aw)
        {
            par->disconnect(aw,&QwtScaleWidget::scaleDivChanged,par,&SAChartNormalSetWidget::onScaleDivChangedYRight);
        }
    }
    static AxisIndex qwtAxisId2AxisIndex(int axisID)
    {
        switch(axisID)
        {
            case QwtPlot::xBottom:return xBottom;
            case QwtPlot::xTop:return xTop;
            case QwtPlot::yLeft:return yLeft;
            case QwtPlot::yRight:return yRight;
            default:return xBottom;
        }
        return xBottom;
    }

    void updateChartAxisValue(int axisID)
    {
        if(nullptr == this->chart)
        {
            return;
        }
        int axisIndex = qwtAxisId2AxisIndex(axisID);
        AxisPropertyItems& axSet = axisSet.at(axisIndex);
        bool b = this->chart->axisEnabled(axisID);

        axSet.enableAxis->setCheckState(b?Qt::Checked:Qt::Unchecked);
        axSet.setAxisVisible(b);
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
                QwtDateScaleDraw* dsd = dynamic_cast<QwtDateScaleDraw*>(sd);
                axSet.enabletimeScale->setCheckState((nullptr != dsd) ? Qt::Checked : Qt::Unchecked);
                axSet.setAxisTimeScaleMode(nullptr != dsd);
                if(dsd)
                {
                    axSet.timeScaleFormat->setText(dsd->dateFormat(QwtDate::Second));
                    axSet.timeScaleGroup->setCollapsed(false);
                }
            }
        }
    }
private:
    void setVisible(bool b,SAChart2D *c,SAChartNormalSetWidget* par)
    {
        Q_UNUSED(c);
        Q_UNUSED(par);
        for(size_t i=0;i<axisSet.size();++i)
        {
            axisSet.at(i).group->setVisible(b);
        }
    }

    void setAxisSetText(AxisPropertyItems& axisSets)
    {
        axisSets.enableAxis->setText(QApplication::translate("SAChartNormalSetWidget", "Enable", 0));
        axisSets.title->setText(QApplication::translate("SAChartNormalSetWidget", "Title", 0));
        axisSets.font->setText(QApplication::translate("SAChartNormalSetWidget", "Font", 0));
        axisSets.labelAligment->setText(QApplication::translate("SAChartNormalSetWidget", "Text Aligment", 0));
        axisSets.labelRotation->setText(QApplication::translate("SAChartNormalSetWidget", "Label Rotation", 0));
        axisSets.scaleMin->setText(QApplication::translate("SAChartNormalSetWidget", "Scale Min", 0));
        axisSets.scaleMax->setText(QApplication::translate("SAChartNormalSetWidget", "Scale Max", 0));
        axisSets.margin->setText(QApplication::translate("SAChartNormalSetWidget", "Margin", 0));
        axisSets.spacing->setText(QApplication::translate("SAChartNormalSetWidget", "spacing", 0));
        axisSets.timeScaleGroup->setTitle(QApplication::translate("SAChartNormalSetWidget", "Time Scale", 0));
        axisSets.enabletimeScale->setText(QApplication::translate("SAChartNormalSetWidget", "Enable Time Scale", 0));
        axisSets.timeScaleFormat->setText(QApplication::translate("SAChartNormalSetWidget", "Time Format", 0));

    }

    void setupAxisSet(SAChartNormalSetWidget* par
                      ,AxisPropertyItems& axisSets
                      ,int axisID)
    {
        axisSets.group = new SAVGroupBoxPropertyItem(par);
        //enable Axis
        axisSets.enableAxis = new SACheckBoxPropertyItem(axisSets.group);
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
        //
        axisSets.timeScaleGroup = new SAVGroupBoxPropertyItem(axisSets.group);
        //
        axisSets.enabletimeScale = new SACheckBoxPropertyItem(axisSets.timeScaleGroup);
        axisSets.timeScaleGroup->addWidget(axisSets.enabletimeScale);
        axisSets.timeScaleFormat = new SAComboBoxPropertyItem(axisSets.timeScaleGroup);
        axisSets.timeScaleFormat->setEditable(true);
        axisSets.timeScaleFormat->addItems(SA2DGraph::axisDateScaleTypes2StringList());
        axisSets.timeScaleFormat->setCurrentText("yyyy-M-d h:m:s");
        axisSets.timeScaleGroup->addWidget(axisSets.timeScaleFormat);


        axisSets.group->addWidget(axisSets.enableAxis);
        axisSets.group->addWidget(axisSets.scaleMin);
        axisSets.group->addWidget(axisSets.scaleMax);
        axisSets.group->addWidget(axisSets.title);
        axisSets.group->addWidget(axisSets.font);
        axisSets.group->addWidget(axisSets.labelAligment);
        axisSets.group->addWidget(axisSets.labelRotation);

        axisSets.group->addWidget(axisSets.margin);
        axisSets.group->addWidget(axisSets.spacing);
        axisSets.group->addWidget(axisSets.timeScaleGroup);


        par->connect(axisSets.enableAxis,&SACheckBoxPropertyItem::stateChanged
                  ,par,[axisID,this](int state){
            bool check = Qt::Checked == state;
            SAChart::setAxisEnable(this->chart,axisID,check);
            this->axisSet.at(qwtAxisId2AxisIndex(axisID)).setAxisVisible(check);
        });
        par->connect(axisSets.title,&SALineEditPropertyItem::textChanged
                  ,par,[axisID,this](const QString& text){SAChart::setAxisTitle(this->chart,axisID,text);});
        par->connect(axisSets.font,&SAFontComboBoxPropertyItem::currentFontChanged
                  ,par,[axisID,this](const QFont &font){SAChart::setAxisFont(this->chart,axisID,font);});
        par->connect(axisSets.labelAligment,&SAAligmentPropertyItem::stateChanged
                  ,par,[axisID,this](Qt::Alignment v){SAChart::setAxisLabelAlignment(this->chart,axisID,v);});
        par->connect(axisSets.labelRotation,&SADoubleSpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](double v){SAChart::setAxisLabelRotation(this->chart,axisID,v);});
        par->connect(axisSets.scaleMin,&SADoubleSpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](double v){SAChart::setAxisScaleMin(this->chart,axisID,v);});
        par->connect(axisSets.scaleMax,&SADoubleSpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](double v){SAChart::setAxisScaleMax(this->chart,axisID,v);});
        par->connect(axisSets.margin,&SASpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](int v){SAChart::setAxisMargin(this->chart,axisID,v);});
        par->connect(axisSets.spacing,&SASpinBoxPropertyItem::valueChanged
                  ,par,[axisID,this](int v){SAChart::setAxisSpacing(this->chart,axisID,v);});
        par->connect(axisSets.enabletimeScale,&SACheckBoxPropertyItem::stateChanged
                  ,par,[axisID,this](int state){
            bool check = (Qt::Checked == state);
            AxisPropertyItems& ax = this->axisSet.at(qwtAxisId2AxisIndex(axisID));
            ax.timeScaleFormat->setEnabled(check);
            if(check)
            {
                QString format = ax.timeScaleFormat->currentText();
                SAChart::setAxisDateTimeScale(this->chart,axisID,format);
            }
            else
            {
                SAChart::setAxisNormalScale(this->chart,axisID);
            }
        });
        par->connect(axisSets.timeScaleFormat,&SAComboBoxPropertyItem::currentTextChanged
                     ,par,[axisID,this](const QString& str){
            SAChart::setAxisDateTimeScale(this->chart,axisID,str);

        });
    }

};

SAChartNormalSetWidget::SAChartNormalSetWidget(QWidget *par):QWidget(par)
  ,ui(new SAChartNormalSetWidget::UI)
{
    ui->setupUI(this);
    setMinimumSize(minimumSizeHint());
    //resize(sizeHint());
}

SAChartNormalSetWidget::~SAChartNormalSetWidget()
{
    delete ui;
}

void SAChartNormalSetWidget::setChart(SAChart2D *chart)
{
    ui->setChart(chart,this);
}

void SAChartNormalSetWidget::retranslateUi()
{
    ui->retranslateUi(this);
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
        ui->chart->replot();//update无法刷新界面
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
    Q_UNUSED(o);
    ui->chart = nullptr;
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










