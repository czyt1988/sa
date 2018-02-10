#include "SAColorSetPropertyItem.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "SAColorPickerButton.h"
#include <QHBoxLayout>
#include <QSlider>
class SAColorSetPropertyItem::UI
{
public:
    QSlider *horizontalSliderColorAlpha;
    QHBoxLayout *horizontalLayout;
    QtColorPicker *colorButton;
    void setupUi(SAColorSetPropertyItem *par)
    {
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        colorButton = new SAColorPickerButton(par);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setStandardColors();

        horizontalSliderColorAlpha = new QSlider(par);
        horizontalSliderColorAlpha->setObjectName(QStringLiteral("horizontalSliderColorAlpha"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(horizontalSliderColorAlpha->sizePolicy().hasHeightForWidth());

        horizontalSliderColorAlpha->setSizePolicy(sizePolicy1);
        horizontalSliderColorAlpha->setMinimumSize(QSize(8, 0));
        horizontalSliderColorAlpha->setOrientation(Qt::Horizontal);
        horizontalSliderColorAlpha->setRange(0,255);

        horizontalLayout->addWidget(colorButton);
        horizontalLayout->addWidget(horizontalSliderColorAlpha);
        par->addLayout(horizontalLayout);
        par->connect(colorButton,&QtColorPicker::colorChanged
                ,par,&SAColorSetPropertyItem::onColorChanged);
        par->connect(horizontalSliderColorAlpha,&QSlider::valueChanged
                ,par,&SAColorSetPropertyItem::onCurrentPenColorAlphaChanged);
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
    ui->horizontalSliderColorAlpha->setValue(clr.alpha());
}

QColor SAColorSetPropertyItem::getCurrentColor() const
{
    QColor clr = ui->colorButton->currentColor();
    clr.setAlpha(ui->horizontalSliderColorAlpha->value());
    return clr;
}

QtColorPicker *SAColorSetPropertyItem::getColorPickerButton()
{
    return ui->colorButton;
}

void SAColorSetPropertyItem::onColorChanged(const QColor &c)
{
    emit colorChanged(getCurrentColor());
}

void SAColorSetPropertyItem::onCurrentPenColorAlphaChanged(int v)
{
    Q_UNUSED(v);
    emit colorChanged(getCurrentColor());
}
