#include "ReportSetDialog.h"
#include "ui_ReportSetDialog.h"
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include "SAFigureWindow.h"
#define ROLE_WIDGET Qt::UserRole+1
ReportSetDialog::ReportSetDialog(QMdiArea *mdi, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportSetDialog),
    m_mdi(mdi)
{
    ui->setupUi(this);
    QStandardItemModel* model = new QStandardItemModel(ui->listView);
    auto subWnds = mdi->subWindowList();
    std::for_each(subWnds.begin(),subWnds.end()
                  ,[&](QMdiSubWindow* subW){
        SAFigureWindow* scw = qobject_cast<SAFigureWindow*>(subW->widget());
        if(scw != nullptr)
        {
            m_report_set.appendDefaultChartReportSet(scw);//自动插入一个设置
            //listView生成一个条目
            QStandardItem* item = new QStandardItem(subW->windowTitle());
            item->setData(QVariant::fromValue(scw),ROLE_WIDGET);
            model->appendRow(item);
        }
    });
    ui->listView->setModel(model);

    connect(ui->groupBox_value,&QGroupBox::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->groupBox_point,&QGroupBox::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->groupBox_points,&QGroupBox::clicked,this,&ReportSetDialog::on_checkBoxClick);

    connect(ui->checkBox_nums,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_sum,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_minValue,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_maxValue,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_mean,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_var,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_std,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_skewness,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_kurtosis,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);
    connect(ui->checkBox_peakPeakValue,&QAbstractButton::clicked,this,&ReportSetDialog::on_checkBoxClick);

    ui->listView->setCurrentIndex(model->index(0,0));
}

ReportSetDialog::~ReportSetDialog()
{
    delete ui;
}



void ReportSetDialog::on_listView_clicked(const QModelIndex &index)
{
    QVariant var = index.data(ROLE_WIDGET);
    if(var.isValid())
    {
        ChartReportSet crs = m_report_set.getChartReportSetByWidgetPtr(
                    var.value<SAChartWidget*>());
        showChartReportSetInUI(crs);
    }
}
void ReportSetDialog::showChartReportSetInUI(const ChartReportSet& res)
{
    ui->groupBox_value->setChecked(res.isShowCharacteristicValue);
    ui->checkBox_nums->setChecked(res.isShowPointsCount);
    ui->checkBox_sum->setChecked(res.isShowSum);
    ui->checkBox_minValue->setChecked(res.isShowMinValue);
    ui->checkBox_maxValue->setChecked(res.isShowMaxValue);
    ui->checkBox_mean->setChecked(res.isShowMean);
    ui->checkBox_var->setChecked(res.isShowVar);
    ui->checkBox_std->setChecked(res.isShowStd);
    ui->checkBox_skewness->setChecked(res.isShowSkewness);
    ui->checkBox_kurtosis->setChecked(res.isShowKurtosis);
    ui->checkBox_peakPeakValue->setChecked(res.isShowPeakPeakValue);

    ui->groupBox_point->setChecked(res.isShowCharacteristicPoint);
    ui->checkBox_minPoint->setChecked(res.isShowMinPoint);
    ui->checkBox_maxPoint->setChecked(res.isShowMaxPoint);

    ui->groupBox_points->setChecked(res.isShowCharacteristicPointsArray);
    ui->checkBox_top->setChecked(res.isShowTop);
    ui->checkBox_low->setChecked(res.isShowLow);

    ui->lineEdit_top->setText(QString::number(res.topCount));
    ui->lineEdit_low->setText(QString::number(res.lowCount));
}



void ReportSetDialog::on_checkBoxClick(bool check)
{
    if(!ui->listView->currentIndex().isValid())
        return;
    QVariant var = ui->listView->currentIndex().data(ROLE_WIDGET);
    if(var.isValid())
    {
        ChartReportSet& crs = m_report_set.getChartReportSetByWidgetPtr(
                    var.value<SAChartWidget*>());
        QCheckBox* btn = dynamic_cast<QCheckBox*>(sender());
        if (btn)
        {
            QString obj = btn->objectName();

            if(obj == QString("checkBox_nums"))
            {
                crs.isShowPointsCount = check;
            }
            else if(obj == QString("checkBox_sum"))
            {
                crs.isShowSum = check;
            }
            else if(obj == QString("checkBox_minValue"))
            {
                crs.isShowMinValue = check;
            }
            else if(obj == QString("checkBox_maxValue"))
            {
                crs.isShowMaxValue = check;
            }
            else if(obj == QString("checkBox_mean"))
            {
                crs.isShowMean = check;
            }
            else if(obj == QString("checkBox_var"))
            {
                crs.isShowVar = check;
            }
            else if(obj == QString("checkBox_std"))
            {
                crs.isShowStd = check;
            }
            else if(obj == QString("checkBox_skewness"))
            {
                crs.isShowSkewness = check;
            }
            else if(obj == QString("checkBox_kurtosis"))
            {
                crs.isShowKurtosis = check;
            }
            else if(obj == QString("checkBox_peakPeakValue"))
            {
                crs.isShowPeakPeakValue = check;
            }
            else if(obj == QString("checkBox_maxPoint"))
            {
                crs.isShowMaxPoint = check;
            }
            else if(obj == QString("checkBox_minPoint"))
            {
                crs.isShowMinPoint = check;
            }
            else if(obj == QString("checkBox_top"))
            {
                crs.isShowTop = check;
            }
            else if(obj == QString("checkBox_low"))
            {
                crs.isShowLow = check;
            }
            return;
        }
        QGroupBox* gp = dynamic_cast<QGroupBox*>(sender());
        if(gp)
        {
            QString obj = gp->objectName();
            if(obj == QString("groupBox_value"))
            {
                crs.isShowCharacteristicValue = check;
            }
            else if(obj == QString("groupBox_points"))
            {
                crs.isShowCharacteristicPointsArray = check;
            }
            else if(obj == QString("groupBox_point"))
            {
                crs.isShowCharacteristicPoint = check;
            }
        }
    }
}

void ReportSetDialog::on_lineEdit_low_editingFinished()
{
    if(!ui->listView->currentIndex().isValid())
        return;
    QVariant var = ui->listView->currentIndex().data(ROLE_WIDGET);
    if(var.isValid())
    {
        ChartReportSet& crs = m_report_set.getChartReportSetByWidgetPtr(
                    var.value<SAChartWidget*>());
        crs.lowCount = ui->lineEdit_low->text().toInt();
    }
}



void ReportSetDialog::on_lineEdit_top_editingFinished()
{
    if(!ui->listView->currentIndex().isValid())
        return;
    QVariant var = ui->listView->currentIndex().data(ROLE_WIDGET);
    if(var.isValid())
    {
        ChartReportSet& crs = m_report_set.getChartReportSetByWidgetPtr(
                    var.value<SAChartWidget*>());
        crs.topCount = ui->lineEdit_top->text().toInt();
    }
}

void ReportSetDialog::on_pushButton_ok_clicked()
{
    m_report_set.title = ui->lineEdit_title->text();
    m_report_set.extraMessage = ui->textEdit_additional->toPlainText();
}
