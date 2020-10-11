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
#include "SAPlotCanvas.h"


class SAChartNormalSetWidget::UI
{
public:
    SAChartNormalSetWidget *parentClass;
    SAChart2D *chart;
    QVBoxLayout *verticalLayout;
    SALineEditPropertyItem *titleEdit;
    SALineEditPropertyItem *footerEdit;
    SAColorSetPropertyItem *canvasBackgroundEdit;
    SAColorSetPropertyItem *canvasBorderColorEdit;
    SASpinBoxPropertyItem *borderWidthEdit;
    QBrush canvasBackBrush;
    int canvasBorderWidth;
    QColor canvasBorderColor;

    UI() : parentClass(nullptr)
        , chart(nullptr)
        , verticalLayout(nullptr)
        , titleEdit(nullptr)
        , footerEdit(nullptr)
        , canvasBackgroundEdit(nullptr)
        , borderWidthEdit(nullptr)
        , canvasBackBrush(Qt::white)
        , canvasBorderWidth(1)
        , canvasBorderColor(Qt::black)
    {
    }


    void setupUI(SAChartNormalSetWidget *par)
    {
        parentClass = par;
        par->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        chart = nullptr;
        verticalLayout = new QVBoxLayout(par);
        par->setObjectName(QStringLiteral("SAChartNormalSetWidget"));
        //Title
        titleEdit = new SALineEditPropertyItem();
        titleEdit->setObjectName("titleEdit");
        par->connect(titleEdit, &SALineEditPropertyItem::textChanged
            , par, &SAChartNormalSetWidget::onTitleTextChanged);
        //Footer
        footerEdit = new SALineEditPropertyItem();
        footerEdit->setObjectName("footerEdit");
        par->connect(footerEdit, &SALineEditPropertyItem::textChanged
            , par, &SAChartNormalSetWidget::onFooterTextChanged);
        //Canvas Background
        canvasBackgroundEdit = new SAColorSetPropertyItem();
        par->connect(canvasBackgroundEdit, &SAColorSetPropertyItem::colorChanged
            , par, &SAChartNormalSetWidget::onCanvasBackgroundColorChanged);
        //canvas Border Color Edit
        canvasBorderColorEdit = new SAColorSetPropertyItem();
        par->connect(canvasBorderColorEdit, &SAColorSetPropertyItem::colorChanged
            , par, &SAChartNormalSetWidget::onCanvasBorderColorChanged);
        //border width
        borderWidthEdit = new SASpinBoxPropertyItem();
        par->connect(borderWidthEdit, &SASpinBoxPropertyItem::valueChanged
            , par, &SAChartNormalSetWidget::onBorderWidthChanged);

        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        verticalLayout->addWidget(titleEdit);
        verticalLayout->addWidget(footerEdit);
        verticalLayout->addWidget(canvasBackgroundEdit);
        verticalLayout->addWidget(canvasBorderColorEdit);
        verticalLayout->addWidget(borderWidthEdit);
        verticalLayout->addStretch();

        retranslateUi(par);
    }


    void retranslateUi(SAChartNormalSetWidget *w)
    {
        w->setWindowTitle(QApplication::translate("SAChartNormalSetWidget", "Normal Set", 0));
        titleEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Title", 0));
        footerEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Footer", 0));
        canvasBackgroundEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Canvas Background", 0));
        canvasBorderColorEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Border Color", 0));
        borderWidthEdit->setText(QApplication::translate("SAChartNormalSetWidget", "Border Width", 0));
    } // retranslateUi


    void setChart(SAChart2D *c)
    {
        if (c != this->chart) {
            this->chart = c;
            updateData();
        }
    }


    void updateData()
    {
        if (nullptr == this->chart) {
            titleEdit->setEditText("");
            footerEdit->setEditText("");
            borderWidthEdit->setValue(0);
            canvasBackgroundEdit->setCurrentColor(QColor());
            return;
        }
        titleEdit->setEditText(this->chart->title().text());
        footerEdit->setEditText(this->chart->footer().text());
        SAPlotCanvas *canvas = qobject_cast<SAPlotCanvas *>(this->chart->canvas());

        if (canvas) {
            canvasBackgroundEdit->setCurrentColor(canvas->getCanvaBackBrush().color());
            canvasBorderColorEdit->setCurrentColor(canvas->getCanvasBorderColor());
            borderWidthEdit->setValue(canvas->lineWidth());
        }
    }
};

//-----------------------------------------------------

SAChartNormalSetWidget::SAChartNormalSetWidget(QWidget *par) : QWidget(par)
    , ui(new SAChartNormalSetWidget::UI)
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


void SAChartNormalSetWidget::updateData()
{
    ui->updateData();
}


void SAChartNormalSetWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
}


void SAChartNormalSetWidget::onTitleTextChanged(const QString& text)
{
    if (ui->chart) {
        ui->chart->setTitle(text);
        emit chartTitleChanged(ui->chart, text);
    }
}


void SAChartNormalSetWidget::onFooterTextChanged(const QString& text)
{
    if (ui->chart) {
        ui->chart->setFooter(text);
    }
}


void SAChartNormalSetWidget::onCanvasBackgroundColorChanged(const QColor& clr)
{
    if (ui->chart) {
        SAPlotCanvas *c = qobject_cast<SAPlotCanvas *>(ui->chart->canvas());

        if (nullptr == c) {
            return;
        }
        if (c->testAttribute(Qt::WA_StyledBackground)) {
            c->setCanvaBackBrush(clr);
        }else {
            ui->chart->setCanvasBackground(clr);
        }
        ui->chart->replot();
    }
}


void SAChartNormalSetWidget::onCanvasBorderColorChanged(const QColor& clr)
{
    if (ui->chart) {
        SAPlotCanvas *c = qobject_cast<SAPlotCanvas *>(ui->chart->canvas());

        if (nullptr == c) {
            return;
        }
        if (c->testAttribute(Qt::WA_StyledBackground)) {
            c->setCanvasBorderColor(clr);
            ui->chart->replot();
        }
    }
}


void SAChartNormalSetWidget::onBorderWidthChanged(int v)
{
    if (ui->chart) {
        ui->canvasBorderColorEdit->setDisabled(v <= 0);
        SAPlotCanvas *c = qobject_cast<SAPlotCanvas *>(ui->chart->canvas());

        if (nullptr == c) {
            return;
        }
        c->setLineWidth(v);
        if (c->testAttribute(Qt::WA_StyledBackground)) {
            c->useQss();
        }
        ui->chart->replot();
    }
}
