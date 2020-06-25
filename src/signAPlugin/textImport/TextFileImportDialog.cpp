#include "TextFileImportDialog.h"
#include "ui_TextFileImportDialog.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "CheckedHeaderView.h"
#include <algorithm>
//#include "SACsvDataImport.h"
#include <QTextCodec>
#include <QDir>
#include <QInputDialog>
//QStringLiteral(":\\-自定义-//:")
#define STR_SELF_DEFINE QObject::tr(":\\-自定义-//:")

#include <QElapsedTimer>
#include <QDebug>
#include <SADataTableModel.h>
#include "SATextReadWriter.h"
#include <QThread>
#include "SALog.h"
#include <QMutexLocker>
#include <QTimer>
TextFileImportDialog::TextFileImportDialog(const QString& filePath, TextType type, QWidget *parent)
    :QDialog(parent),
    ui(new Ui::TextFileImportDialog)
    ,m_enableInput(true)
    ,m_currentSplit(",")
    ,m_maxShowLine(500)
    ,m_realEndLine(0)
    ,m_textReaderThread(nullptr)
    ,m_textReader(nullptr)
{
    ui->setupUi(this);
    setTextType(type);
    init();
    openFile(filePath);
}

TextFileImportDialog::TextFileImportDialog(QWidget *parent)
    :QDialog(parent),
    ui(new Ui::TextFileImportDialog)
    ,m_enableInput(true)
    ,m_currentSplit(",")
    ,m_maxShowLine(500)
    ,m_realEndLine(0)
    ,m_textReaderThread(nullptr)
    ,m_textReader(nullptr)
{
    ui->setupUi(this);
    init();
}

void TextFileImportDialog::init()
{

    ui->comboBox_Separator->addItem(tr(",(comma)"),QVariant(","));//逗号
    ui->comboBox_Separator->addItem(tr(" (space)"),QVariant(" "));//空格
    ui->comboBox_Separator->addItem(tr("    (tab stop)"),QVariant("\t"));//tab制表位
    ui->comboBox_Separator->addItem(tr(";(semicolon)"),QVariant(";"));//分号
    ui->comboBox_Separator->addItem(tr("_(underscore)"),QVariant("_"));//下横杠
    ui->comboBox_Separator->addItem(tr("-(dash)"),QVariant("-"));//横杠
    ui->comboBox_Separator->addItem(tr("customized"),QVariant(STR_SELF_DEFINE));


    SADataTableModel* model = new SADataTableModel(ui->tableView);
    ui->tableView->setModel (model);

    ui->comboBox_codec->setCurrentText (QString(QTextCodec::codecForLocale ()->name ()));
    QList<QByteArray> codecs =	QTextCodec::availableCodecs();
    for(int i=0;i<codecs.size();++i)
    {
        ui->comboBox_codec->addItem(QString(codecs[i]),codecs[i]);
    }


    CheckedHeaderView* header = new CheckedHeaderView(Qt::Horizontal,ui->tableView);

    header->setHeaderChecked (false);
    ui->tableView->setHorizontalHeader (header);
    m_config.setAutoParser(false);

    m_config.setStartLine(1);
    ui->spinBox_startReadLine->setValue(1);
    m_config.setEndLine (m_maxShowLine);//为了刷新速度，只加载500行
    ui->spinBox_endLine->setValue(m_realEndLine);

    m_config.setAutoParser(true);
    updateData(false);

    connect(&m_config,&TextImportConfig::dataChanged
            ,this,&TextFileImportDialog::onHaveParser);

}

TextFileImportDialog::~TextFileImportDialog()
{
    delete ui;
}

QList<SAAbstractDatas *> TextFileImportDialog::getResult() const
{
    QList<SAAbstractDatas *> res;
    std::for_each(m_res.cbegin(),m_res.cend(),[&res](const std::shared_ptr<SAAbstractDatas>& d){
        res << d.get();
    });
    return res;
}

bool TextFileImportDialog::openFile(const QString &filePath)
{
    m_config.setFile(filePath);
    if(!m_config.open())
    {
        ui->stackedWidget->setDisabled(true);
        ui->pushButton_Next->setDisabled(true);
        ui->pushButton_moreSet->setDisabled(true);
        ui->pushButton_Back->setDisabled(true);
        return false;
    }
    if(filePath.size() > 3)
    {
        if(filePath.mid(filePath.size()-3,3) == "csv")
        {
            ui->radioButton_Separator->setEnabled(false);
            ui->comboBox_Separator->setCurrentIndex(0);
            ui->comboBox_Separator->setEnabled(false);
            m_currentSplit = ",";
        }
        else
        {
            QVariant var = ui->comboBox_Separator->currentData ();
            if(var.isValid ())
            {
                m_currentSplit = var.toString ();
                if(STR_SELF_DEFINE!=m_currentSplit)
                    m_config.setSpliter (m_currentSplit);
                else
                    m_currentSplit = "";
            }

        }
    }
    saDebug(QString("open file:%1").arg(filePath));
    m_config.parser();
    startReadTextThread(filePath);
    return true;
}

void TextFileImportDialog::setTextType(TextType type)
{
    switch(type)
    {
    case Txt:m_config.setTextType(TextImportConfig::Txt);break;
    case Csv:m_config.setTextType(TextImportConfig::Csv);break;
    default:m_config.setTextType(TextImportConfig::Txt);break;
    }
}

SADataTableModel *TextFileImportDialog::getTableModel()
{
    return static_cast<SADataTableModel*>(ui->tableView->model ());
}




void TextFileImportDialog::on_spinBox_startReadLine_valueChanged(int arg1)
{
    m_config.setStartLine (arg1);
}

void TextFileImportDialog::on_spinBox_toHeader_valueChanged(int arg1)
{
    SADataTableModel* txtModel = getTableModel ();
    if(!txtModel)
        return;
    if(arg1 < 0)
    {
        ui->spinBox_toHeader->setValue(0);
    }
    if(arg1 <= 0)
    {
        auto datas = m_config.getData();
        for(int i=0;i<datas.size();++i)
        {
            if(i < datas.size())
            {
                datas[i]->setName(tr("data_%1").arg(i+1));
            }
        }
    }
    else
    {
        QStringList rowData = m_config.getRawConfigRow(arg1-1);
        auto datas = m_config.getData();
        for(int i=0;i<rowData.size();++i)
        {
            if(i < datas.size())
            {
                datas[i]->setName(rowData[i]);
            }
        }
    }
    getTableModel()->update();
}

void TextFileImportDialog::on_spinBox_endLine_valueChanged(int arg1)
{
    m_realEndLine = arg1;
    if(m_realEndLine < m_maxShowLine && m_realEndLine>0)
        m_config.setEndLine (m_realEndLine);
    else
        m_config.setEndLine (m_maxShowLine);
}



void TextFileImportDialog::on_comboBox_Separator_currentIndexChanged(int index)
{
    QVariant var = ui->comboBox_Separator->itemData (index);
    if(var.isValid ())
    {
        QString s = var.toString ();
        bool ok;
        if(STR_SELF_DEFINE==s && m_enableInput)
        {
            s = QInputDialog::getText (this,tr("Input")//输入
                                       ,tr("Please enter a separator:")//请输入分隔符：
                                       ,QLineEdit::Normal,QString(),&ok);
            if(!ok)
            {
                ui->comboBox_Separator->setCurrentText (m_currentSplit);
                return;
            }
            m_enableInput = false;
            ui->comboBox_Separator->insertItem (0,s,s);
            ui->comboBox_Separator->setCurrentIndex (0);
        }
        m_config.setSpliter (s);
        m_currentSplit = s;
        m_enableInput = true;
    }
}

void TextFileImportDialog::on_pushButton_Next_clicked()
{
    int index = ui->stackedWidget->currentIndex ();
    if(index != ui->stackedWidget->count ()-1)
    {
        ++index;
        ui->stackedWidget->setCurrentIndex (index);
        ui->pushButton_Back->setEnabled (true);
    }
    else
    {
        updateData (true);
        deal();
        return QDialog::accept();
    }
    if(index == ui->stackedWidget->count ()-1)
        ui->pushButton_Next->setText (tr("finish"));//完 成
}

void TextFileImportDialog::on_pushButton_Back_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    if(index == ui->stackedWidget->count ()-1)
        ui->pushButton_Next->setText (tr("next"));//下一步
    if( index != 0)
    {
        --index;
        ui->stackedWidget->setCurrentIndex(index);
        ui->pushButton_Back->setEnabled((0 == index) ? false : true);
    }
}

void TextFileImportDialog::deal()
{
    m_config.setAutoParser(false);
    m_config.setEndLine(m_realEndLine);
    m_config.parser();
    //开始提取数据
    auto resPtr = m_config.createResultPtr();

    QString dataNameList;
    m_res = m_config.getData();

    std::for_each(m_res.begin(),m_res.end(),[&dataNameList](std::shared_ptr<SAAbstractDatas>& p){
        dataNameList += QString("[%1]").arg(p->getName());
    });

    QString info = tr("import data from:\"%1\" \r\n"
                      "create data:%2")
            .arg(m_config.getOpenFilePath())
            .arg(dataNameList);
    saUI->showNormalMessageInfo(info);

    //
    if(m_realEndLine<m_maxShowLine && m_realEndLine>0)
        m_config.setEndLine (m_realEndLine);
    else
        m_config.setEndLine (m_maxShowLine);
    m_config.setAutoParser(true);
}




void TextFileImportDialog::on_radioButton_1D_toggled(bool checked)
{
    Q_UNUSED(checked);
}



void TextFileImportDialog::on_stackedWidget_currentChanged(int arg1)
{
    CheckedHeaderView* head = static_cast<CheckedHeaderView*>(ui->tableView->horizontalHeader ());
    if(head)
    {
        if(1!=arg1)
        {
            head->setHeaderChecked (false);
            return;
        }
        head->setHeaderChecked (ui->radioButton_1D->isChecked ());
    }

}


///
/// \brief 刷新界面
/// \param downLoad
///
void TextFileImportDialog::updateData(bool downLoad)
{
    m_config.setAutoParser(false);
    if(downLoad)
    {
        m_config.setSpliter (m_currentSplit);
        m_config.setCodec (ui->comboBox_codec->currentText ());
        m_config.setStartLine (ui->spinBox_startReadLine->value ());
        m_realEndLine = ui->spinBox_endLine->value ();
        if(m_realEndLine<m_maxShowLine && m_realEndLine>0)
            m_config.setEndLine (m_realEndLine);
        else
            m_config.setEndLine (m_maxShowLine);
        m_config.setToOneColumn (ui->radioButton_oneColumn->isChecked ());
    }
    else
    {
        ui->comboBox_Separator->setCurrentText (m_config.getSpliter ());
        m_currentSplit = m_config.getSpliter ();
        ui->comboBox_codec->setCurrentText (m_config.getCodec ());
        ui->spinBox_startReadLine->setValue (m_config.getStartLine ());
        ui->spinBox_endLine->setValue (m_realEndLine);
        ui->radioButton_oneColumn->setChecked (m_config.isToOneColumn ());
    }
    m_config.setAutoParser(true);
}





void TextFileImportDialog::on_comboBox_codec_currentIndexChanged(const QString &arg1)
{
    m_config.setCodec(arg1);
    startReadTextThread(m_config.getOpenFilePath());
}

void TextFileImportDialog::on_comboBox_codec_currentTextChanged(const QString &arg1)
{
    m_config.setCodec(arg1);
    startReadTextThread(m_config.getOpenFilePath());
}

void TextFileImportDialog::onHaveParser()
{
    QList<SAAbstractDatas*> datas;
    auto d = m_config.getData();
    std::for_each(d.begin(),d.end(),[&datas](std::shared_ptr<SAAbstractDatas>& p){
        datas<< p.get();
    });


    SADataTableModel* txtModel = getTableModel ();
    if(!txtModel)
        return;
    int arg1 =  ui->spinBox_toHeader->value();
    if(arg1 >= 1)
    {
        QStringList rowData = m_config.getRawConfigRow(arg1-1);
        auto datas = m_config.getData();
        for(int i=0;i<rowData.size();++i)
        {
            if(i < datas.size())
            {
                datas[i]->setName(rowData[i]);
            }
        }
    }


    txtModel->setSADataPtrs(datas);
    txtModel->update();
}



void TextFileImportDialog::on_radioButton_oneColumn_clicked(bool checked)
{
    m_config.setToOneColumn (checked);
}

void TextFileImportDialog::onTextReadComplete(const QString &text, bool isOK)
{
    saStartElapsed();
    //TODO 耗时过久，可以考虑使用mvc模型，使用QTableView
    if(isOK)
    {
        //ui->textBrowser->setText(text);
        ui->plainTextEdit->setPlainText(text);//->append( m_readedText.takeFirst() );
    }
    saElapsed();
}

void TextFileImportDialog::onTextThreadDestroyed(QObject *obj)
{
    Q_UNUSED(obj);
    QMutexLocker lock(&m_mutex);
    m_textReaderThread = nullptr;
    m_textReader = nullptr;
}

void TextFileImportDialog::startReadTextThread(const QString &filePath)
{
    ui->plainTextEdit->clear();
    //打开文本线程
    //如果上个线程还没结束，就让它结束,线程的资源(m_textReaderThread and m_textReader)自动释放
    {
        QMutexLocker lock(&m_mutex);
        if(m_textReaderThread)
        {
            disconnect(m_textReader,&SATextReadWriter::readComplete,this,&TextFileImportDialog::onTextReadComplete);
            m_textReaderThread->quit();
        }
    }
    m_readedText.clear();
    m_textReaderThread = new QThread();
    m_textReader = new SATextReadWriter();
    m_textReader->moveToThread(m_textReaderThread);
    m_textReader->setReadOnceCharCount(-1);
    m_textReader->setReadOnceLineCount(-1);
    m_textReader->setFileName(filePath);
    m_textReader->setCodec(ui->comboBox_codec->currentText());

    connect(m_textReaderThread,&QThread::finished,m_textReaderThread,&QThread::deleteLater);
    connect(m_textReaderThread,&QThread::finished,m_textReader,&SATextReadWriter::deleteLater);
    connect(m_textReaderThread,&QThread::started,m_textReader,&SATextReadWriter::startReadText);
    connect(m_textReader,&SATextReadWriter::readComplete,this,&TextFileImportDialog::onTextReadComplete);
    connect(m_textReader,&SATextReadWriter::readAllComplete,m_textReaderThread,&QThread::quit);
    connect(m_textReader,&SATextReadWriter::destroyed,this,&TextFileImportDialog::onTextThreadDestroyed);
    m_textReaderThread->start();

}

void TextFileImportDialog::onAppendTextTimeOut()
{

}
///
/// \brief 显示数据
///
void TextFileImportDialog::on_pushButton_DatasView_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}
