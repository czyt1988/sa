#ifndef SACHARTDATASVIEWWIDGET_H
#define SACHARTDATASVIEWWIDGET_H
#include <QWidget>
#include <QStringList>
#include <QModelIndexList>
class QwtPlotItemTreeModel;
class SAPlotDataModel;
class SAFigureWindow;
class QwtPlot;
class QwtPlotItem;
class QwtPlotCurve;
class QMenu;
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
    SAPlotDataModel* getPlotModel() const;
private slots:
    void onTreeViewCurPlotItemClicked(const QModelIndex& index);
    //figure有图添加发送的信号
    void onChartAdded(QwtPlot* plot);
    //figure销毁
    void onFigureDestroy(QObject* obj);
    //右键菜单
    void onTableViewCustomContextMenuRequested(const QPoint &pos);
    //插入命令
    void onActionInsertTriggered();
    //delete 点击
    void onActionDeleteTriggered();
    //ctrl + v
    void onActionPasteTriggered();
    //ctrl + c
    void onActionCopyTriggered();
private:
    //解析剪切板的数据 返回按照tab分隔解析的字符表的尺寸
    QSize getClipboardTextTable(QList<QStringList> &res);
    //
    void createMenu();
    //根据选中的表格单元，分析所有选中单元格对应的item和这个item对应的单元格索引
    void analysisSelectModelIndex(const QModelIndexList &indexLists, QMap<QwtPlotItem *, QVector<QPoint> > &res);
    //根据选中的表格单元，分析所有选中单元格对应的item和这个item对应的最小行索引
    void analysisSelectModelIndex(const QModelIndexList &indexLists, QMap<QwtPlotItem *, int > &res);
private:
    Ui::SAChartDatasViewWidget *ui;
    QwtPlotItemTreeModel* m_treeModel;
    SAPlotDataModel* m_tableModel;
    SAFigureWindow* m_figure;
    QMenu* m_menu;
    QAction* m_insertAction;
    QAction* m_deleteAction;
    QAction* m_pasteAction;
    QAction* m_copyAction;
};

#endif // SACHARTDATASVIEWWIDGET_H
