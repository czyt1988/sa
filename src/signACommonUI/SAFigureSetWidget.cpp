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
#if SAFiugreSetWidget_USE_COMBOX
    QComboBox* chartSelectComboBox;
    SAChartSetWidget* chartSetWidget;
#else
    QTabWidget *tabWidget;
#endif
    QMap<QwtPlot*,SAChartSetWidget*> plotMapToWidget;

    void setupUi(SAFigureSetWidget *p)
    {
        if (p->objectName().isEmpty())
            p->setObjectName(QStringLiteral("SAFiugreSetWidget"));
        p->resize(308, 464);

#if SAFiugreSetWidget_USE_COMBOX
        chartSelectComboBox = new QComboBox(p);
        chartSelectComboBox->setObjectName(QStringLiteral("SAFiugreSetWidgetChartSelectComboBox"));
        chartSetWidget = new SAChartSetWidget(p);
        chartSetWidget->setObjectName(QStringLiteral("ChartSetWidget"));
        chartSetWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->addWidget(chartSelectComboBox);
        verticalLayout->addWidget(chartSetWidget);
        p->setLayout(verticalLayout);
        p->connect(chartSetWidget,&SAChartSetWidget::chartTitleChanged
                ,p,&SAFigureSetWidget::onChartTitleChanged);
        p->connect(chartSelectComboBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                   ,p,&SAFigureSetWidget::onComboxChanged);
        retranslateUi(p);
#else
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
#endif
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
    m_fig = fig;
    disconnectOldFigure();
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
#endif
}

#ifdef SAFiugreSetWidget_USE_COMBOX
void SAFigureSetWidget::clear()
{
    while(ui->chartSelectComboBox->count())
    {
        ui->chartSelectComboBox->removeItem(0);
    }
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
#endif

void SAFigureSetWidget::onChartTitleChanged(const QString &text)
{
#if SAFiugreSetWidget_USE_COMBOX
    //ui->chartSelectComboBox->setCurrentText(text);
    ui->chartSelectComboBox->setItemText(ui->chartSelectComboBox->currentIndex(),text);
#else
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
#else
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
#endif
