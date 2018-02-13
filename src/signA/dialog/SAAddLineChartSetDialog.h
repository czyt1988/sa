#ifndef SAADDLINECHARTSETDIALOG_H
#define SAADDLINECHARTSETDIALOG_H

#include <QDialog>

class SAUserDefineAxisDataWidget;
class QButtonGroup;
class QComboBox;
class SAValueSelectComboBox;
class SAAbstractDatas;

namespace Ui {
class SAAddLineChartSetDialog;
}

class SAAddLineChartSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SAAddLineChartSetDialog(QWidget *parent = 0);
    ~SAAddLineChartSetDialog();
    enum AsixSet
    {
        NormalSet = 0
        ,UserDefineSet
    };
    //设置x，y轴当前选中的状态
    void setXAsixSet(AsixSet set);
    void setYAsixSet(AsixSet set);
    //设置x选中的变量名
    void setXSelectName(const QString& name);
    void setYSelectName(const QString& name);
    //设置x，y的用户定义值
    void setXUserDefineValues(double start,double increase);
    void setYUserDefineValues(double start,double increase);
    //获取坐标轴的设置
    AsixSet getXAxisSet() const;
    AsixSet getYAxisSet() const;

    //获取自定义设置的值
    void getXUserDefineValues(double& start,double& increase) const;
    void getYUserDefineValues(double& start,double& increase) const;
    //是否添加到当前的chart
    bool isAddInCurrentChart() const;
    //曲线的名
    QString getChartTitle() const;
    void setChartTitle(const QString& title);
    //获取最后选择的x，y值
    SAAbstractDatas* getXDatas() const;
    SAAbstractDatas* getYDatas() const;
    //

private slots:
    //radio x more set clicked
    void onXRadioButtonClicked(int id);
    //radio y more set clicked
    void onYRadioButtonClicked(int id);
private:
    void showValueComboBoxX(bool on);
    void showValueComboBoxY(bool on);
    void showUserDefX(bool on);
    void showUserDefY(bool on);
private:
    Ui::SAAddLineChartSetDialog *ui;
    QButtonGroup* m_xMoreSetButtonGroup;
    QButtonGroup* m_yMoreSetButtonGroup;
    SAUserDefineAxisDataWidget* m_xUsrDef;
    SAUserDefineAxisDataWidget* m_yUsrDef;
};

#endif // SAADDLINECHARTSETDIALOG_H
