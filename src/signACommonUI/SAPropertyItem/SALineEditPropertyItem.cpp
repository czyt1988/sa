#include "SALineEditPropertyItem.h"
#include <QLineEdit>
class SALineEditPropertyItem::UI
{
public:
    QLineEdit* lineEdit;
    void setupUI(SALineEditPropertyItem* par)
    {
        par->setObjectName(QStringLiteral("SASpinBoxPropertyItem"));
        lineEdit = new QLineEdit(par);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        par->setWidget(lineEdit);
        par->connect(lineEdit,&QLineEdit::textChanged
                ,par,&SALineEditPropertyItem::textChanged);

    }
};

SALineEditPropertyItem::SALineEditPropertyItem(QWidget *parent)
    :SAPropertyItemContainer(parent)
    ,ui(new SALineEditPropertyItem::UI)
{
    ui->setupUI(this);
}

SALineEditPropertyItem::~SALineEditPropertyItem()
{
    delete ui;
}

void SALineEditPropertyItem::setEditText(const QString &text)
{
    ui->lineEdit->setText(text);
}

QString SALineEditPropertyItem::getEditText() const
{
    return ui->lineEdit->text();
}
