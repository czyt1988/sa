#pragma once
#include <QAbstractTableModel>
#include <qwt_plot.h>
#include <QIcon>
class QwtPlotItem;
class SAPlotLayerModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	SAPlotLayerModel(QObject * parent = 0);
	~SAPlotLayerModel();

	virtual int rowCount(const QModelIndex &parent) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	void setPlot(QwtPlot* plot);
	void updateModel();
    QwtPlotItem* getPlotItemFromIndex(const QModelIndex &index);
private slots:
	void itemAttached (QwtPlotItem *plotItem, bool on);
private:
    //item的颜色设置
	QVariant getColorFromItem(const QwtPlotItem* item,int alpha=255) const;
    void setColorForItem(QwtPlotItem* item,QColor clr);
    //item的文字设置
    QVariant getTextFromItem(const QwtPlotItem* item) const;
    void setTextForItem(QwtPlotItem* item,const QString& text);


	QVariant getIconFromItem(const QwtPlotItem* item) const;
	QVariant getVisibleIconFromItem(const QwtPlotItem* item) const;
	QVariant displayDataRole(const QModelIndex &index,const QwtPlotItem* plotItem) const;
	QVariant displayDecorationRole(const QModelIndex &index,const QwtPlotItem* plotItem) const;
	QwtPlot* m_plot;
    short m_alpha;
};

