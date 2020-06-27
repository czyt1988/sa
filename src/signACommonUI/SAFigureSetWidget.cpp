#include "SAFigureSetWidget.h"
#include "SAFigureWindow.h"
#include "SAChartSetWidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QStackedWidget>
#include "SAChart2D.h"

class SAFigureSetWidget::UI
{
public:
    QVBoxLayout *verticalLayout;
    SAChartSetWidget* chartSetWidget;
    QMap<QwtPlot*,SAChartSetWidget*> plotMapToWidget;

    void setupUi(SAFigureSetWidget *p)
    {
        if (p->objectName().isEmpty())
            p->setObjectName(QStringLiteral("SAFiugreSetWidget"));
        p->resize(308, 464);

        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        chartSetWidget = new SAChartSetWidget(p);
        chartSetWidget->setObjectName(QStringLiteral("ChartSetWidget"));
        chartSetWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        verticalLayout->addWidget(chartSetWidget);
        p->setLayout(verticalLayout);
        p->connect(chartSetWidget,&SAChartSetWidget::chartTitleChanged
                ,p,&SAFigureSetWidget::chartTitleChanged);
        retranslateUi(p);

    } // setupUi

    void retranslateUi(QWidget *SAFiugreSetWidget)
    {
        SAFiugreSetWidget->setWindowTitle(QApplication::translate("SAFiugreSetWidget", "Fiugre Set", 0));
    } // retranslateUi
};

SAFigureSetWidget::SAFigureSetWidget(QWidget *parent) :
    QWidget(parent)
    ,ui(new SAFigureSetWidget::UI)
    ,m_fig(nullptr)
{
    ui->setupUi(this);
}

SAFigureSetWidget::~SAFigureSetWidget()
{
    delete ui;
}

void SAFigureSetWidget::setFigureWidget(SAFigureWindow *fig)
{
    if(m_fig)
    {
        disconnect(fig,&QObject::destroyed,this,&SAFigureSetWidget::onFigutrDestroy);
        disconnect(fig,&SAFigureWindow::currentWidgetChanged,this,&SAFigureSetWidget::onCurrentFigureWidgetChanged);
    }
    if(nullptr == fig)
    {
        clear();
        return;
    }
    if(nullptr != fig && m_fig != fig)
    {
        connect(fig,&QObject::destroyed,this,&SAFigureSetWidget::onFigutrDestroy);
        connect(fig,&SAFigureWindow::currentWidgetChanged,this,&SAFigureSetWidget::onCurrentFigureWidgetChanged);
    }
    m_fig = nullptr;
    m_fig = fig;
    setCurrentChart(fig->current2DPlot());
}

void SAFigureSetWidget::clear()
{
    ui->chartSetWidget->setChart(nullptr);
}

void SAFigureSetWidget::updateData()
{
    ui->chartSetWidget->updateAll();
}

void SAFigureSetWidget::updatePlotItemsSet()
{
    ui->chartSetWidget->updatePlotItemsSet();
}

void SAFigureSetWidget::updateAxesSet()
{
    ui->chartSetWidget->updateAxesSet();
}

void SAFigureSetWidget::updateNormalSet()
{
    ui->chartSetWidget->updateNormalSet();
}




void SAFigureSetWidget::onFigutrDestroy(QObject *obj)
{
    Q_UNUSED(obj);
    m_fig = nullptr;
}

///
/// \brief 当前绘图窗口选中的窗体对象发生改变
/// \param w
///
void SAFigureSetWidget::onCurrentFigureWidgetChanged(QWidget *w)
{
    if(SAChart2D* chart = qobject_cast<SAChart2D*>(w))
    {
        setCurrentChart(chart);
    }
}

///
/// \brief 设置图表
/// \param chart
///
void SAFigureSetWidget::setCurrentChart(SAChart2D *chart)
{
    ui->chartSetWidget->setChart(chart);
}


