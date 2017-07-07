#include "SAColorSetPropertyItem.h"
#include "ui_SAColorSetPropertyItem.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "qtcolorpicker.h"

class SAColorSetPropertyItem::UI
{
public:
    QtColorPicker *colorButton;
    void setupUi(QWidget *SAColorSetPropertyItem)
    {
        colorButton = new QtColorPicker(SAColorSetPropertyItem);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setStandardColors();
    } // setupUi


};


SAColorSetPropertyItem::SAColorSetPropertyItem(QWidget *parent) :
    SAPropertyItemContainer(parent),
    ui(new SAColorSetPropertyItem::UI)
{
    ui->setupUi(this);
    //setAutoFillBackground(true);
    setWidget(ui->colorButton);
    connect(ui->colorButton,&QtColorPicker::colorChanged
            ,this,&SAColorSetPropertyItem::colorChanged);
}

SAColorSetPropertyItem::~SAColorSetPropertyItem()
{
    delete ui;
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
