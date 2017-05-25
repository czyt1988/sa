#include "SAProjectInfomationSetDialog.h"
#include "ui_SAProjectInfomationSetDialog.h"

SAProjectInfomationSetDialog::SAProjectInfomationSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAProjectInfomationSetDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);
}

SAProjectInfomationSetDialog::~SAProjectInfomationSetDialog()
{
    delete ui;
}

QString SAProjectInfomationSetDialog::getProjectName() const
{
    return ui->lineEditProjectName->text();
}

void SAProjectInfomationSetDialog::setProjectName(const QString &projectName)
{
    ui->lineEditProjectName->setText(projectName);
}

QString SAProjectInfomationSetDialog::getProjectDescribe() const
{
    return ui->textEditProjectDescribe->toPlainText();
}

void SAProjectInfomationSetDialog::setProjectDescribe(const QString &projectDescribe)
{
    ui->textEditProjectDescribe->setText(projectDescribe);
}
