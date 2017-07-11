#include "SAChartNormalSetWidget.h"
#include <QApplication>
#include <QVBoxLayout>

#include "qwt_plot.h"
#include "SALineEditPropertyItem.h"
#include "SAColorSetPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include <QMap>
#include <functional>
#include "qwt_plot_canvas.h"
class SAChartNormalSetWidget::UI
{
public:

    QwtPlot* chart;
    QVBoxLayout *verticalLayout;
    SALineEditPropertyItem* titleEdit;
    SALineEditPropertyItem* footerEdit;
    SAColorSetPropertyItem* canvasBackgroundEdit;
    SADoubleSpinBoxPropertyItem* borderRadiusEdit;
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

        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        verticalLayout->addWidget(titleEdit);
        verticalLayout->addWidget(footerEdit);
        verticalLayout->addWidget(canvasBackgroundEdit);
        verticalLayout->addWidget(borderRadiusEdit);
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

    } // retranslateUi
};

SAChartNormalSetWidget::SAChartNormalSetWidget(QWidget *par):QWidget(par)
  ,ui(new SAChartNormalSetWidget::UI)
{
    ui->setupUI(this);
}

void SAChartNormalSetWidget::setChart(QwtPlot *chart)
{
    ui->chart = chart;
    ui->titleEdit->setEditText(chart->title().text());
    ui->footerEdit->setEditText(chart->footer().text());
    ui->canvasBackgroundEdit->setCurrentColor(chart->canvasBackground().color());
    QWidget* w = ui->chart->canvas();
    QwtPlotCanvas* canvas = qobject_cast<QwtPlotCanvas*>(w);
    if(canvas)
    {
        ui->borderRadiusEdit->setValue(canvas->borderRadius());
    }
}

void SAChartNormalSetWidget::onTitleTextChanged(const QString &text)
{
    if(ui->chart)
    {
        ui->chart->setTitle(text);
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



