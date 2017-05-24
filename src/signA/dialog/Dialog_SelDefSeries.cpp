#include "Dialog_SelDefSeries.h"
#include "ui_Dialog_SelDefSeries.h"
#include <QStandardItemModel>
#include <QDebug>
Dialog_SelDefSeries::Dialog_SelDefSeries(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SelDefSeries)
	,m_size(100)
	,m_detal(1)
	,m_start(0)
{
    ui->setupUi(this);
	connect(ui->doubleSpinBox_Start
		,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
		,this,&Dialog_SelDefSeries::valueChanged_start);
	connect(ui->doubleSpinBox_Detal
		,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
		,this,&Dialog_SelDefSeries::valueChanged_detal);
	refleshView();
}

Dialog_SelDefSeries::~Dialog_SelDefSeries()
{
    delete ui;
}

void Dialog_SelDefSeries::valueChanged_start(double d)
{
	m_start = d;
	refleshView();
}
void Dialog_SelDefSeries::valueChanged_detal(double d)
{
	m_detal = d;
	refleshView();
}

void Dialog_SelDefSeries::refleshView()
{
	m_start = ui->doubleSpinBox_Start->value();
	m_detal = ui->doubleSpinBox_Detal->value();
	QStandardItemModel* model= nullptr;
	model = static_cast<QStandardItemModel*>(ui->tableView->model());
	if (model == nullptr)
	{
		model = new QStandardItemModel(this);
        model->setHorizontalHeaderLabels(QStringList()<<QString(""));
		model->setRowCount(m_size);
		model->setColumnCount(1);
	}
	else
	{
		ui->tableView->setModel(nullptr);
	}
	for (size_t i=0;i<m_size;++i)
	{
		model->setData(model->index(i,0),m_start+i*m_detal);
	}
	ui->tableView->setModel(model);
}
