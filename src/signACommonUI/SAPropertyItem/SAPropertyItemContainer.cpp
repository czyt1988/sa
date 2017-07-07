#include "SAPropertyItemContainer.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
class SAPropertyItemContainer::UI
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *labelName;
    void setupUi(QWidget *w)
    {
        if (w->objectName().isEmpty())
            w->setObjectName(QStringLiteral("SAColorSetPropertyItem"));
        w->resize(221, 23);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
        w->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(w);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelName = new QLabel(w);
        labelName->setObjectName(QStringLiteral("labelName"));

        horizontalLayout->addWidget(labelName);


    } // setupUi


};
SAPropertyItemContainer::SAPropertyItemContainer(QWidget *parent):QWidget(parent)
  ,ui(new SAPropertyItemContainer::UI)
{
    ui->setupUi(this);
}

SAPropertyItemContainer::~SAPropertyItemContainer()
{
    delete ui;
}

void SAPropertyItemContainer::setText(const QString &text)
{
    ui->labelName->setText(text);
}

QString SAPropertyItemContainer::getText() const
{
    return ui->labelName->text();
}



void SAPropertyItemContainer::setWidget(QWidget *w)
{
    ui->horizontalLayout->addWidget(w);
}
