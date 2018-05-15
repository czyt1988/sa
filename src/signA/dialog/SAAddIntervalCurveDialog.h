#ifndef SAADDINTERVALCURVEDIALOG_H
#define SAADDINTERVALCURVEDIALOG_H

#include <QDialog>
class SAAbstractDatas;

namespace Ui {
class SAAddIntervalCurveDialog;
}

class SAAddIntervalCurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SAAddIntervalCurveDialog(QWidget *parent = 0);
    ~SAAddIntervalCurveDialog();
    //获取最后选择的x，ymin,ymax值
    SAAbstractDatas *getValueDatas() const;
    SAAbstractDatas* getMinDatas() const;
    SAAbstractDatas* getMaxDatas() const;
    //
    void setValueDatas(SAAbstractDatas* d);
    void setMinDatas(SAAbstractDatas* d);
    void setMaxDatas(SAAbstractDatas* d);
    //曲线的名
    QString getChartTitle() const;
    void setChartTitle(const QString& title);
    //是否添加到当前的chart
    bool isAddInCurrentChart() const;
private:
    Ui::SAAddIntervalCurveDialog *ui;
};

#endif // SAADDINTERVALCURVEDIALOG_H
