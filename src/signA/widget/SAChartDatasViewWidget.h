#ifndef SACHARTDATASVIEWWIDGET_H
#define SACHARTDATASVIEWWIDGET_H

#include <QWidget>
class QwtPlotItemTreeModel;
class QwtPlotItemDataModel;
class SAFigureWindow;
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
private:
    Ui::SAChartDatasViewWidget *ui;
    QwtPlotItemTreeModel* m_treeModel;
    QwtPlotItemDataModel* m_tableModel;
    SAFigureWindow* m_figure;
};

#endif // SACHARTDATASVIEWWIDGET_H
