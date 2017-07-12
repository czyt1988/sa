#include "SAFontComboBoxPropertyItem.h"
#include <QFontComboBox>
class SAFontComboBoxPropertyItem::UI
{
public:
    QFontComboBox* fontComboBox;
    void setupUI(SAFontComboBoxPropertyItem* par)
    {
        fontComboBox = new QFontComboBox(par);
        fontComboBox->setObjectName(QStringLiteral("fontComboBox"));
        par->setWidget(fontComboBox);
        par->connect(fontComboBox,&QFontComboBox::currentFontChanged
                ,par,&SAFontComboBoxPropertyItem::currentFontChanged);

    }
};

SAFontComboBoxPropertyItem::SAFontComboBoxPropertyItem(QWidget *par)
    :SAPropertyItemContainer(par)
    ,ui(new SAFontComboBoxPropertyItem::UI)
{
    ui->setupUI(this);
}

SAFontComboBoxPropertyItem::~SAFontComboBoxPropertyItem()
{
    delete ui;
}

QFontComboBox *SAFontComboBoxPropertyItem::getFontComboBox() const
{
    return ui->fontComboBox;
}

void SAFontComboBoxPropertyItem::setCurrentFont(const QFont &f)
{
    ui->fontComboBox->setCurrentFont(f);
}
