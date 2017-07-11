#include "SAComboBoxPropertyItem.h"
#include <QComboBox>

class SAComboBoxPropertyItem::UI
{
public:
    QComboBox *comboBox;
    void setupUi(SAComboBoxPropertyItem *par)
    {
        comboBox = new QComboBox(par);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        par->setWidget(comboBox);
        par->connect(comboBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
                ,par,static_cast<void(SAComboBoxPropertyItem::*)(int)>(&SAComboBoxPropertyItem::currentIndexChanged));
        par->connect(comboBox,static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged)
                ,par,static_cast<void(SAComboBoxPropertyItem::*)(const QString&)>(&SAComboBoxPropertyItem::currentIndexChanged));
        par->connect(comboBox,&QComboBox::currentTextChanged
                ,par,&SAComboBoxPropertyItem::currentTextChanged);
        par->connect(comboBox,&QComboBox::editTextChanged
                ,par,&SAComboBoxPropertyItem::editTextChanged);

    } // setupUi
};

SAComboBoxPropertyItem::SAComboBoxPropertyItem(QWidget *parent)
    :SAPropertyItemContainer(parent)
    ,ui(new SAComboBoxPropertyItem::UI)
{
    ui->setupUi(this);
}

SAComboBoxPropertyItem::~SAComboBoxPropertyItem()
{
    delete ui;
}

QComboBox *SAComboBoxPropertyItem::getComboBox() const
{
    return ui->comboBox;
}

void SAComboBoxPropertyItem::addItem(const QString &text, const QVariant &userData)
{
    ui->comboBox->addItem(text,userData);
}

void SAComboBoxPropertyItem::addItem(const QIcon &icon, const QString &text, const QVariant &userData)
{
    ui->comboBox->addItem(icon,text,userData);
}

void SAComboBoxPropertyItem::addItems(const QStringList &texts)
{
    ui->comboBox->addItems(texts);
}
