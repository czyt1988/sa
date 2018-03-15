#ifndef SAQWTPLOTITEMAXISBINDSETWIDGET_H
#define SAQWTPLOTITEMAXISBINDSETWIDGET_H

#include <QWidget>
class QButtonGroup;
namespace Ui {
class SAQwtPlotItemAxisBindSetWidget;
}
///
/// \brief 用于设置qwtplotitem绑定的坐标轴
///
class SAQwtPlotItemAxisBindSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAQwtPlotItemAxisBindSetWidget(QWidget *parent = 0);
    ~SAQwtPlotItemAxisBindSetWidget();
    int getXAxisBind() const;
    void setXAxisBind(int axisID) ;
    int getYAxisBind() const;
    void setYAxisBind(int axisID) ;
signals:
    void axisBindChanged(int xaxis,int yaxis);
private slots:
    void onBtnChanged(int id);
private:
    Ui::SAQwtPlotItemAxisBindSetWidget *ui;
    QButtonGroup* m_xBtnGroup;
    QButtonGroup* m_yBtnGroup;
};

#endif // SAQWTPLOTITEMAXISBINDSETWIDGET_H
