#include "SAColorSetPropertyItem.h"
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
    void setupUi(SAColorSetPropertyItem *par)
    {
        colorButton = new QtColorPicker(par);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setStandardColors();
        par->setWidget(colorButton);
        par->connect(colorButton,&QtColorPicker::colorChanged
                ,par,&SAColorSetPropertyItem::colorChanged);
    } // setupUi


};


SAColorSetPropertyItem::SAColorSetPropertyItem(QWidget *parent) :
    SAPropertyItemContainer(parent),
    ui(new SAColorSetPropertyItem::UI)
{
    ui->setupUi(this);

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
