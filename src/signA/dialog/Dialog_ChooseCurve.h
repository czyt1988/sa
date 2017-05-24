#ifndef DIALOG_CHOOSECURVE_H
#define DIALOG_CHOOSECURVE_H

#include <QDialog>

namespace Ui {
class Dialog_ChooseCurve;
}

class Dialog_ChooseCurve : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_ChooseCurve(QWidget *parent = 0);
    ~Dialog_ChooseCurve();
    
private:
    Ui::Dialog_ChooseCurve *ui;
};

#endif // DIALOG_CHOOSECURVE_H
