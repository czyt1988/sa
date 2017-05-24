#ifndef EXPROTCURVEDATADIALOG_H
#define EXPROTCURVEDATADIALOG_H
#include "SAGlobals.h"
#include <QDialog>

namespace Ui {
class PickCurveDataModeSetDialog;
}

class PickCurveDataModeSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PickCurveDataModeSetDialog(QWidget *parent = 0);
    ~PickCurveDataModeSetDialog();
    SA::ViewRange getViewRange() const;
    SA::PickDataMode getPickDataMode() const;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::PickCurveDataModeSetDialog *ui;
};

#endif // EXPROTCURVEDATADIALOG_H
