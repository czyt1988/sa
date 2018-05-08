#include "SAVCollapsibleGroupBox.h"
#include <QVBoxLayout>
#include <QStyle>
#include <QStyleOption>
#include <QApplication>





class SAVCollapsibleGroupBox::UI
{
public:
    QVBoxLayout* vlayout;
    void setupUI(SAVCollapsibleGroupBox* par)
    {
        vlayout = new QVBoxLayout(par);
        vlayout->setContentsMargins(4, 4, 4, 4);
        vlayout->setSpacing(4);
    }
};

SAVCollapsibleGroupBox::SAVCollapsibleGroupBox(QWidget* par):SACollapsibleGroupBox(par)
  ,ui(new SAVCollapsibleGroupBox::UI)
{
    ui->setupUI(this);
}
SAVCollapsibleGroupBox::SAVCollapsibleGroupBox(const QString &title, QWidget* par):SACollapsibleGroupBox(title,par)
  ,ui(new SAVCollapsibleGroupBox::UI)
{
    ui->setupUI(this);
}
SAVCollapsibleGroupBox::~SAVCollapsibleGroupBox()
{
    delete ui;
}


void SAVCollapsibleGroupBox::addWidget(QWidget *w, int stretch,Qt::Alignment alignment)
{
    ui->vlayout->addWidget(w,stretch,alignment);
}

void SAVCollapsibleGroupBox::addLayout(QLayout *layout, int stretch)
{
    ui->vlayout->addLayout(layout,stretch);
}




