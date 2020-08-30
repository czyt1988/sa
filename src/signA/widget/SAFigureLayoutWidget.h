#ifndef SAFIGURELAYOUTWIDGET_H
#define SAFIGURELAYOUTWIDGET_H

#include <QWidget>
#include "SAPlotLayerTreeModel.h"
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
    SAPlotLayerTreeModel *getLayoutModel() const;

    //设置figure
    void setFigure(SAFigureWindow *fig);

    //获取当前的figure
    SAFigureWindow *currentFigure() const;

    //刷新model
    void updateModel();

signals:

    /**
     * @brief 选中了item
     * @param item
     */
    void itemSelected(QwtPlotItem *item);

    /**
     * @brief 选中了图像
     * @param chart
     */
    void chartSelected(SAChart2D *chart);

    ///
    /// \brief 图层修改item的可见性
    /// \param chart 图指针
    /// \param item item指针
    /// \param visible 可见性
    ///
    void itemVisibleChanged(QwtPlotItem *item, bool visible);

    ///
    /// \brief 图层修改item颜色发送的信号
    /// \param chart 图指针
    /// \param item item指针
    /// \param clr 颜色
    ///
    void itemColorChanged(QwtPlotItem *item, QColor clr);

    ///
    /// \brief item删除发送的信号
    /// \param chart
    /// \param item
    ///
    void itemRemoved(SAChart2D *chart, QwtPlotItem *item);

    /**
     * @brief item的描述改变产生的信号
     * @param chart
     * @param item
     * @param title
     */
    void itemTitleChanged(QwtPlotItem *item, const QString& title);

private slots:
    //表格点击
    void onTreeViewPressed(const QModelIndex& index);

    //清除按钮点击
    void onToolButtonDeleteClicked(bool on);

    //模型数据发生
    void onItemValueChanged(QwtPlotItem *plotItem, const QVariant& value, int type, const QModelIndex& index);

    //model 重置
    void onModelReseted();

private:
    Ui::SAFigureLayoutWidget *ui;
    SAPlotLayerTreeModel *m_layoutModel;
    SAFigureWindow *m_figure;
    QModelIndex m_lastPressedIndex;
};

#endif // SAFIGURELAYOUTWIDGET_H
