#include "SAFiugreSetWidget.h"
#include "ui_SAFiugreSetWidget.h"
#include "SAFigureWindow.h"
#include "SAFigureCanvasSetWidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
class SAFiugreSetWidget::UI
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    SAFigureCanvasSetWidget *canvasSetWidget;

    void setupUi(QWidget *SAFiugreSetWidget)
    {
        if (SAFiugreSetWidget->objectName().isEmpty())
            SAFiugreSetWidget->setObjectName(QStringLiteral("SAFiugreSetWidget"));
        SAFiugreSetWidget->resize(308, 464);
        verticalLayout = new QVBoxLayout(SAFiugreSetWidget);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(SAFiugreSetWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::West);
        //
        canvasSetWidget = new SAFigureCanvasSetWidget(SAFiugreSetWidget);
        canvasSetWidget->setObjectName(QStringLiteral("canvasSetWidget"));
        tabWidget->addTab(canvasSetWidget, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(SAFiugreSetWidget);

        tabWidget->setCurrentIndex(0);
    } // setupUi

    void retranslateUi(QWidget *SAFiugreSetWidget)
    {
        SAFiugreSetWidget->setWindowTitle(QApplication::translate("SAFiugreSetWidget", "Form", 0));
        tabWidget->setTabText(tabWidget->indexOf(canvasSetWidget), QApplication::translate("SAFiugreSetWidget", "canvas", 0));
    } // retranslateUi
};

SAFiugreSetWidget::SAFiugreSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new SAFiugreSetWidget::UI)
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
    ui->canvasSetWidget->setFigureWindow(fig);
}
