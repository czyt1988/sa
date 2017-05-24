#include "CFX2DCsvImportDialog.h"
#include "ui_CFX2DCsvImportDialog.h"
#include <QFileDialog>
#include <QStringListModel>
#include <QMessageBox>
#include "SADataTableModel.h"
#include <QTextCodec>
#include "SAValueManager.h"
CFX2DCsvImportDialog::CFX2DCsvImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFX2DCsvImportDialog)
  ,m_isDeleteMakeValue(true)
{
    ui->setupUi(this);
    m_dataConfigs.reset(new CfxCsvDataImportConfig);
    initUI ();

}

CFX2DCsvImportDialog::~CFX2DCsvImportDialog()
{
    if(m_isDeleteMakeValue)
    {
        clearAndDeleteDatas();
    }
    delete ui;
}

QList<SAAbstractDatas*> CFX2DCsvImportDialog::getResult() const
{
    return m_datas;
}

void CFX2DCsvImportDialog::addCsvFile()
{
    QStringList selFiles = QFileDialog::getOpenFileNames (this,tr("open csv file")//打开csv文件
                                   ,QString()
                                   ,tr("Ansys CFX csv file(*.csv)")//Ansys CFX csv file(*.csv)
                                   );
    if(selFiles.size ()<=0)
        return;
    QStringListModel* model = getStringListModel ();
    if(!model)
        return;
    QStringList slist = model->stringList ();
    slist += selFiles;
    model->setStringList (slist);
}

void CFX2DCsvImportDialog::removeFile()
{
    QModelIndex index = ui->listView->currentIndex ();
    if(!index.isValid ())
        return;
    QStringListModel* model = getStringListModel ();
    model->removeRow (index.row ());
}

void CFX2DCsvImportDialog::parser()
{
    clearAndDeleteDatas();
    m_datas.clear();
    SADataTableModel* model = getDataTableModel();
    model->setSADataPtrs (QList<SAAbstractDatas*>());

    ui->pushButton_ok->setEnabled (true);
    m_dataConfigs->clear ();//重新解析需要清空之前的内容，否则会重复解析
    QStringList paths = getStringListModel ()->stringList ();
    if(!m_dataConfigs->openFiles (paths))
        return;
    m_dataConfigs->setCodec(m_codec);
    QVariant var = ui->comboBox_parserSet->currentData ();
    if(!var.isValid ())
    {
        QMessageBox::information (this,tr("information")
                                  ,tr("Please select the analysis mode!"));//请选择解析模式！
        return;
    }
    CfxCsvDataImportConfig::ExportWay exportWay = static_cast<CfxCsvDataImportConfig::ExportWay>(var.toInt ());
    m_dataConfigs->setExportWay (exportWay);
    if(!m_dataConfigs->parser ())
    {
        QMessageBox::information (this,tr("information")
                                  ,tr("faile to parser, please ensure that the data format of the file is correct"));//解析失败，请确保文件的数据格式正确性
        return;
    }

    QList<SAAbstractDatas*> res = m_dataConfigs->createResultPtr ();
    if(res.size () <= 0)
        return;
    m_datas = res;
    if(model)
        model->setSADataPtrs (m_datas);
}

void CFX2DCsvImportDialog::initUI()
{
    connect (ui->pushButton_ok,&QAbstractButton::clicked,this,&QDialog::accept);
    connect (ui->pushButton_cancel,&QAbstractButton::clicked,this,&QDialog::reject);
    connect (ui->pushButton_add,&QAbstractButton::clicked,this,&CFX2DCsvImportDialog::addCsvFile);
    connect (ui->pushButton_remove,&QAbstractButton::clicked,this,&CFX2DCsvImportDialog::removeFile);
    connect (ui->pushButton_parser,&QAbstractButton::clicked,this,&CFX2DCsvImportDialog::parser);
    QStringListModel* model = new QStringListModel(this);
    ui->listView->setModel (model);
    SADataTableModel* dataModel = new SADataTableModel(this);
    ui->tableView->setModel (dataModel);
    ui->comboBox_parserSet->addItem (tr("1 row x,n rows y"),CfxCsvDataImportConfig::ToVectorDouble_1XnY);//1行x值,n行y值
    ui->comboBox_parserSet->addItem (tr("n rows x,1 row y"),CfxCsvDataImportConfig::ToVectorDouble_nX1Y);//n行x值,1行y值
    ui->comboBox_parserSet->addItem (tr("n rows x,n rows y"),CfxCsvDataImportConfig::ToVectorDouble_nXnY);//n行x值,n行y值
    ui->comboBox_parserSet->addItem (tr("(x,y) point array"),CfxCsvDataImportConfig::toVectorPointF);//作为(x,y)点序列
    ui->comboBox_parserSet->setCurrentIndex (2);
    ui->pushButton_ok->setEnabled (false);

    ui->comboBox_codec->setCurrentText (QString(QTextCodec::codecForLocale ()->name ()));
    QList<QByteArray> codecs =	QTextCodec::availableCodecs();
    for(int i=0;i<codecs.size();++i)
    {
        ui->comboBox_codec->addItem(QString(codecs[i]),codecs[i]);
    }
}

QStringListModel* CFX2DCsvImportDialog::getStringListModel()
{
    return static_cast<QStringListModel*>(ui->listView->model ());
}

SADataTableModel*CFX2DCsvImportDialog::getDataTableModel()
{
    return static_cast<SADataTableModel*>(ui->tableView->model ());
}

void CFX2DCsvImportDialog::clearAndDeleteDatas()
{
    if(m_datas.size ()<=0)
        return;
    for(int i=0;i<m_datas.size ();++i)
    {
        if(m_datas[i])
        {
            SAValueManager::deleteData_s(m_datas[i]);
        }
    }
    m_datas.clear ();
}

void CFX2DCsvImportDialog::on_comboBox_codec_currentIndexChanged(const QString &arg1)
{
    m_codec = arg1;
    parser();
}

void CFX2DCsvImportDialog::on_pushButton_ok_clicked()
{
    saValueManager->addDatas(m_datas);
    m_isDeleteMakeValue = false;
    return QDialog::accept();
}



void CFX2DCsvImportDialog::on_pushButton_cancel_clicked()
{
    clearAndDeleteDatas();
    return QDialog::reject();
}
