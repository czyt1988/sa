#include "SAFigureWindowChartTableModel.h"
#include "SAChart2D.h"
#define COL_COUNT    5

class SAFigureWindowChartTableModelPrivate
{
    SA_IMPL_PUBLIC(SAFigureWindowChartTableModel)
public:
    SAFigureWindowChartTableModelPrivate(SAFigureWindowChartTableModel *p)
        : q_ptr(p)
        , mFigure(nullptr)
    {
    }


    SAFigureWindow *mFigure;
};

SAFigureWindowChartTableModel::SAFigureWindowChartTableModel(QObject *par)
    : QAbstractTableModel(par)
    , d_ptr(new SAFigureWindowChartTableModelPrivate(this))
{
}


SAFigureWindowChartTableModel::~SAFigureWindowChartTableModel()
{
}


void SAFigureWindowChartTableModel::setFigure(SAFigureWindow *fig)
{
    if (d_ptr->mFigure == fig) {
        return;
    }
    beginResetModel();
    d_ptr->mFigure = fig;
    endResetModel();
}


void SAFigureWindowChartTableModel::refresh()
{
    beginResetModel();
    endResetModel();
}


int SAFigureWindowChartTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    if (nullptr == d_ptr->mFigure) {
        return (0);
    }
    qDebug() << "rowCount" <<  d_ptr->mFigure->get2DPlots().size();
    return (d_ptr->mFigure->get2DPlots().size());
}


int SAFigureWindowChartTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return (COL_COUNT);
}


QVariant SAFigureWindowChartTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return (QVariant());
    }
    if (Qt::Horizontal == orientation) {//说明是水平表头
        switch (section)
        {
        case 0:
            return (tr("name"));

        case 1:
            return (tr("x"));

        case 2:
            return (tr("y"));

        case 3:
            return (tr("w"));

        case 4:
            return (tr("h"));

        default:
            break;
        }
    }
    return (QVariant());
}


QVariant SAFigureWindowChartTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return (QVariant());
    }
    if (nullptr == d_ptr->mFigure) {
        return (QVariant());
    }

    if (role == Qt::TextAlignmentRole) {    //返回的是对其方式
        return (int(Qt::AlignRight | Qt::AlignVCenter));
    }else if (role == Qt::DisplayRole) {    //返回的是显示内容
        QList<SAChart2D *> charts = d_ptr->mFigure->get2DPlots();
        int col = index.column();
        int row = index.row();
        if (row >= charts.size()) {
            return (QVariant());
        }
        SAChart2D *chart = charts[row];
        QRectF pos = d_ptr->mFigure->getWidgetPosPercent(chart);
        switch (col)
        {
        case 0:
            return (chart->title().text());

        case 1:
            return (tr("%1").arg(pos.x()));

        case 2:
            //把其转换为正常笛卡尔坐标
            return (tr("%1").arg(pos.y()));

        case 3:
            return (tr("%1").arg(pos.width()));

        case 4:
            return (tr("%1").arg(pos.height()));

        default:
            break;
        }
    }
    return (QVariant());
}


bool SAFigureWindowChartTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || (nullptr == d_ptr->mFigure)) {
        return (false);
    }
    if (role == Qt::EditRole) {
        QList<SAChart2D *> charts = d_ptr->mFigure->get2DPlots();
        int col = index.column();
        int row = index.row();
        if (row >= charts.size()) {
            return (false);
        }
        SAChart2D *chart = charts[row];
        switch (col)
        {
        case 0:
            chart->setTitle(value.toString());
            return (true);

        case 1:
        {
            bool isOK = false;
            qreal x = value.toReal(&isOK);
            if (!isOK) {
                return (false);
            }
            QRectF pos = d_ptr->mFigure->getWidgetPosPercent(chart);
            pos.setX(x);
            d_ptr->mFigure->setWidgetPosPercent(chart, pos);
            return (true);
        }

        case 2:
        {
            bool isOK = false;
            qreal y = value.toReal(&isOK);
            if (!isOK) {
                return (false);
            }
            QRectF pos = d_ptr->mFigure->getWidgetPosPercent(chart);
            pos.setY(y);
            d_ptr->mFigure->setWidgetPosPercent(chart, pos);
            return (true);
        }

        case 3:
        {
            bool isOK = false;
            qreal w = value.toReal(&isOK);
            if (!isOK) {
                return (false);
            }
            QRectF pos = d_ptr->mFigure->getWidgetPosPercent(chart);
            pos.setWidth(w);
            d_ptr->mFigure->setWidgetPosPercent(chart, pos);
            return (true);
        }

        case 4:
        {
            bool isOK = false;
            qreal h = value.toReal(&isOK);
            if (!isOK) {
                return (false);
            }
            QRectF pos = d_ptr->mFigure->getWidgetPosPercent(chart);
            pos.setHeight(h);
            d_ptr->mFigure->setWidgetPosPercent(chart, pos);
            return (true);
        }

        default:
            break;
        }
    }
    return (false);
}


Qt::ItemFlags SAFigureWindowChartTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return (Qt::NoItemFlags);
    }
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
}
