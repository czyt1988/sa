#include "SACheckBoxPropertyItem.h"
#include <QCheckBox>
class SACheckBoxPropertyItem::UI
{
public:
    QCheckBox *checkBox;
    void setupUi(SACheckBoxPropertyItem *par)
    {
        checkBox = new QCheckBox(par);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        par->setWidget(checkBox);
        par->connect(checkBox,&QCheckBox::stateChanged
                     ,par,&SACheckBoxPropertyItem::stateChanged);
    } // setupUi
};

SACheckBoxPropertyItem::SACheckBoxPropertyItem(QWidget *parent)
    :SAPropertyItemContainer(parent)
    ,ui(new SACheckBoxPropertyItem::UI)
{
    ui->setupUi(this);
}

SACheckBoxPropertyItem::~SACheckBoxPropertyItem()
{
    delete ui;
}

void SACheckBoxPropertyItem::setCheckState(Qt::CheckState state)
{
    ui->checkBox->setCheckState(state);
}

Qt::CheckState SACheckBoxPropertyItem::checkState() const
{
    return ui->checkBox->checkState();
}
