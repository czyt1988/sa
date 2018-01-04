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

void SAComboBoxPropertyItem::setEditable(bool editable)
{
    ui->comboBox->setEditable(editable);
}

QString SAComboBoxPropertyItem::currentText() const
{
    return ui->comboBox->currentText();
}

QVariant SAComboBoxPropertyItem::currentData(int role) const
{
    return ui->comboBox->currentData(role);
}

int SAComboBoxPropertyItem::currentIndex() const
{
    return ui->comboBox->currentIndex();
}

void SAComboBoxPropertyItem::clearAllItem()
{
    while(ui->comboBox->count())
    {
        ui->comboBox->removeItem(0);
    }
}

void SAComboBoxPropertyItem::setCurrentIndex(int index)
{
    ui->comboBox->setCurrentIndex(index);
}

void SAComboBoxPropertyItem::setCurrentText(const QString &text)
{
    ui->comboBox->setCurrentText(text);
}

void SAComboBoxPropertyItem::setEditText(const QString &text)
{
    ui->comboBox->setEditText(text);
}
