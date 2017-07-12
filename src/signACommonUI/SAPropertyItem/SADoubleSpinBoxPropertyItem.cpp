#include "SADoubleSpinBoxPropertyItem.h"
#include <QDoubleSpinBox>
#include <numeric>
class SADoubleSpinBoxPropertyItem::UI
{
public:
    QDoubleSpinBox *spinBox;
    void setupUi(SADoubleSpinBoxPropertyItem *par)
    {
        spinBox = new QDoubleSpinBox(par);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMinimum(std::numeric_limits<double>::min());
        spinBox->setMaximum(std::numeric_limits<double>::max());
        spinBox->setValue(0);

        par->setWidget(spinBox);
        par->connect(spinBox,static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
                ,par,&SADoubleSpinBoxPropertyItem::valueChanged);
    } // setupUi
};

SADoubleSpinBoxPropertyItem::SADoubleSpinBoxPropertyItem(QWidget *parent)
    :SAPropertyItemContainer(parent)
    ,ui(new SADoubleSpinBoxPropertyItem::UI)
{
    ui->setupUi(this);
}

SADoubleSpinBoxPropertyItem::~SADoubleSpinBoxPropertyItem()
{
    delete ui;
}

void SADoubleSpinBoxPropertyItem::setValue(double v)
{
    ui->spinBox->setValue(v);
}

double SADoubleSpinBoxPropertyItem::getValue() const
{
    return ui->spinBox->value();
}

void SADoubleSpinBoxPropertyItem::setMinimum(double v)
{
    ui->spinBox->setMinimum(v);
}

void SADoubleSpinBoxPropertyItem::setMaximum(double v)
{
    ui->spinBox->setMaximum(v);
}

QDoubleSpinBox *SADoubleSpinBoxPropertyItem::getDoubleSpinBox() const
{
    return ui->spinBox;
}
