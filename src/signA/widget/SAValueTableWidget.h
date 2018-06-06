#ifndef SAVALUTABLEWIDGET_H
#define SAVALUTABLEWIDGET_H

#include <QWidget>
#include <SAData.h>
#include <QList>
#include <QHash>
#include <memory>
#include <QMenu>
#include <QItemSelectionModel>
#include <QUndoStack>
namespace Ui {
class SAValueTableWidget;
}

class SADataTableModel;
class QTableView;
class QWheelEvent;
class SACellInputWidget;


///
/// \brief sa的表格窗体
///
class SAValueTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAValueTableWidget(QWidget *parent = 0);
    ~SAValueTableWidget();
    QTableView* getTableView();
    //设置数据
    void setSADataPtr(SAAbstractDatas* data);
    void setSADataPtrs(const QList<SAAbstractDatas*>& datas);
    //插入数据
    void appendSADataPtr(SAAbstractDatas* data);
    void appendSADataPtrs(QList<SAAbstractDatas*> datas);
    //移除显示的数据
    void removeDatas(const QList<SAAbstractDatas*>& datas);
    //把表格保存到csv
    void saveTableToCsv(const QString &fullFilePath);
    //redo / undo
    void redo();
    void undo();
protected:
    //获取model
    SADataTableModel* getDataModel() const;
    void wheelEvent(QWheelEvent * event);
    //void keyPressEvent(QKeyEvent *e);
private slots:
    //右键菜单
    void onTableViewCustomContextMenuRequested(const QPoint &pos);
    //选择的列转换为向量
    void onActionToLinerDataTriggered();
    //选择的列转换为点序列
    void onActionToPointFVectorDataTriggered();
    //数据删除触发的槽
    void onDataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    //插入命令
    void onActionInsertTriggered();
    //delete 点击
    void onActionDeleteTriggered();
    //ctrl + v
    void onActionPasteTriggered();
private:
    //输入数据
    bool onTableViewSetData(int r,int c,const QVariant& v);

    //获取选中的线形数据
    void getSelectLinerData(QHash<int, QVector<double> >& rawData) const;
    //获取选择的点集
    void getSelectVectorPointData(QVector< std::shared_ptr<QVector<QPointF> > > &rawData,int dim = 0);
    bool getSelectVectorPointData(SAVectorPointF* data);
    //获取选择的列值
    static void getItemSelectionColumns(QItemSelectionModel* selModel
                                         ,QMap<int,std::shared_ptr<QVector<QVariant> > >& res);
    //解析剪切板的数据 返回按照tab分隔解析的字符表的尺寸
    QSize getClipboardTextTable(QList<QVariantList> &res);
    //
    static void doubleVectorAppendFromVariant(const QVariant& var,QVector<double>& data);
    //获取表格的表头的尺寸，返回一个QPoint，x:为竖直表头宽度，y为水平表头高度
    QPoint tableHeaderPositionOffset() const;
    //
    void createMenu();
private:
    Ui::SAValueTableWidget *ui;
    //OpenFileManager* m_values;
    uint m_countNewData;
    QMenu* m_menu;
    QAction* m_undo;
    QAction* m_redo;
    QAction* m_insertAction;
    QAction* m_deleteAction;
    QAction* m_pasteAction;
    QUndoStack* m_undoStack;
};

#endif // VALUEVIEWERTABPAGE_H
