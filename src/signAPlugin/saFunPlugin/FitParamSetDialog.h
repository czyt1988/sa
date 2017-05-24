#ifndef FITPARAMSETDIALOG_H
#define FITPARAMSETDIALOG_H

#include <QDialog>

namespace Ui {
class FitParamSetDialog;
}

class FitParamSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FitParamSetDialog(QWidget *parent = 0);
    ~FitParamSetDialog();

private:
    Ui::FitParamSetDialog *ui;
};

#endif // FITPARAMSETDIALOG_H
