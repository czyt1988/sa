#include "SAFiugreSetWidget.h"
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
#include "SAChart2D.h"
class SAFiugreSetWidget::UI
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QMap<QwtPlot*,SAChartSetWidget*> plotMapToWidget;

    void setupUi(QWidget *SAFiugreSetWidget)
    {
        if (SAFiugreSetWidget->objectName().isEmpty())
            SAFiugreSetWidget->setObjectName(QStringLiteral("SAFiugreSetWidget"));
        SAFiugreSetWidget->resize(308, 464);

        tabWidget = new QTabWidget(SAFiugreSetWidget);
        tabWidget->setObjectName(QStringLiteral("SAFiugreSetWidgetTabWidget"));
        tabWidget->setTabPosition(QTabWidget::South);
        tabWidget->tabBar()->setObjectName(QStringLiteral("SAFiugreSetWidgetTabBar"));
        //

        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->addWidget(tabWidget);
        SAFiugreSetWidget->setLayout(verticalLayout);
        retranslateUi(SAFiugreSetWidget);

        tabWidget->setCurrentIndex(0);
    } // setupUi

    void retranslateUi(QWidget *SAFiugreSetWidget)
    {
        SAFiugreSetWidget->setWindowTitle(QApplication::translate("SAFiugreSetWidget", "Form", 0));
    } // retranslateUi
};

SAFiugreSetWidget::SAFiugreSetWidget(QWidget *parent) :
    QWidget(parent)
    ,ui(new SAFiugreSetWidget::UI)
    ,m_fig(nullptr)
{
    ui->setupUi(this);
}

SAFiugreSetWidget::~SAFiugreSetWidget()
{
    delete ui;
}

void SAFiugreSetWidget::setFigureWidget(SAFigureWindow *fig)
{
    disconnectOldFigure();
    m_fig = fig;
    if(nullptr == fig)
    {
        while(ui->tabWidget->count() > 1)
        {
            int removeIndex = ui->tabWidget->count()-1;
            QWidget* w = ui->tabWidget->widget(removeIndex);
            ui->tabWidget->removeTab(removeIndex);
            if(w)
            {
                delete w;
            }
        }
        QWidget* w = ui->tabWidget->widget(0);
        SAChartSetWidget* csw = nullptr;
        if(nullptr == w)
        {
            w = new SAChartSetWidget();
            connect((SAChartSetWidget*)w,&SAChartSetWidget::chartTitleChanged
                    ,this,&SAFiugreSetWidget::onChartTitleChanged);
            ui->tabWidget->addTab(w,QString());
        }
        csw = qobject_cast<SAChartSetWidget*>(w);
        if(nullptr == csw)
        {
            delete w;
            return;
        }
        ui->tabWidget->setTabText(0,tr("none figure"));
    }
    else
    {
        QList<SAChart2D*> plots = fig->get2DPlots();
        //删除多余的tab
        while(ui->tabWidget->count() > plots.size())
        {
            int removeIndex = ui->tabWidget->count()-1;
            QWidget* w = ui->tabWidget->widget(removeIndex);
            ui->tabWidget->removeTab(removeIndex);
            if(w)
            {
                delete w;
            }
        }
        //根据chart个数来建立tab
        ui->plotMapToWidget.clear();
        for(int i=0;i<plots.size();++i)
        {
            QWidget* w = ui->tabWidget->widget(i);
            SAChartSetWidget* csw = nullptr;
            if(nullptr == w)
            {
                w = new SAChartSetWidget();
                connect((SAChartSetWidget*)w,&SAChartSetWidget::chartTitleChanged
                        ,this,&SAFiugreSetWidget::onChartTitleChanged);
                ui->tabWidget->addTab(w,QString());
            }

            csw = qobject_cast<SAChartSetWidget*>(w);
            if(nullptr == csw)
            {
                delete w;
                continue;
            }
            QString title = plots[i]->title().text();
            if(title.isEmpty())
            {
                title = tr("chart %1").arg(i+1);
            }
            ui->tabWidget->setTabText(i,title);
            csw->setChart(plots[i]);
            ui->plotMapToWidget[plots[i]] = csw;
            connect(plots[i],&QObject::destroyed,this,&SAFiugreSetWidget::onPlotDestroy);
        }
    }
}

void SAFiugreSetWidget::onChartTitleChanged(const QString &text)
{
    QObject* obj = sender();
    SAChartSetWidget* w = qobject_cast<SAChartSetWidget*>(obj);
    if(nullptr == w)
    {
        return;
    }
    int index = ui->tabWidget->indexOf(w);
    if(index >= 0)
    {
        if(text.isEmpty())
        {
            ui->tabWidget->setTabText(index,tr("chart %1").arg(index+1));
        }
        else
        {
            ui->tabWidget->setTabText(index,text);
        }
    }
}

void SAFiugreSetWidget::onPlotDestroy(QObject *obj)
{
    QwtPlot* plot = (QwtPlot*)obj;
    SAChartSetWidget* w = ui->plotMapToWidget.value(plot,nullptr);
    if(w)
    {
        int index = ui->tabWidget->indexOf(w);
        if(index >= 0)
        {
            if(ui->tabWidget->count()>1)
            {
                //如果有多个tab，就移除
                ui->tabWidget->removeTab(index);
                delete w;
            }
            else
            {
                //如果tab不只有一个，就保留这个tab
                w->setChart(nullptr);
            }
        }
        ui->plotMapToWidget.remove(plot);
    }
}

void SAFiugreSetWidget::disconnectOldFigure()
{
    if(nullptr == m_fig)
    {
        return;
    }
    for(auto i=ui->plotMapToWidget.begin();i!=ui->plotMapToWidget.end();++i)
    {
        QwtPlot* plot = i.key();
        disconnect(plot,&QObject::destroyed,this,&SAFiugreSetWidget::onPlotDestroy);
    }
    ui->plotMapToWidget.clear();
}
