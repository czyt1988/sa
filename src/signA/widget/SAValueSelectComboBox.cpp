#include "SAValueSelectComboBox.h"
#include "ui_SAValueSelectComboBox.h"
#include "SAValueManager.h"
SAValueSelectComboBox::SAValueSelectComboBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAValueSelectComboBox)
  ,m_fun(nullptr)
{
    ui->setupUi(this);
}

SAValueSelectComboBox::~SAValueSelectComboBox()
{
    delete ui;
}

void SAValueSelectComboBox::setFilter(SAValueSelectComboBox::FunPtrFilter filterFun)
{
    m_fun = filterFun;
    updateItems();
}

void SAValueSelectComboBox::setText(const QString &text)
{
    ui->label->setText(text);
}

QString SAValueSelectComboBox::text() const
{
    return ui->label->text();
}

void SAValueSelectComboBox::setComboBoxText(const QString &text)
{
    ui->comboBox->setCurrentText(text);
}

QString SAValueSelectComboBox::comboBoxText() const
{
    return ui->comboBox->currentText();
}

SAAbstractDatas *SAValueSelectComboBox::getSelectData() const
{
    return saValueManager->findData(comboBoxText());
}

void SAValueSelectComboBox::updateItems()
{
    ui->comboBox->clear();
    const int size = saValueManager->count();

    if(nullptr != m_fun)
    {
        for(int i=0;i<size;++i)
        {
            SAAbstractDatas* p = saValueManager->at(i);
            if(m_fun(p,i))
            {
                ui->comboBox->addItem(SAValueManager::getDataIcon(p->getType())
                                      ,p->getName()
                                      ,i);
            }
        }
    }
    else
    {
        for(int i=0;i<size;++i)
        {
            SAAbstractDatas* p = saValueManager->at(i);
            ui->comboBox->addItem(SAValueManager::getDataIcon(p->getType())
                                  ,p->getName()
                                  ,i);
        }
    }
}

bool filterSAValueSelectComboBox1Dim(const SAAbstractDatas *p, int index)
{
    Q_UNUSED(index);
    if(p)
    {
        if(SA::Dim1 == p->getDim())
        {
            return true;
        }
    }
    return false;
}
