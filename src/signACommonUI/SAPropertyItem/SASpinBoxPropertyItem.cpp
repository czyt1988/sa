#include "SASpinBoxPropertyItem.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QSpinBox>
#include <numeric>
class SASpinBoxPropertyItem::UI
{
public:
    QSpinBox *spinBox;
    void setupUi(SASpinBoxPropertyItem *par)
    {
        par->setObjectName(QStringLiteral("SASpinBoxPropertyItem"));
        spinBox = new QSpinBox(par);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setValue(0);
        spinBox->setMinimum(std::numeric_limits<int>::min());
        spinBox->setMaximum(std::numeric_limits<int>::max());
        par->setWidget(spinBox);
        par->connect(spinBox,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)
                ,par,&SASpinBoxPropertyItem::valueChanged);

    } // setupUi

};


SASpinBoxPropertyItem::SASpinBoxPropertyItem(QWidget *parent) :
    SAPropertyItemContainer(parent),
    ui(new SASpinBoxPropertyItem::UI)
{
    ui->setupUi(this);
}

SASpinBoxPropertyItem::~SASpinBoxPropertyItem()
{
    delete ui;
}


void SASpinBoxPropertyItem::setValue(int v)
{
    ui->spinBox->setValue(v);
}

int SASpinBoxPropertyItem::getValue() const
{
    return ui->spinBox->value();
}

QSpinBox *SASpinBoxPropertyItem::getSpinBox() const
{
    return ui->spinBox;
}

void SASpinBoxPropertyItem::setMinimum(int v)
{
    ui->spinBox->setMinimum(v);
}

void SASpinBoxPropertyItem::setMaximum(int v)
{
    ui->spinBox->setMaximum(v);
}

