#include "SAChartNormalSetWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include "SAChart2D.h"
#include "SALineEditPropertyItem.h"
#include "SAColorSetPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include "SAVCollapsibleGroupBox.h"
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



class SAChartNormalSetWidget::UI
{
public:
    SAChartNormalSetWidget* parentClass;
    SAChart2D* chart;
    QVBoxLayout *verticalLayout;
    SALineEditPropertyItem* titleEdit;
    SALineEditPropertyItem* footerEdit;
    SAColorSetPropertyItem* canvasBackgroundEdit;
    SADoubleSpinBoxPropertyItem* borderRadiusEdit;
    UI():parentClass(nullptr)
      ,chart(nullptr)
      ,verticalLayout(nullptr)
      ,titleEdit(nullptr)
      ,footerEdit(nullptr)
      ,canvasBackgroundEdit(nullptr)
      ,borderRadiusEdit(nullptr)
    {

    }

    void setupUI(SAChartNormalSetWidget* par)
    {
        parentClass = par;
        par->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        chart = nullptr;
        verticalLayout = new QVBoxLayout(par);
        par->setObjectName(QStringLiteral("SAChartNormalSetWidget"));
        //Title
        titleEdit = new SALineEditPropertyItem();
        titleEdit->setObjectName("titleEdit");
        par->connect(titleEdit,&SALineEditPropertyItem::textChanged
                     ,par,&SAChartNormalSetWidget::onTitleTextChanged);
        //Footer
        footerEdit = new SALineEditPropertyItem();
        footerEdit->setObjectName("footerEdit");
        par->connect(footerEdit,&SALineEditPropertyItem::textChanged
                     ,par,&SAChartNormalSetWidget::onFooterTextChanged);
        //Canvas Background
        canvasBackgroundEdit = new SAColorSetPropertyItem();
        par->connect(canvasBackgroundEdit,&SAColorSetPropertyItem::colorChanged
                     ,par,&SAChartNormalSetWidget::onCanvasBackgroundColorChanged);
        //borderRadius
        borderRadiusEdit = new SADoubleSpinBoxPropertyItem();
        par->connect(borderRadiusEdit,&SADoubleSpinBoxPropertyItem::valueChanged
                     ,par,&SAChartNormalSetWidget::onBorderRadiusChanged);

        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        verticalLayout->addWidget(titleEdit);
        verticalLayout->addWidget(footerEdit);
        verticalLayout->addWidget(canvasBackgroundEdit);
        verticalLayout->addWidget(borderRadiusEdit);
        verticalLayout->addStretch();

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

    void setChart(SAChart2D *c)
    {
        this->chart = c;
        if(nullptr == c)
        {
            titleEdit->setEditText("");
            footerEdit->setEditText("");
            borderRadiusEdit->setValue(0);
            return;
        }
        titleEdit->setEditText(c->title().text());
        footerEdit->setEditText(c->footer().text());
        canvasBackgroundEdit->setCurrentColor(c->canvasBackground().color());
        QWidget* w = c->canvas();
        QwtPlotCanvas* canvas = qobject_cast<QwtPlotCanvas*>(w);
        if(canvas)
        {
            borderRadiusEdit->setValue(canvas->borderRadius());
        }
    }

};

//-----------------------------------------------------

SAChartNormalSetWidget::SAChartNormalSetWidget(QWidget *par):QWidget(par)
  ,ui(new SAChartNormalSetWidget::UI)
{
    ui->setupUI(this);
    setEnabled(false);
}

SAChartNormalSetWidget::~SAChartNormalSetWidget()
{
    delete ui;
}

void SAChartNormalSetWidget::setChart(SAChart2D *chart)
{
    ui->setChart(chart);
    setEnabled(nullptr != chart);
}

void SAChartNormalSetWidget::retranslateUi()
{
    ui->retranslateUi(this);
}

void SAChartNormalSetWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
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














