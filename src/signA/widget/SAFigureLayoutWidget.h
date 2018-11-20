#ifndef SAFIGURELAYOUTWIDGET_H
#define SAFIGURELAYOUTWIDGET_H

#include <QWidget>
class SAPlotLayerModel;
class SAFigureWindow;
class SAChart2D;
class QwtPlotItem;
class QwtPlot;
namespace Ui {
class SAFigureLayoutWidget;
}
///
/// \brief 图层管理窗口
///
class SAFigureLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAFigureLayoutWidget(QWidget *parent = 0);
    ~SAFigureLayoutWidget();
    //获取model
    SAPlotLayerModel *getLayoutModel() const;
    //设置figure
    void setFigure(SAFigureWindow* fig);
    //获取当前的figure
    SAFigureWindow* currentFigure() const;
    //图表标题改变
    void setChartTitle(QwtPlot* chart,const QString& title);
signals:
    ///
    /// \brief 图层修改item的可见性
    /// \param chart 图指针
    /// \param item item指针
    /// \param visible 可见性
    ///
    void itemVisibleChanged(SAChart2D* chart,QwtPlotItem* item,bool visible);
    ///
    /// \brief 图层修改item颜色发送的信号
    /// \param chart 图指针
    /// \param item item指针
    /// \param clr 颜色
    ///
    void itemColorChanged(SAChart2D* chart,QwtPlotItem* item,QColor clr);
    ///
    /// \brief item删除发送的信号
    /// \param chart
    /// \param item
    ///
    void itemRemoved(SAChart2D* chart,QwtPlotItem* item);

public slots:
    //更新图层
    void updateLayout();
    //更新当前选中的图表
    void updateCurrentChart();
private slots:
    //表格点击
    void onTableViewLayerPressed(const QModelIndex &index);
    //清除按钮点击
    void onToolButtonDeleteClicked(bool on);
    //当前绘图选择改变
    void onComboBoxCurrentChartCurrentIndexChanged(int index);
    //添加图表
    void onChartAdded(QwtPlot* plot);
    void onChartRemoved(QwtPlot* plot);
private:
    Ui::SAFigureLayoutWidget *ui;
    SAPlotLayerModel* m_layoutModel;
    SAFigureWindow* m_figure;

};

#endif // SAFIGURELAYOUTWIDGET_H
