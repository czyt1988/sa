#ifndef TENDENCYCHARTDATAMODESEL_H
#define TENDENCYCHARTDATAMODESEL_H

#include <QDialog>
class SAChart2D;
class QwtPlotCurve;
namespace Ui {
class TendencyChartDataModeSel;
}

class TendencyChartDataModeSel : public QDialog
{
    Q_OBJECT

public:
    explicit TendencyChartDataModeSel(SAChart2D* chart,QWidget *parent = 0);
    ~TendencyChartDataModeSel();
    bool isFollow() const;
    bool isSelDef() const;
    void getSelDefData(double& start,double& added) const;
    QwtPlotCurve* getSelFollowCurs() const;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::TendencyChartDataModeSel *ui;
    SAChart2D* m_chart;
};

#endif // TENDENCYCHARTDATAMODESEL_H
