#include "SAColorSetPropertyItem.h"
#include "ui_SAColorSetPropertyItem.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "qtcolorpicker.h"

class SAColorSetPropertyItem::UI
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *labelName;
    QtColorPicker *colorButton;
    void setupUi(QWidget *SAColorSetPropertyItem)
    {
        if (SAColorSetPropertyItem->objectName().isEmpty())
            SAColorSetPropertyItem->setObjectName(QStringLiteral("SAColorSetPropertyItem"));
        SAColorSetPropertyItem->resize(221, 23);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SAColorSetPropertyItem->sizePolicy().hasHeightForWidth());
        SAColorSetPropertyItem->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(SAColorSetPropertyItem);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelName = new QLabel(SAColorSetPropertyItem);
        labelName->setObjectName(QStringLiteral("labelName"));

        horizontalLayout->addWidget(labelName);

        colorButton = new QtColorPicker(SAColorSetPropertyItem);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setStandardColors();
        horizontalLayout->addWidget(colorButton);


        retranslateUi(SAColorSetPropertyItem);

        QMetaObject::connectSlotsByName(SAColorSetPropertyItem);
    } // setupUi

    void retranslateUi(QWidget *SAColorSetPropertyItem)
    {
        SAColorSetPropertyItem->setWindowTitle(QApplication::translate("SAColorSetPropertyItem", "Form", 0));
        labelName->setText(QString());
        colorButton->setText(QString());
    } // retranslateUi

};


SAColorSetPropertyItem::SAColorSetPropertyItem(QWidget *parent) :
    QWidget(parent),
    ui(new SAColorSetPropertyItem::UI)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    connect(ui->colorButton,&QtColorPicker::colorChanged
            ,this,&SAColorSetPropertyItem::colorChanged);
}

SAColorSetPropertyItem::~SAColorSetPropertyItem()
{
    delete ui;
}
///
/// \brief SAColorSetPropertyItem::setText
/// \param text
///
void SAColorSetPropertyItem::setText(const QString &text)
{
    ui->labelName->setText(text);
}
///
/// \brief SAColorSetPropertyItem::getText
/// \return
///
QString SAColorSetPropertyItem::getText() const
{
    return ui->labelName->text();
}
///
/// \brief SAColorSetPropertyItem::setColor
/// \param clr
///
void SAColorSetPropertyItem::setCurrentColor(const QColor &clr)
{
    ui->colorButton->setCurrentColor(clr);
}

QColor SAColorSetPropertyItem::getCurrentColor() const
{
    return ui->colorButton->currentColor();
}

QtColorPicker *SAColorSetPropertyItem::getColorPickerButton()
{
    return ui->colorButton;
}
