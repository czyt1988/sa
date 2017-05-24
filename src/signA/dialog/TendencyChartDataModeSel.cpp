#include "TendencyChartDataModeSel.h"
#include "ui_TendencyChartDataModeSel.h"
#include <QStringList>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QMessageBox>
#include "SAChart2D.h"
#include "qwt_plot_curve.h"
#define ROLE_CUR_PTR Qt::UserRole+1
TendencyChartDataModeSel::TendencyChartDataModeSel(SAChart2D* chart, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TendencyChartDataModeSel)
  ,m_chart(chart)
{
    ui->setupUi(this);
    QList<QwtPlotCurve*> curList = chart->getCurveList();
    QStandardItemModel* model = new QStandardItemModel(ui->listView_curs);
    for(auto i=curList.begin();i!=curList.end();++i)
    {
        auto item = new QStandardItem((*i)->title().text());
        item->setData(reinterpret_cast<quintptr>(*i),ROLE_CUR_PTR);
        model->appendRow(item);
    }
    ui->radioButton_follow->setChecked(true);
    ui->listView_curs->setModel(model);
    ui->listView_curs->setCurrentIndex(model->index(0,0));
}

TendencyChartDataModeSel::~TendencyChartDataModeSel()
{
    delete ui;
}

bool TendencyChartDataModeSel::isFollow() const
{
    return ui->radioButton_follow->isChecked();
}

bool TendencyChartDataModeSel::isSelDef() const
{
    return ui->radioButton_selDef->isChecked();
}

void TendencyChartDataModeSel::getSelDefData(double& start, double& added) const
{
    start = ui->lineEdit_start->text().toDouble();
    added = ui->lineEdit_added->text().toDouble();
}
///
/// \brief 获取需要选择跟随的曲线
/// \return
///
QwtPlotCurve*TendencyChartDataModeSel::getSelFollowCurs() const
{
    QModelIndexList sel = ui->listView_curs->selectionModel()->selectedIndexes();
    if(sel.size()!=1)
        return nullptr;
    quintptr curuint = sel.at(0).data(ROLE_CUR_PTR).value<quintptr>();
    return reinterpret_cast<QwtPlotCurve*>(curuint);
}

void TendencyChartDataModeSel::on_buttonBox_accepted()
{
    if(!(isFollow() | isSelDef()))
    {
        QMessageBox::information(this,tr("information")
                                 ,tr("Please select at least one X axis generating method"));//请至少选择一种x轴生成方式
        return;
    }
    if(isSelDef())
    {
        if(ui->lineEdit_start->text().isEmpty() || ui->lineEdit_added->text().isEmpty())
        {
            QMessageBox::information(this,tr("information")
                                     ,tr("Please set the start and interval values of the coordinate axes to generate the data"));//请至设置坐标轴的起始值及增值，以便生成坐标轴
            return;
        }
    }
    if(isFollow())
    {
        if(ui->listView_curs->selectionModel()->selectedIndexes().size() != 1)
        {
            QMessageBox::information(this,tr("information")
                                     ,tr("Select 1 curves to follow"));//选择1条需要跟随的曲线
            return;
        }
    }
    return QDialog::accept();
}
