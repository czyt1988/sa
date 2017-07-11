#include "SADoubleSpinBoxPropertyItem.h"
#include <QDoubleSpinBox>
class SADoubleSpinBoxPropertyItem::UI
{
public:
    QDoubleSpinBox *spinBox;
    void setupUi(SADoubleSpinBoxPropertyItem *par)
    {
        spinBox = new QDoubleSpinBox(par);
        spinBox->setObjectName(QStringLiteral("spinBox"));
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

QDoubleSpinBox *SADoubleSpinBoxPropertyItem::getDoubleSpinBox() const
{
    return ui->spinBox;
}
