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
#if SAFiugreSetWidget_USE_COMBOX
    QComboBox* chartSelectComboBox;
#endif
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
#if SAFiugreSetWidget_USE_COMBOX
        chartSelectComboBox = new QComboBox(p);
        chartSelectComboBox->setObjectName(QStringLiteral("SAFiugreSetWidgetChartSelectComboBox"));
        verticalLayout->addWidget(chartSelectComboBox);
        p->connect(chartSelectComboBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                   ,p,&SAFigureSetWidget::onComboxChanged);
#endif
        chartSetWidget = new SAChartSetWidget(p);
        chartSetWidget->setObjectName(QStringLiteral("ChartSetWidget"));
        chartSetWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        verticalLayout->addWidget(chartSetWidget);
        p->setLayout(verticalLayout);
        p->connect(chartSetWidget,&SAChartSetWidget::chartTitleChanged
                ,p,&SAFigureSetWidget::onChartTitleChanged);
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
#if SAFiugreSetWidget_USE_COMBOX
    if(m_fig)
    {
        disconnect(fig,&QObject::destroyed,this,&SAFigureSetWidget::onFigutrDestroy);
    }
    if(nullptr == fig)
    {
        clear();
        return;
    }
    if(nullptr != fig && m_fig != fig)
    {
        connect(fig,&QObject::destroyed,this,&SAFigureSetWidget::onFigutrDestroy);
    }
    m_fig = nullptr;
    ui->chartSelectComboBox->clear();
    QList<SAChart2D*> plots = fig->get2DPlots();
    for(int i=0;i<plots.size();++i)
    {
        QString title = plots[i]->title().text();
        if(title.isEmpty())
        {
            title = tr("chart %1").arg(i+1);
        }
        //combox 插入条目
        ui->chartSelectComboBox->addItem(title,QVariant::fromValue((quintptr)plots[i]));
        //关联槽
        connect(plots[i],&QObject::destroyed,this,&SAFigureSetWidget::onPlotDestroy);
    }
    if(!plots.isEmpty())
    {
        ui->chartSetWidget->setChart(plots.first());
    }

    m_fig = fig;
#else
    if(m_fig)
    {
        disconnect(fig,&QObject::destroyed,this,&SAFigureSetWidget::onFigutrDestroy);
    }
    if(nullptr == fig)
    {
        clear();
        return;
    }
    if(nullptr != fig && m_fig != fig)
    {
        connect(fig,&QObject::destroyed,this,&SAFigureSetWidget::onFigutrDestroy);
    }
    m_fig = nullptr;
    SAChart2D* plot = fig->current2DPlot();
    if(nullptr != plot)
    {
        //关联槽
        connect(plot,&QObject::destroyed,this,&SAFigureSetWidget::onPlotDestroy);
    }
    ui->chartSetWidget->setChart(plot);
    m_fig = fig;

#endif
}

void SAFigureSetWidget::clear()
{
#if SAFiugreSetWidget_USE_COMBOX
    while(ui->chartSelectComboBox->count())
    {
        ui->chartSelectComboBox->removeItem(0);
    }
#endif
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


void SAFigureSetWidget::onChartTitleChanged(const QString &text)
{
#if SAFiugreSetWidget_USE_COMBOX
    //ui->chartSelectComboBox->setCurrentText(text);
    ui->chartSelectComboBox->setItemText(ui->chartSelectComboBox->currentIndex(),text);
#endif
}

void SAFigureSetWidget::onPlotDestroy(QObject *obj)
{
#if SAFiugreSetWidget_USE_COMBOX
    int count = ui->chartSelectComboBox->count();
    for(int i=0;i<count;++i)
    {
        QVariant var = ui->chartSelectComboBox->itemData(i);
        if(var.canConvert<qintptr>())
        {
            qintptr ptr = var.value<qintptr>();
            if(ptr == ((qintptr)obj))
            {
                ui->chartSelectComboBox->removeItem(i);
                return;
            }
        }
    }
#else

#endif
}

void SAFigureSetWidget::onFigutrDestroy(QObject *obj)
{
    Q_UNUSED(obj);
    m_fig = nullptr;
}



#if SAFiugreSetWidget_USE_COMBOX
void SAFigureSetWidget::onComboxChanged(int index)
{
    if(nullptr == m_fig||index<0)
    {
        return;
    }
    QList<SAChart2D*> plots = m_fig->get2DPlots();
    if(index >= 0 && index < plots.size())
    {
        SAChart2D* p = plots[index];
        ui->chartSetWidget->setChart(p);
    }
}

#endif
