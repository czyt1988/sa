#ifndef DIALOG_SELDEFSERIES_H
#define DIALOG_SELDEFSERIES_H

#include <QDialog>

namespace Ui {
class Dialog_SelDefSeries;
}

class Dialog_SelDefSeries : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog_SelDefSeries(QWidget *parent = 0);
    ~Dialog_SelDefSeries();
	double getStartData(){return m_start;}
	double getDetalData(){return m_detal;}
private slots:
	void valueChanged_start(double d);
	void valueChanged_detal(double d);
private:
    Ui::Dialog_SelDefSeries *ui;
	double m_detal;
	double m_start;
	const size_t m_size;
	void refleshView();
};

#endif // DIALOG_SELDEFSERIES_H
