#ifndef SPECTRUMSETDIALOG_H
#define SPECTRUMSETDIALOG_H

#include <QDialog>

namespace Ui {
class SpectrumSetDialog;
}

class SpectrumSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrumSetDialog(QWidget *parent = 0);
    ~SpectrumSetDialog();

private:
    Ui::SpectrumSetDialog *ui;
};

#endif // SPECTRUMSETDIALOG_H
