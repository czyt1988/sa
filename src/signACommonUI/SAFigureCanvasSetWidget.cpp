#include "SAFigureCanvasSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include <QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QTreeWidget>
class SAFigureCanvasSetWidget::UI
{
public:
    QVBoxLayout *verticalLayout;
    SAColorSetPropertyItem* canvasBkColorItem;
    SASpinBoxPropertyItem* sizeWidthItem;
    SASpinBoxPropertyItem* sizeHightItem;
    void setupUI(SAFigureCanvasSetWidget* par)
    {
        verticalLayout = new QVBoxLayout(par);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        canvasBkColorItem = new SAColorSetPropertyItem(par);
        canvasBkColorItem->setObjectName(QStringLiteral("canvasBkColorItem"));

        sizeWidthItem = new SASpinBoxPropertyItem(par);
        sizeHightItem = new SASpinBoxPropertyItem(par);

        verticalLayout->addWidget(canvasBkColorItem);
        verticalLayout->addWidget(sizeWidthItem);
        verticalLayout->addWidget(sizeHightItem);
        verticalLayout->addStretch();
        //verticalLayout->addSpacerItem(new QSpacerItem(par));
        retranslateUi(par);
    }
    void retranslateUi(QWidget *w)
    {
        w->setWindowTitle(QApplication::translate("SAFigureCanvasSetWidget", "Figure Canvas Set Widget", 0));
        canvasBkColorItem->setText( QApplication::translate("SAFigureCanvasSetWidget", "canvas background", 0));
        sizeWidthItem->setText(QApplication::translate("SAFigureCanvasSetWidget", "width", 0));
        sizeHightItem->setText(QApplication::translate("SAFigureCanvasSetWidget", "height", 0));
    } // retranslateUi
};

SAFigureCanvasSetWidget::SAFigureCanvasSetWidget(QWidget *parent):QWidget(parent)
  ,ui(new UI)
  ,m_fig(nullptr)

{
    ui->setupUI(this);
}

SAFigureCanvasSetWidget::~SAFigureCanvasSetWidget()
{
    delete ui;
}

void SAFigureCanvasSetWidget::setFigureWindow(SAFigureWindow *fig)
{
    m_fig = fig;
    ui->canvasBkColorItem->setCurrentColor(fig->palette().color( backgroundRole()));
    ui->sizeWidthItem->setValue(fig->width());
    ui->sizeHightItem->setValue(fig->height());
}
