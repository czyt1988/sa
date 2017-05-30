#include "SAPlotLayerModel.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_barchart.h>
#include <QColorDialog>
//#include <SAChart.h>
#include <SAPlotMarker.h>
#include "SAResourDefine.h"

#define COLUMN_COUNT 4
SAPlotLayerModel::SAPlotLayerModel(QObject * parent):QAbstractTableModel(parent)
	,m_plot(nullptr)
    ,m_alpha(30)	
{
}


SAPlotLayerModel::~SAPlotLayerModel(void)
{
}

void SAPlotLayerModel::setPlot(QwtPlot* plot)
{
	beginResetModel();
	m_plot = plot;
	connect(m_plot,&QwtPlot::itemAttached,this,&SAPlotLayerModel::itemAttached);
	endResetModel();
}

void SAPlotLayerModel::itemAttached (QwtPlotItem *plotItem, bool on)
{
	updateModel();
}

void SAPlotLayerModel::updateModel()
{
	beginResetModel();
    endResetModel();
}


int SAPlotLayerModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_plot ? m_plot->itemList().count() : 0;
}
int SAPlotLayerModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return COLUMN_COUNT;
}
QVariant SAPlotLayerModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	if(Qt::Horizontal == orientation)
	{//说明是水平表头
		switch (section)
		{
		case 0:
			return QStringLiteral("类型");
		case 1:
			return QStringLiteral("可见性");
		case 2:
			return QStringLiteral("颜色");
		case 3:
			return QStringLiteral("描述");
		default:
			break;
		}
		return QVariant();
	}
	return QVariant();
}
Qt::ItemFlags SAPlotLayerModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::NoItemFlags;
    if(index.column() == 3 || index.column() == 2)//描述和颜色可以改变内容
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
}
QVariant SAPlotLayerModel::data(const QModelIndex &index, int role) const
{
	if(!m_plot)
		return QVariant();
	QwtPlotItemList itemList = m_plot->itemList();
	int layIndex = itemList.size()-index.row() - 1;
	if(layIndex > itemList.size() || layIndex<0)
		return QVariant();
	
	if (role == Qt::TextAlignmentRole)
	{ //返回的是对其方式
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	}
	else if (role == Qt::DisplayRole) //返回的是现实内容
	{
		return displayDataRole(index,itemList[layIndex]);
		//return itemList[i]->title().text();
	}
	else if (role == Qt::DecorationRole)
	{
		return displayDecorationRole(index,itemList[layIndex]);
	}
	else if(role == Qt::BackgroundRole)
	{
		if(2 == index.column())
			return getColorFromItem(itemList[layIndex]);
		return getColorFromItem(itemList[layIndex],30);
	}

	return QVariant();
}

QVariant SAPlotLayerModel::displayDataRole(const QModelIndex &index,const QwtPlotItem* plotItem) const
{
	switch (index.column())
	{
	case 0:
	case 1: 	
	case 2:
		return QVariant();
	case 3:
		return plotItem->title().text();
	default:
		break;
	}
	return QVariant();
}

QVariant SAPlotLayerModel::displayDecorationRole(const QModelIndex &index,const QwtPlotItem* plotItem) const
{
	switch (index.column())
	{
	case 0:
		return getIconFromItem(plotItem);
	case 1:
		return getVisibleIconFromItem(plotItem);
	case 2:
	case 3: 
		return QVariant();
	
	default:
		break;
	}
	return QVariant();
}

bool SAPlotLayerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_plot)
        return false;
    beginResetModel();
    QwtPlotItem* item = getPlotItemFromIndex(index);
    if(!item)
        return false;
    if(index.column() == 1)
    {//颜色
        bool show = value.toBool ();
        item->setVisible (show);
        return true;
    }
    else if(index.column() == 2)
    {//颜色
        if (!value.canConvert<QColor>())
            return false;
        QColor clr = value.value<QColor>();
        setColorForItem(item,clr);
        return true;
    }
    else if(index.column() == 3)
    {//描述
        if (value.toString ().isEmpty ())
            return false;
        setTextForItem(item,value.toString ());
    }
    endResetModel();
	return false;
}

QwtPlotItem*SAPlotLayerModel::getPlotItemFromIndex(const QModelIndex& index)
{
    QwtPlotItemList itemList = m_plot->itemList();
    int layIndex = itemList.size()-index.row() - 1;
    if(layIndex > itemList.size() || layIndex<0)
        return nullptr;
    return itemList[layIndex];
}

QVariant SAPlotLayerModel::getColorFromItem(const QwtPlotItem* item,int alpha) const
{
	const int rtti = item->rtti();
	QColor c;
	switch (rtti)
	{
	case QwtPlotItem::Rtti_PlotCurve :
		c = static_cast<const QwtPlotCurve*>(item)->pen().color();
		break;
	case QwtPlotItem::Rtti_PlotGrid:
		c = static_cast<const QwtPlotGrid*>(item)->majorPen().color();
		break;
    case SAAbstractPlotMarker::Rtti_SAPointMarker:
    case SAAbstractPlotMarker::Rtti_SAYValueMarker:
	case QwtPlotItem::Rtti_PlotMarker:
		c = static_cast<const QwtPlotMarker*>(item)->linePen ().color();
    case QwtPlotItem::Rtti_PlotBarChart://bar没有明确颜色
        return QVariant();
	default:
		return QVariant();
	}
	if(alpha<255)
		c.setAlpha(alpha);
    return c;
}

void SAPlotLayerModel::setColorForItem(QwtPlotItem *item, QColor clr)
{
    const int rtti = item->rtti();
    switch (rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve :
    {
        QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);
        QPen pen = p->pen();
        pen.setColor(clr);
        p->setPen(pen);
    }break;
    case QwtPlotItem::Rtti_PlotGrid:
    {
        QwtPlotGrid* p = static_cast<QwtPlotGrid*>(item);
        QPen pen = p->majorPen();
        pen.setColor(clr);
        p->setMajorPen(pen);
    }break;
    case QwtPlotItem::Rtti_PlotMarker:
    {
        QwtPlotMarker* p = static_cast<QwtPlotMarker*>(item);
        QPen pen = p->linePen();
        pen.setColor(clr);
        p->setLinePen(pen);
    }break;
    default:
        return;
    }
}

QVariant SAPlotLayerModel::getTextFromItem(const QwtPlotItem* item) const
{
    return item->title ().text ();
}

void SAPlotLayerModel::setTextForItem(QwtPlotItem* item, const QString& text)
{
    item->setTitle (text);
}


QVariant SAPlotLayerModel::getIconFromItem(const QwtPlotItem* item) const
{
	const int rtti = item->rtti();
	switch (rtti)
	{
	case QwtPlotItem::Rtti_PlotGrid:
        return ICON_LayeroutGrid;
	case QwtPlotItem::Rtti_PlotCurve :
        return ICON_LayeroutCurve;
    case QwtPlotItem::Rtti_PlotBarChart:
        return ICON_LayoutBarChart;
	case QwtPlotItem::Rtti_PlotLegend:
        return ICON_LayoutLegend;
	case QwtPlotItem::Rtti_PlotMarker:
        return ICON_LayoutMark;
    case SAAbstractPlotMarker::Rtti_SAPointMarker:
        return ICON_LayoutPointMarker;
    case SAAbstractPlotMarker::Rtti_SAYValueMarker:
        return ICON_LayoutYValueMarker;
	}
	return QVariant();
}

QVariant SAPlotLayerModel::getVisibleIconFromItem(const QwtPlotItem* item) const
{
	if(item->isVisible())
        return ICON_LayoutVisible;
    return ICON_LayoutUnvisible;
}
