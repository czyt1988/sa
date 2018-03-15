#ifndef SACHARTDATASVIEWWIDGET_H
#define SACHARTDATASVIEWWIDGET_H

#include <QWidget>
class QwtPlotItemTreeModel;
class SAPlotDataModel;
class SAFigureWindow;
class QwtPlot;
class SACellInputWidget;
class QwtPlotItem;
namespace Ui {
class SAChartDatasViewWidget;
}
///
/// \brief 曲线数据查看窗口
///
class SAChartDatasViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAChartDatasViewWidget(QWidget *parent = 0);
    ~SAChartDatasViewWidget();
    QwtPlotItemTreeModel* getTreeModel() const;
    void setFigure(SAFigureWindow *fig);
private slots:
    void onTreeViewCurPlotItemClicked(const QModelIndex& index);
    //数据表双击 - 处理插入新数据
    void onTableViewDoubleClicked(const QModelIndex& index);
    //figure有图添加发送的信号
    void onChartAdded(QwtPlot* plot);
    //figure销毁
    void onFigureDestroy(QObject* obj);
private:
    //处理ctrl + v事件
    void onCtrlVInTableView();
    //获取表格的表头的尺寸，返回一个QPoint，x:为竖直表头宽度，y为水平表头高度
    QPoint tableHeaderPositionOffset() const;
    //设置输入窗口的位置
    void initCellInputPosition(SACellInputWidget* w,QwtPlotItem* item,int row,int colStart,int colEnd);
private:
    Ui::SAChartDatasViewWidget *ui;
    QwtPlotItemTreeModel* m_treeModel;
    SAPlotDataModel* m_tableModel;
    SAFigureWindow* m_figure;
};

#endif // SACHARTDATASVIEWWIDGET_H
