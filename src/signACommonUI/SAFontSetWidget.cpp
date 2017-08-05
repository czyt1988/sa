#include "SAFontSetWidget.h"
#include "ui_SAFontSetWidget.h"

SAFontSetWidget::SAFontSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAFontSetWidget)
{
    ui->setupUi(this);
    connect(ui->toolButtonBold,&QToolButton::toggled
            ,this,&SAFontSetWidget::onToolButtonBoldToggled);
    connect(ui->toolButtonItalic,&QToolButton::toggled
            ,this,&SAFontSetWidget::onToolButtonItalicToggled);
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

void SAFontSetWidget::onToolButtonBoldToggled(bool checked)
{
    QFont font = ui->fontComboBox->currentFont();
    font.setBold(checked);
    font.setItalic(ui->toolButtonItalic->isChecked());
    emit fontChanged(font);
}

void SAFontSetWidget::onToolButtonItalicToggled(bool checked)
{
    QFont font = ui->fontComboBox->currentFont();
    font.setBold(ui->toolButtonBold->isChecked());
    font.setItalic(checked);
    emit fontChanged(font);
}

void SAFontSetWidget::onFontChanged(const QFont &newfont)
{
    QFont font = newfont;
    font.setBold(ui->toolButtonBold->isChecked());
    font.setItalic(ui->toolButtonItalic->isChecked());
    emit fontChanged(font);
}
