#pragma once
#include <QAbstractTableModel>
#include <qwt_plot.h>
#include <QIcon>
class QwtPlotItem;

/**
 * @brief 绘图模型
 */
class SAPlotLayerModel : public QAbstractTableModel
{
	Q_OBJECT
public:
    SAPlotLayerModel(QObject *parent = 0);
	~SAPlotLayerModel();

    /**
     * @brief 定义item的值类型
     */
    enum ItemValueType {
        ItemVisible	= 0,    ///< item的可见性设置
        ItemColor	= 1,    ///< item的颜色
        ItemTitle               ///< item的label
    };

    virtual int rowCount(const QModelIndex& parent) const;
    virtual int columnCount(const QModelIndex& parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    void setPlot(QwtPlot *plot);
    QwtPlot *getPlot() const;
	void updateModel();
    QwtPlotItem *getPlotItemFromIndex(const QModelIndex& index);
    QModelIndexList getIndexFromPlotItems(const QList<QwtPlotItem *>& plotItem);

signals:

    /**
     * @brief item值被设置后出发的信号
     * @param plotItem
     * @param value
     * @param type 类型，@sa  ItemValueType
     * @param index
     */
    void itemValueChanged(QwtPlotItem *plotItem, const QVariant& value, ItemValueType type, const QModelIndex& index);

private slots:
    void itemAttached(QwtPlotItem *plotItem, bool on);

private:
    //item的颜色设置
    QVariant getColorFromItem(const QwtPlotItem *item, int alpha = 255) const;
    void setColorForItem(QwtPlotItem *item, QColor clr);

    //item的文字设置
    QVariant getTextFromItem(const QwtPlotItem *item) const;
    void setTextForItem(QwtPlotItem *item, const QString& text);


    QVariant getIconFromItem(const QwtPlotItem *item) const;
    QVariant getVisibleIconFromItem(const QwtPlotItem *item) const;
    QVariant displayDataRole(const QModelIndex& index, const QwtPlotItem *plotItem) const;
    QVariant displayDecorationRole(const QModelIndex& index, const QwtPlotItem *plotItem) const;

    QwtPlot *m_plot;
    short m_alpha;
};
