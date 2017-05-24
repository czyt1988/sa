#ifndef DIALOG_RENAMEPROJECT_H
#define DIALOG_RENAMEPROJECT_H

#include <QDialog>

namespace Ui {
class Dialog_RenameProject;
}

class Dialog_RenameProject : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_RenameProject(QWidget *parent = 0);
    ~Dialog_RenameProject();
    void setProjectName(const QString& m_Name);
    QString getNewProjectName() const;
private:
    Ui::Dialog_RenameProject *ui;
};

#endif // DIALOG_RENAMEPROJECT_H
