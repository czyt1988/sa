#include "dialog_renameproject.h"
#include "ui_dialog_renameproject.h"

Dialog_RenameProject::Dialog_RenameProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_RenameProject)
{
    ui->setupUi(this);
}

Dialog_RenameProject::~Dialog_RenameProject()
{
    delete ui;
}

void Dialog_RenameProject::setProjectName(const QString& m_Name)
{
    ui->lineEdit->setText(m_Name);
}

QString Dialog_RenameProject::getNewProjectName() const
{
    return ui->lineEdit->text();
}
