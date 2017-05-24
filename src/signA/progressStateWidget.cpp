#include "progressStateWidget.h"
#include "ui_progressStateWidget.h"
#include <QProgressBar>
progressStateWidget::progressStateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progressStateWidget)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    setText("");
}

progressStateWidget::~progressStateWidget()
{
    delete ui;
}

QProgressBar *progressStateWidget::getProgressBar()
{
    return ui->progressBar;
}

void progressStateWidget::setProgress(int present,const QString& text)
{
    ui->progressBar->setValue(present);
    ui->label->setText(text);
}

void progressStateWidget::setPresent(int present)
{
    ui->progressBar->setValue(present);
}

void progressStateWidget::setText(const QString& text)
{
    ui->label->setText(text);
}
