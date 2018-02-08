#include "Dialog_AddChart.h"
#include "ui_Dialog_AddChart.h"
#include <memory>
#include <qmessagebox.h>
#include <algorithm>
#include "czyQArrayEx.h"
#include <qwt_date_scale_draw.h>
#include "Dialog_SelDefSeries.h"

#include "SAData.h"
#include "SAValueManager.h"
#include "SAValueManagerModel.h"
#include "SAChart2D.h"

#ifndef Dialog_AddChart_TREE_ITEM_ROLE_CURVE_PTR
#define Dialog_AddChart_TREE_ITEM_ROLE_CURVE_PTR (Qt::UserRole + 1011)
#endif

#define ROLE_DATA_ID (Qt::UserRole + 1)

Dialog_AddChart::Dialog_AddChart(QWidget *parent) :
    QDialog(parent)
		,ui(new Ui::Dialog_AddChart)
		,m_lineCount(0)
		,m_start(0)
		,m_detal(0)
{
    ui->setupUi(this);
    SAValueManagerModel* model = new SAValueManagerModel(this);
    ui->treeView->setModel(model);

    m_comboModel = new QStandardItemModel(this);
    QList<SAAbstractDatas*> dataPtrs = saValueManager->getVectorDataPtrs();//m_openFile->getLinerDataPtrs ();
    for(int i=0;i<dataPtrs.size ();++i)
    {
        SAAbstractDatas* data = dataPtrs[i];
        QStandardItem* item = new QStandardItem(SAValueManager::getDataIcon(data->getType())
                                                ,data->getName());
        item->setData(data->getID(),ROLE_DATA_ID);
        m_comboModel->appendRow (item);
    }
    ui->comboBox_X->setModel(m_comboModel);
    ui->comboBox_Y->setModel(m_comboModel);

	m_pVarManager_NotEdit = new QtVariantPropertyManager(ui->propertyTree);
	QStandardItemModel* tableModel = new QStandardItemModel(ui->tableView);

	tableModel->setHorizontalHeaderLabels(QStringList()
        <<tr("series name")//序列名称
        <<tr("series group")//序列组合
        <<tr("data type")//数据类型
        <<tr("data nums")//数据点数
        <<tr("color")//颜色
		);
	ui->tableView->setModel(tableModel);

    ui->chart->setMinimumHeight(10);
    ui->chart->setMinimumWidth(10);
	ui->chart->enableGrid(true);
    ui->chart->setTitle(tr("chart"));//示意图
    ui->chart->setAxisTitle(QwtPlot::xBottom,QString("X"));
    ui->chart->setAxisTitle(QwtPlot::yLeft,QString("Y"));
	ui_setCombox_TimeFormat(ui->comboBox_TX);
	ui_setCombox_TimeFormat(ui->comboBox_TY);
}

Dialog_AddChart::~Dialog_AddChart()
{
    delete ui;
}

void Dialog_AddChart::ui_setCombox_TimeFormat(QComboBox* comb)
{
	comb->clear();
    comb->addItem(QString("h:m (2:21)"),QVariant(QString("h:m")));
    comb->addItem(QString("hh:mm (02:21)"),QVariant(QString("hh:mm")));
    comb->addItem(QString("h:m:s (2:21:9)"),QVariant(QString("h:m:s")));
    comb->addItem(QString("hh:mm:ss (02:21:09)"),QVariant(QString("hh:mm:ss")));
    comb->addItem(QString("yyyy-M-d (2014-2-8)"),QVariant(QString("yyyy-M-d")));
    comb->addItem(QString("yyyy-M-d h:m (2014-2-8 2:21)"),QVariant(QString("yyyy-M-d h:m")));
    comb->addItem(QString("yyyy-M-d h:m:s (2014-2-8 2:21:9)"),QVariant(QString("yyyy-M-d h:m:s")));
    comb->addItem(QString("yyyy-MM-dd (2014-02-08)"),QVariant(QString("yyyy-MM-dd")));
    comb->addItem(QString("yyyy-MM-dd hh:mm(2014-02-08 02:21)"),QVariant(QString("yyyy-MM-dd hh:mm")));
    comb->addItem(QString("yyyy-MM-dd hh:mm:ss (2014-02-08 02:21:09)"),QVariant(QString("yyyy-MM-dd hh:mm:ss")));
}


void Dialog_AddChart::on_checkBox_isSelDefX_toggled(bool checked)
{
    if (checked)
    {
        Dialog_SelDefSeries dlg;
        if (QDialog::Accepted == dlg.exec())
        {
            ui->label_selDefX->setText(QString("%1:%2").arg(dlg.getStartData()).arg(dlg.getDetalData()));
            m_start = dlg.getStartData();
            m_detal = dlg.getDetalData();
        }
        else
        {
            ui->checkBox_isSelDefX->setChecked(false);
        }
    }
}


void Dialog_AddChart::on_checkBox_isSelDefY_toggled(bool checked)
{
    if (checked)
    {
        Dialog_SelDefSeries dlg;
        if (QDialog::Accepted == dlg.exec())
        {
            ui->label_selDefY->setText(QString("%1:%2").arg(dlg.getStartData()).arg(dlg.getDetalData()));
            m_start = dlg.getStartData();
            m_detal = dlg.getDetalData();
        }
        else
        {
            ui->checkBox_isSelDefY->setChecked(false);
        }
    }
}

SAValueManagerModel *Dialog_AddChart::getValueModel()
{
    return qobject_cast<SAValueManagerModel*>(ui->treeView->model());
}

void Dialog_AddChart::showProperty(QModelIndex index)
{
    if(index.column () != 0)
    {
        QModelIndex parent = index.parent ();
        index = parent.child (index.row (),0);
    }
    m_easyProperty.deleteAllProperty();
    SAAbstractDatas* baseData = getValueModel()->castToDataPtr(index);
    if(!baseData)
        return;
    QList<QtProperty*> property =  getDataTreeProperty(baseData);
    std::for_each(property.begin (),property.end (),[&](QtProperty* p){
        ui->propertyTree->addProperty(p);
    });
}

QList<QtProperty*> Dialog_AddChart::getDataTreeProperty(SAAbstractDatas* baseData)
{
    QList<QtProperty*> propertys;
    getBaseDataProperty(baseData,propertys);
    int type = baseData->getType ();
    switch (type) {
    case SA::VectorDouble:
    case SA::VectorPoint:
        break;
    default:
        break;
    }
    return propertys;
}

void Dialog_AddChart::on_treeView_clicked(const QModelIndex & index)
{
    showProperty(index);
}


void Dialog_AddChart::on_pushButto_add_clicked()
{
	int xIndex,yIndex;
	xIndex = ui->comboBox_X->currentIndex();
	yIndex = ui->comboBox_Y->currentIndex();
    if( (-1 == xIndex)&& (-1 == yIndex) )
    {
        QMessageBox::information(this,tr("information"),tr("Please select the x, y data in the check box"));//请在复选框中选择X,y对应的数据
        return;
    }
    bool isXSelDef(false),isYSelDef(false);
    //看看有没有自定义
    isXSelDef = ui->checkBox_isSelDefX->isChecked();
    isYSelDef = ui->checkBox_isSelDefY->isChecked();
    if(xIndex < 0)
    {
        if (!isXSelDef)
        {
            QMessageBox::information(this,tr("information"),tr("Please select the x data"));//请选择X对应的数据
            return;
        }
    }
    if(yIndex<0)
    {
        if (!isYSelDef)
        {
            QMessageBox::information(this,tr("information"),tr("Please select the y data"));//请选择Y对应的数据
            return;
        }
    }
    if (isXSelDef&&isYSelDef)
    {
        QMessageBox::information(this,tr("information"),tr("X, Y data can not be customized for the same time"));//X、Y数据不能同时为自定义
        return;
    }
    if(xIndex == yIndex && !isYSelDef && !isXSelDef)
    {
        QMessageBox::information(this,tr("information"),tr("Please select a different series"));//请选择不同的序列
        return;
    }
    if (!isYSelDef && !isXSelDef)
        noSelfDefineSeriesSelectDraw();
    else if(isXSelDef)
        haveXSelfDefineSeriesSelectDraw();
    else if(isYSelDef)
        haveYSelfDefineSeriesSelectDraw();
    ui->comboBox_X->setCurrentIndex(-1);
    ui->comboBox_Y->setCurrentIndex(-1);
}

void Dialog_AddChart::noSelfDefineSeriesSelectDraw()
{
	QStandardItemModel* comboxModel = static_cast<QStandardItemModel*>(ui->comboBox_X->model());
	int xIndex,yIndex;
	xIndex = ui->comboBox_X->currentIndex();
	yIndex = ui->comboBox_Y->currentIndex();
    if(xIndex < 0 || yIndex < 0)
    {
        return;
    }

    int x_id = comboxModel->item(xIndex)->data(ROLE_DATA_ID).toInt();
    int y_id = comboxModel->item(yIndex)->data(ROLE_DATA_ID).toInt();
    SAAbstractDatas* x = saValueManager->findData (x_id);
    SAAbstractDatas* y = saValueManager->findData (y_id);
    if(nullptr==x||nullptr ==y)
	{
        QMessageBox::information(this
                                 ,tr("information")
                                 ,tr("Please select the x, y data in the check box"));//请选择X和Y对应的数据
		return;
	}
    ++m_lineCount;
	QColor clr = getRandColor();
    QString strDataGroup=QStringLiteral("x:%1,y:%2")
            .arg(x->getName())
            .arg(y->getName());//序列组合
    QString strDataType=QStringLiteral("x:%1,y:%2")
            .arg(x->getTypeName())
             .arg(y->getTypeName());//数据类型
    auto nums = std::min(x->getSize (),y->getSize ());

    QwtPlotCurve* cur = drawChart(x,y);
    cur->setTitle(tr("curve-%1").arg(m_lineCount));
	cur->setPen(clr);
    addTableRow(cur,strDataGroup,strDataType,nums);
	ui->chart->updateAxes();
}



void Dialog_AddChart::haveXSelfDefineSeriesSelectDraw()
{
    QStandardItemModel* comboxModel = static_cast<QStandardItemModel*>(ui->comboBox_X->model());
    int yIndex;
    yIndex = ui->comboBox_Y->currentIndex();
    if(yIndex < 0)
    {
        return;
    }
    ++m_lineCount;
    int y_id = comboxModel->item(yIndex)->data(ROLE_DATA_ID).toInt();
    SAAbstractDatas* y = saValueManager->findData (y_id);
    if(!y)
    {
        QMessageBox::information(this,tr("information")
                                 ,tr("Please select the y data"));//请选择Y对应的数据
        return;
    }
    QColor clr = getRandColor();
    QString strDataGroup = QStringLiteral("x:%1-%2,y:%3")
            .arg(m_start).arg(m_detal)
            .arg(y->getName());
     QString strDataType = QStringLiteral("x:%1,y:%2")//数据类型
        .arg(tr("User Define-double"))
        .arg(y->getTypeName());
    auto nums = y->getSize ();//数据点数
    //==这里处理自定义数据

    QwtPlotCurve* cur = drawChart(y,true,m_start,m_detal);
    cur->setTitle(tr("curve-%1").arg(m_lineCount));
    cur->setPen(clr);
    addTableRow(cur,strDataGroup,strDataType,nums);
    ui->chart->updateAxes();
}

void Dialog_AddChart::haveYSelfDefineSeriesSelectDraw()
{
    QStandardItemModel* comboxModel = static_cast<QStandardItemModel*>(ui->comboBox_Y->model());
    int xIndex;
    xIndex = ui->comboBox_X->currentIndex();
    if(xIndex < 0)
    {
        return;
    }
    int x_id = comboxModel->item(xIndex)->data(ROLE_DATA_ID).toInt();
    SAAbstractDatas* x = saValueManager->findData (x_id);
    if(!x)
    {
        QMessageBox::information(this,tr("information")
                                 ,tr("Please select the x data"));
        return;
    }
    ++m_lineCount;
    QColor clr = getRandColor();
    QString strDataGroup = QStringLiteral("x:%1,y:%2-%3")
            .arg(x->getName())
            .arg(m_start)
            .arg(m_detal);
     QString strDataType = QStringLiteral("x:%1,y:%2")//数据类型
        .arg(x->getTypeName())
        .arg(tr("UserDef-double"));

    auto nums =  x->getSize ();//数据点数
    //==这里处理自定义数据

    QwtPlotCurve* cur = drawChart(x,false,m_start,m_detal);
    cur->setTitle(tr("curve-%1").arg(m_lineCount));
    cur->setPen(clr);
    addTableRow(cur,strDataGroup,strDataType,nums);

    ui->chart->updateAxes();
}

void Dialog_AddChart::on_pushButton_del_clicked()
{
    QStandardItemModel* tableModel = qobject_cast<QStandardItemModel*>(ui->tableView->model());
	QModelIndex curIndex = ui->tableView->currentIndex();
	if (!curIndex.isValid())
	{
        QMessageBox::information(this,tr("information")
                                 ,tr("Please select the serial to be deleted"));//请选择需要删除的序列
		return;
	}
	QVariant var = curIndex.data(Dialog_AddChart_TREE_ITEM_ROLE_CURVE_PTR);
	if (!var.isValid())
	{
        QMessageBox::information(this,tr("information")
                                 ,tr("Failed to detect curve"));//未能检测到对应曲线
		return;
	}
    QwtPlotCurve* cur = reinterpret_cast<QwtPlotCurve*>(var.value<quintptr>());
	if (nullptr != cur)
	{
		cur->detach();
		delete cur;
		ui->chart->replot();
	}
	tableModel->removeRow(curIndex.row());
}

void Dialog_AddChart::on_pushButton_finish_clicked()
{
	if (getDrawCurveList().size()<=0)
	{
        QMessageBox::information(this,tr("information")
                                 ,tr("Failed to detect curve"));//未检测到图例
		return;
	}
	QDialog::accept();
}


bool Dialog_AddChart::getSeries(SAAbstractDatas* series,QVector<double>& datas,bool isTime,const QString& timeFormat)
{
    if(isTime)
    {
        {
            return saValueManager->getVectorData (series,datas);
        }
    }
    else
    {
        return saValueManager->getVectorData (series,datas);
    }
    return false;
}

QwtPlotCurve* Dialog_AddChart::drawChart(SAAbstractDatas* x, SAAbstractDatas* y)
{
	QwtPlotCurve* pC = nullptr;
    QVector<double> xdata,ydata;

	bool isXTimeScale = Qt::Checked == ui->checkBox_isTimeX->checkState();
	bool isYTimeScale = Qt::Checked == ui->checkBox_isTimeY->checkState();	
	if (isXTimeScale)
	{
		QString strDateFormat = ui->comboBox_TX->itemData(ui->comboBox_TX->currentIndex()).toString();
        getSeries (x,xdata,isXTimeScale,strDateFormat);

	}
    else
        getSeries (x,xdata,isXTimeScale,QString());

    if (isYTimeScale)
	{
		QString strDateFormat = ui->comboBox_TY->itemData(ui->comboBox_TY->currentIndex()).toString();
        getSeries (y,ydata,isYTimeScale,strDateFormat);

    }
    else
        getSeries (y,ydata,isYTimeScale,QString());
    pC = ui->chart->addCurve(xdata,ydata);
    if (isXTimeScale)
        ui->chart->setAxisDateTimeScale(ui->comboBox_TX->itemData(ui->comboBox_TX->currentIndex()).toString(),QwtPlot::xBottom);
    if (isYTimeScale)
        ui->chart->setAxisDateTimeScale(ui->comboBox_TY->itemData(ui->comboBox_TY->currentIndex()).toString(),QwtPlot::yLeft);
    return pC;
}

QwtPlotCurve*Dialog_AddChart::drawChart(SAAbstractDatas* data, bool isXSelDef, double start, double detal)
{
    QwtPlotCurve* pC = nullptr;
    QVector<double> xdata,ydata;
    if(isXSelDef)
    {
        getSeries (data,ydata,false,QString());
        for(int i=0;i<ydata.size ();++i)
        {
            xdata.push_back (start+double(i)*detal);
        }
    }
     else
    {
        getSeries (data,xdata,false,QString());
        for(int i=0;i<xdata.size ();++i)
        {
            ydata.push_back (start+double(i)*detal);
        }
    }
    pC = ui->chart->addCurve(xdata,ydata);
    return pC;
}

void Dialog_AddChart::getBaseDataProperty(SAAbstractDatas* baseData,QList<QtProperty*>& propertys)
{ 
    QtProperty* groupItem = m_pVarManager_NotEdit->addProperty(QtVariantPropertyManager::groupTypeId()
                                                               ,tr("Basic data type"));//基本数据属性
    m_easyProperty.rememberTheProperty(QString("Basic data type"),groupItem);//
    m_easyProperty.addVariantPropertyInGroup(m_pVarManager_NotEdit
                                        ,QVariant::String
                                        ,groupItem
                                        ,tr("name"),QString("data name")//名称
                                        ,baseData->getName ());
    m_easyProperty.addVariantPropertyInGroup(m_pVarManager_NotEdit
                                        ,QVariant::String
                                        ,groupItem
                                        ,tr("type"),QString("data type")//数据类型
                                        ,baseData->getTypeName());
    int size = baseData->getSize();
    m_easyProperty.addVariantPropertyInGroup(m_pVarManager_NotEdit
                                        ,QVariant::String
                                        ,groupItem
                                        ,tr("length"),QString("data length")//长度
                                        ,QString::number ((size<0)?0:size));

    propertys.append (groupItem);
}

void Dialog_AddChart::addTableRow(const QwtPlotCurve* cur, const QString& dataGroup, const QString& DataType, size_t dataNum)
{
    QStandardItemModel* tableModel = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if(tableModel)
    {
        int rowCount = tableModel->rowCount();
        QStandardItem* itemTable = nullptr;
        itemTable = new QStandardItem(cur->title().text());
        itemTable->setData(QVariant::fromValue(reinterpret_cast<quintptr>(cur)),Dialog_AddChart_TREE_ITEM_ROLE_CURVE_PTR);//把绘图指针记录下来
        tableModel->setItem(rowCount,0,itemTable);//序列名称
        itemTable = new QStandardItem(dataGroup);
        tableModel->setItem(rowCount,1,itemTable);//序列组合
        itemTable = new QStandardItem(DataType);
        tableModel->setItem(rowCount,2,itemTable);//数据类型
        itemTable = new QStandardItem(QString::number(dataNum));
        tableModel->setItem(rowCount,3,itemTable);
        itemTable = new QStandardItem((""));//颜色
        itemTable->setData(QVariant(cur->pen().color()),Qt::BackgroundRole);
        tableModel->setItem(rowCount,4,itemTable);
    }
}


static QColor static_line_color[9] = {
	Qt::black
	,Qt::red
	,Qt::green
	,Qt::blue
	,Qt::cyan
	,Qt::magenta
	,Qt::yellow
	,Qt::gray
	,Qt::lightGray
};

QColor Dialog_AddChart::getRandColor()
{
	return static_line_color[int (float(qrand())/float(RAND_MAX)*9.0)];
}
void Dialog_AddChart::vectorData2VectorPoint(const QVector<QVariant>& in_X,const QVector<QVariant>& in_Y, QVector<QPointF>& res)
{
	auto length = (in_X.size() > in_Y.size() ? in_Y.size() : in_X.size());
	res.clear();
	res.reserve(length);
	for(auto i = 0;i<length;++i)
	{
		res.push_back(QPointF(in_X.at(i).toDouble(),in_Y.at(i).toDouble()));
	}
}

QList<QwtPlotCurve*> Dialog_AddChart::getDrawCurveList()
{
    return ui->chart->getCurveList();
}
///
/// \brief 判断是否是时间坐标，是的话返回时间的格式
/// \param is
/// \param ax
/// \return
///
QString Dialog_AddChart::isAxisDateTime(bool *is, QwtPlot::Axis ax)
{
    QwtDateScaleDraw* dateScale = dynamic_cast<QwtDateScaleDraw*>(ui->chart->axisScaleDraw(ax));
    if(dateScale)
    {
        *is = true;
        return dateScale->dateFormat(QwtDate::Second);
    }
    *is = false;
    return "";
}

const SAChart2D *Dialog_AddChart::chart()
{
    return ui->chart;
}



void Dialog_AddChart::on_pushButton_clearXY_clicked()
{
    ui->comboBox_X->setCurrentIndex(-1);
    ui->comboBox_Y->setCurrentIndex(-1);
}




