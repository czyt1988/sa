#include "SAFontSetWidget.h"
#include "ui_SAFontSetWidget.h"
#include <QFontDialog>
SAFontSetWidget::SAFontSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAFontSetWidget)
{
    ui->setupUi(this);
    connect(ui->toolButtonBold,&QToolButton::toggled
            ,this,&SAFontSetWidget::onToolButtonBoldToggled);
    connect(ui->toolButtonItalic,&QToolButton::toggled
            ,this,&SAFontSetWidget::onToolButtonItalicToggled);
    connect(ui->toolButtonFontSet,&QToolButton::pressed
            ,this,&SAFontSetWidget::onToolButtonFontSetPressed);
    connect(ui->fontComboBox,&QFontComboBox::currentFontChanged
            ,this,&SAFontSetWidget::onFontChanged);
}

SAFontSetWidget::~SAFontSetWidget()
{
    delete ui;
}

void SAFontSetWidget::setCurrentFont(const QFont &font)
{
    ui->fontComboBox->setCurrentFont(font);
    ui->toolButtonBold->setChecked(font.bold());
    ui->toolButtonItalic->setChecked(font.italic());
}

QFont SAFontSetWidget::getFont() const
{
    QFont font = ui->fontComboBox->currentFont();
    font.setBold(ui->toolButtonBold->isChecked());
    font.setItalic(ui->toolButtonItalic->isChecked());
    return font;
}

void SAFontSetWidget::onToolButtonBoldToggled(bool checked)
{
    Q_UNUSED(checked);
    emit fontChanged(getFont());
}

void SAFontSetWidget::onToolButtonItalicToggled(bool checked)
{
    Q_UNUSED(checked);
    emit fontChanged(getFont());
}

void SAFontSetWidget::onToolButtonFontSetPressed()
{
    bool isOK = false;
    QFont font = QFontDialog::getFont(&isOK,getFont(),this);
    if(isOK)
    {
        setCurrentFont(font);
        emit fontChanged(font);
    }
}

void SAFontSetWidget::onFontChanged(const QFont &newfont)
{
    Q_UNUSED(newfont);
    emit fontChanged(getFont());
}
