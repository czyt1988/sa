#ifndef SACHARTDATASVIEWWIDGET_H
#define SACHARTDATASVIEWWIDGET_H

#include <QWidget>
class QwtPlotItemTreeModel;
class QwtPlotItemDataModel;
class SAFigureWindow;
class QwtPlot;
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
    //figure有图添加发送的信号
    void onChartAdded(QwtPlot* plot);
    //figure销毁
    void onFigureDestroy(QObject* obj);
private:
    Ui::SAChartDatasViewWidget *ui;
    QwtPlotItemTreeModel* m_treeModel;
    QwtPlotItemDataModel* m_tableModel;
    SAFigureWindow* m_figure;
};

#endif // SACHARTDATASVIEWWIDGET_H
