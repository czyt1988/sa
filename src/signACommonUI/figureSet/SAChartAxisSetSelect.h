#ifndef SACHARTAXISSETSELECT_H
#define SACHARTAXISSETSELECT_H
#include "SACommonUIGlobal.h"
#include <QWidget>
class QButtonGroup;
namespace Ui {
class SAChartAxisSetSelect;
}
class QAbstractButton;
class SA_COMMON_UI_EXPORT SAChartAxisSetSelect : public QWidget
{
    Q_OBJECT

public:
    explicit SAChartAxisSetSelect(QWidget *parent = 0);
    ~SAChartAxisSetSelect();
    void setCurrentSelectAxis(int xAxis,int yAxis);
    int getCurrentSelectAxisX() const;
    int getCurrentSelectAxisY() const;
public slots:
    void onButtonClicked(QAbstractButton* btn);
signals:
    void axisChanged(int xAxis,int yAxis);
private:
    Ui::SAChartAxisSetSelect *ui;
    QButtonGroup* mXAxisButtonGroup;
    QButtonGroup* mYAxisButtonGroup;
};

#endif // SACHARTAXISSETSELECT_H
