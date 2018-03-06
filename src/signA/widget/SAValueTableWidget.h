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
protected:
    //获取model
    SADataTableModel* getDataModel() const;
private slots:
    //右键菜单
    void onTableViewCustomContextMenuRequested(const QPoint &pos);
    //选择的列转换为向量
    void onActionToLinerDataTriggered();
    //选择的列转换为点序列
    void onActionToPointFVectorDataTriggered();
    //表格双击 进入编辑模式
    void onTableViewDoubleClicked(const QModelIndex& index);
    //数据删除触发的槽
    void onDataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
private:
    bool setData(int r,int c,const QVariant& v);
    void onTableViewCtrlV();
    void getSelectLinerData(QHash<int, QVector<double> >& rawData) const;
    void getSelectVectorPointData(QVector< std::shared_ptr<QVector<QPointF> > > &rawData,int dim = 0);
    bool getSelectVectorPointData(SAVectorPointF* data);
    void appendDataFromVariant(const QVariant& var,QVector<double>& data) const;
    //获取选择的列值
    static void getItemSelectionColumns(QItemSelectionModel* selModel
                                         ,QMap<int,std::shared_ptr<QVector<QVariant> > >& res);
    void wheelEvent(QWheelEvent * event);
    //解析剪切板的数据 返回按照tab分隔解析的字符表的尺寸
    QSize getClipboardTextTable(QVector<QStringList> &res);
private:
    Ui::SAValueTableWidget *ui;
    //OpenFileManager* m_values;
    uint m_countNewData;
    QMenu* m_menu;
    QUndoStack* m_undoStack;
};

#endif // VALUEVIEWERTABPAGE_H
