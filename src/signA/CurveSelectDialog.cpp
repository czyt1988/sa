#include "CurveSelectDialog.h"
#include <QItemSelectionModel>
#include <QMessageBox>
#include <algorithm>
#include "SAChart2D.h"
#include "SASeries.h"
#include "SAChart.h"
#include <QHeaderView>
#define ROLE_PTR Qt::UserRole


void CurveSelectDialog::UI::setupUi(CurveSelectDialog *d)
{
    if (d->objectName().isEmpty())
        d->setObjectName(("CurveSelectDialog"));
    d->resize(587, 383);
    verticalLayout = new QVBoxLayout(d);
    verticalLayout->setObjectName(("verticalLayout"));
    tableView = new QTableView(d);
    tableView->setObjectName(("tableView"));
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSortingEnabled(true);

    verticalLayout->addWidget(tableView);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    pushButton_reject = new QPushButton(d);
    pushButton_reject->setObjectName(("pushButton_reject"));

    horizontalLayout->addWidget(pushButton_reject);

    pushButton_ok = new QPushButton(d);
    pushButton_ok->setObjectName(("pushButton_ok"));

    horizontalLayout->addWidget(pushButton_ok);


    verticalLayout->addLayout(horizontalLayout);


    d->setWindowTitle(QApplication::translate("CurveSelectDialog", "\346\233\262\347\272\277\351\200\211\346\213\251", 0));
    pushButton_reject->setText(QApplication::translate("CurveSelectDialog", "\345\217\226 \346\266\210", 0));
    pushButton_ok->setText(QApplication::translate("CurveSelectDialog", "\347\241\256 \345\256\232", 0));

    QObject::connect(pushButton_reject, SIGNAL(clicked()), d, SLOT(reject()));

    QMetaObject::connectSlotsByName(d);
}

CurveSelectDialog::CurveSelectDialog(SAChart2D *chart , QWidget *parent) :
    QDialog(parent)
	,m_chart(chart)
    ,ui(new CurveSelectDialog::UI)
{
    ui->setupUi(this);

    m_itemFilter= SAChart2D::getPlotItemsRTTI().toSet();

	QStandardItemModel* model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList()
        <<tr("curve name")//曲线名称
        <<tr("color")//颜色
        <<tr("size"));//数据点数
    ui->tableView->setModel(model);
    updateTable();
    QHeaderView* header = ui->tableView->horizontalHeader();
    if(header)
    {
        //header->setSectionResizeMode(0,QHeaderView::ResizeToContents);
        header->setStretchLastSection(true);
    }

}

CurveSelectDialog::~CurveSelectDialog()
{

}
///
/// \brief 设置条目过滤
///
/// 默认有以下会被设置
/// SAPlotItem::Rtti_PlotCurve
/// SAPlotItem::Rtti_PlotSpectroCurve
/// SAPlotItem::Rtti_PlotIntervalCurve
/// SAPlotItem::Rtti_PlotHistogram
/// SAPlotItem::Rtti_PlotBarChart
/// SAPlotItem::Rtti_PlotMultiBarChart
/// SAPlotItem::Rtti_PlotMultiBarChart;
///
void CurveSelectDialog::setItemFilter(const QSet<int> &filters)
{
    m_itemFilter = filters;
    updateTable();
}
///
/// \brief 获取设置的条目过滤
/// \return
///
QSet<int> CurveSelectDialog::getItemFilter() const
{
    return m_itemFilter;
}
///
/// \brief 曲线选择的静态函数，让用户选中XY曲线
/// \param chart 绘图的窗口
/// \param parent 对话框的父窗口
/// \return 返回选中的条目，如果点击了取消，返回一个空的list
///
QList<QwtPlotCurve *> CurveSelectDialog::getSelCurve(SAChart2D *chart, QWidget *par)
{
    QList<QwtPlotCurve *> res;
    CurveSelectDialog dlg(chart,par);
    dlg.setItemFilter({QwtPlotItem::Rtti_PlotCurve,SA::RTTI_SAXYSeries});
    if(QDialog::Accepted == dlg.exec())
    {
        QList<QwtPlotItem*> items = dlg.getSelItem();
        std::for_each(items.begin(),items.end(),[&res](QwtPlotItem* p){
            if(p->rtti() == QwtPlotItem::Rtti_PlotCurve || p->rtti() == SA::RTTI_SAXYSeries)
            {
                res.append(static_cast<QwtPlotCurve*>(p));
            }
        });
    }
    return res;
}

QList<QwtPlotItem *> CurveSelectDialog::getSelectChartPlotItems(SAChart2D *chart, QWidget *par)
{
    QList<QwtPlotItem *> res;
    CurveSelectDialog dlg(chart,par);
    dlg.setItemFilter(SAChart2D::getPlotItemsRTTI().toSet());
    if(QDialog::Accepted == dlg.exec())
    {
        QList<QwtPlotItem*> items = dlg.getSelItem();
        std::for_each(items.begin(),items.end(),[&res](QwtPlotItem* p){
            if(SAChart::dynamicCheckIsPlotChartItem(p))
            {
                res.append(p);
            }
        });
    }
    return res;
}

///
/// \brief 根据设定的filter显示条目
///
void CurveSelectDialog::updateTable()
{
    QStandardItemModel * model = getTableModel();
    if(nullptr == model)
    {
        return;
    }
    model->clear();
    QList<QwtPlotItem *>  items = getFilterItems();
    int i = 0;
    std::for_each(items.begin(),items.end(),[&](QwtPlotItem *plotItem){
        QStandardItem* item = nullptr;
        item = new QStandardItem(plotItem->title().text());
        item->setData(QVariant::fromValue((quintptr)plotItem),ROLE_PTR);
        model->setItem(i,0,item);

        item = new QStandardItem;
        item->setData(SAChart2D::getItemColor(plotItem),Qt::BackgroundRole);
        model->setItem(i,1,item);

        int size = SAChart2D::getItemDataSize(plotItem);
        if(size > 0)
        {
            item = new QStandardItem(QString("%1").arg(size));
        }
        else
        {
            item = new QStandardItem();
        }
        model->setItem(i,2,item);
        ++i;
    });
}

QStandardItemModel* CurveSelectDialog::getTableModel()
{
    return static_cast<QStandardItemModel*>(ui->tableView->model());
}
///
/// \brief 根据过滤条件获取items
/// \return
///
QList<QwtPlotItem *> CurveSelectDialog::getFilterItems()
{
    QList<QwtPlotItem*> items = m_chart->itemList();
    QList<QwtPlotItem*> filterItem;
    std::for_each(items.begin(),items.end(),[&](QwtPlotItem* p){
        if(m_itemFilter.contains(p->rtti()))
        {
            filterItem.append(p);
        }
    });
    return filterItem;
}


void CurveSelectDialog::on_pushButton_ok_clicked()
{
	QItemSelectionModel *sel = ui->tableView->selectionModel();
	QModelIndexList indexList = sel->selectedRows();
    QStandardItemModel* model = getTableModel();
	if (indexList.size()<=0)
	{
        QMessageBox::information(this,tr("information"),tr("please select curve"));//请选择曲线
		return;
	}
    m_selItem.clear();

	for(int i=0;i<indexList.size();++i)
	{
        QStandardItem* item = model->item(indexList[i].row(),0);
        if(item)
        {
            quintptr p = item->data(ROLE_PTR).value<quintptr>();
            m_selItem.append((QwtPlotItem*)p);
        }
	}
	QDialog::accept();
}
