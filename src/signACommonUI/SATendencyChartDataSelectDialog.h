#ifndef TENDENCYCHARTDATAMODESEL_H
#define TENDENCYCHARTDATAMODESEL_H
#include "SACommonUIGlobal.h"
#include <QDialog>
class SAChart2D;
class QwtPlotCurve;
namespace Ui {
class SATendencyChartDataSelectDialog;
}

class SA_COMMON_UI_EXPORT SATendencyChartDataSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SATendencyChartDataSelectDialog(SAChart2D* chart,QWidget *parent = 0);
    ~SATendencyChartDataSelectDialog();
    bool isFollow() const;
    bool isSelDef() const;
    void getSelDefData(double& start,double& added) const;
    QwtPlotCurve* getSelFollowCurs() const;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SATendencyChartDataSelectDialog *ui;
    SAChart2D* m_chart;
};

#endif // TENDENCYCHARTDATAMODESEL_H
