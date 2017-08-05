#include "SAQwtDateScaleSeWidget.h"
#include "ui_SAQwtDateScaleSeWidget.h"
SAQwtDateScaleSeWidget::SAQwtDateScaleSeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAQwtDateScaleSeWidget)
{
    ui->setupUi(this);
    ui->comboBox->addItems(QStringList()<<"h:m"<<"hh:mm"<<"h:m:s"<<"hh:mm:ss"
                           <<"yyyy-M-d"<<"yyyy-M-d h:m"<<"yyyy-M-d h:m:s"
                          <<"yyyy-MM-dd"<<"yyyy-MM-dd hh:mm"<<"yyyy-MM-dd hh:mm:ss");
    connect(ui->comboBox,&QComboBox::currentTextChanged
            ,this,&SAQwtDateScaleSeWidget::timeFormatStringChanged);
}

SAQwtDateScaleSeWidget::~SAQwtDateScaleSeWidget()
{
    delete ui;
}

void SAQwtDateScaleSeWidget::setText(const QString &text)
{
    ui->comboBox->setCurrentText(text);
}

QString SAQwtDateScaleSeWidget::text() const
{
    return ui->comboBox->currentText();
}
