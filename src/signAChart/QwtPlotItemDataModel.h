#ifndef QWTPLOTITEMDATAMODEL_H
#define QWTPLOTITEMDATAMODEL_H
#include "SAChartGlobals.h"
#include <QAbstractTableModel>
#include <QList>
#include <algorithm>
#include <vector>
#include <qwt_plot_item.h>
#include <limits>
class QwtPlotMultiBarChart;
class QwtPlotItemDataModelPrivate;
#define QwtPlotItemDataModel_Use_Dynamic_Cast 0
///
/// \brief 显示item数据的tablemodel
///
class SA_CHART_EXPORT QwtPlotItemDataModel : public QAbstractTableModel
{
    Q_OBJECT
    SA_IMPL(QwtPlotItemDataModel)
public:
    QwtPlotItemDataModel(QObject *p = 0);
    ~QwtPlotItemDataModel();
    //给model设置item
    void setPlotItems(const QList<QwtPlotItem*>& items);
    //清空所有内容
    void clear();
    //允许显示背景色
    void enableBackgroundColor(bool enable, int alpha = 30);
    //nan的数值
    static double nan();
    //根据表格的列号获取对应的QwtPlotItem
    QwtPlotItem* getItemFromCol(int col, int *dataColumnDim = nullptr) const;
    //获取这个item对应的表格颜色
    QColor getItemColor(QwtPlotItem* item) const;
    //获取item对应的行数
    int getItemRowCount(QwtPlotItem* item) const;
    //获取item对应的第一个列号
    int getItemsColumnStartIndex(QwtPlotItem* item) const;
    //获取item对应的列范围
    void getItemColumnRange(QwtPlotItem* item,int* startCol=nullptr,int* endCol=nullptr) const;
    //计算QwtPlotMultiBarChart的最大维度
    static int calcPlotMultiBarChartDim(const QwtPlotMultiBarChart* p);
    //获取绘图数据维度的描述
    virtual QString getItemDimDescribe(QwtPlotItem* item,int index) const;
    //通过列号获取item的名字
    QString getItemNameFromCol(int col) const;
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
protected:
    //获取item的最大行数
    virtual int calcItemDataRowCount(QwtPlotItem* item) const;
    //获取item的最大列数
    virtual int calcItemDataColumnCount(QwtPlotItem* item) const;
    //获取数据 row col 要对应item的维度 ,col是以数据为基准而不是以表格为基准
    virtual double getItemData(int row,int col,QwtPlotItem* item) const;
    //设置数据 row col 要对应item的维度
    virtual bool setItemData(int row,int col,QwtPlotItem* item,const QVariant& var);
    //更新最大行数
    void updateRowCount();
    //更新列数
    void updateColumnCount();
    //更新颜色
    void updateItemColor();
};

#endif // QWTPLOTITEMDATAMODEL_H
