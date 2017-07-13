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


    void setupUi(QWidget *SAFiugreSetWidget)
    {
        if (SAFiugreSetWidget->objectName().isEmpty())
            SAFiugreSetWidget->setObjectName(QStringLiteral("SAFiugreSetWidget"));
        SAFiugreSetWidget->resize(308, 464);

        tabWidget = new QTabWidget(SAFiugreSetWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::South);
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
        }
    }
}

void SAFiugreSetWidget::onChartTitleChanged(const QString &text)
{
    int index = ui->tabWidget->currentIndex();
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
