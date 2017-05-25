#ifndef SAPROPERTYSETDIALOG_H
#define SAPROPERTYSETDIALOG_H

#include <QDialog>

namespace Ui {
class SAProjectInfomationSetDialog;
}

class SAProjectInfomationSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SAProjectInfomationSetDialog(QWidget *parent = 0);
    ~SAProjectInfomationSetDialog();
    //项目名
    QString getProjectName() const;
    void setProjectName(const QString &projectName);
    //项目描述
    QString getProjectDescribe() const;
    void setProjectDescribe(const QString &projectDescribe);

private:
    Ui::SAProjectInfomationSetDialog *ui;
};

#endif // SAPROPERTYSETDIALOG_H
