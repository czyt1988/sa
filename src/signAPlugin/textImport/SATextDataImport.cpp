#include "SATextDataImport.h"
#include "SADataImportFactory.h"
#include "TextFileImportDialog.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include "SAUIReflection.h"
#include <QAction>
#include <QMenu>
#include "CFX2DCsvImportDialog.h"
#include <QFileInfo>
#define TR(str)\
    QApplication::translate("SATextDataImport",str,0)
static const QString s_pluginName = TR("text/csv data import");
static const QString s_pluginDescription =  TR("import the txt or csv data");
static const QString s_pluginVersion = TR("Version 0.1");
static const SAAbstractPlugin::PluginType s_pluginType = SAAbstractPlugin::DataImport;
SA_PLUGIN_EXPORT SAAbstractPlugin *createPlugin()
{
    return new SATextDataImport();
}
SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin *obj)
{
    if(obj != nullptr)
    {
        delete obj;
    }
}
SA_PLUGIN_EXPORT QString pluginName()
{
    return s_pluginName;
}
SA_PLUGIN_EXPORT QString pluginVersion()
{
    return s_pluginVersion;
}
SA_PLUGIN_EXPORT QString pluginDescription()
{
    return s_pluginDescription;
}
SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType()
{
    return s_pluginType;
}

///
/// \brief 是通过SA系统自带的OpenFile还是使用自身openFile
///
/// 如果返回true。这时sa将会调用bool openFile(const QStringList* filePaths),并传入用户选择的路径，
/// 如果需要多选，需要isMultiselect返回true
/// 如果返回false，sa将会调用bool openFile()
/// \return
///
SATextDataImport::SATextDataImport()
    :m_ui(nullptr)
    ,m_isOpen(false)
{

}

SATextDataImport::~SATextDataImport()
{

}
///
/// \brief 返回插件类型
/// \return
///
SAAbstractPlugin::PluginType SATextDataImport::getType() const
{
    return s_pluginType;
}
///
/// \brief 获取插件名字
/// \return
///
QString SATextDataImport::getPluginName() const
{
    return s_pluginName;
}
///
/// \brief 获取插件版本
/// \return
///
QString SATextDataImport::getVersion() const
{
    return s_pluginVersion;
}
///
/// \brief 获取描述信息
/// \return
///
QString SATextDataImport::getDescription() const
{
    return s_pluginDescription;
}
///
/// \brief ui构建
///
void SATextDataImport::setupUI(SAUIInterface *ui)
{
    m_ui = ui;
    QMenu* menu = new QMenu(tr("Text/CSV Data Import"),m_ui->getMainWindowPtr());
    menu->setIcon(QIcon(":/icon/ico/import_txt.png"));

    m_actionTextImport = new QAction(QIcon(":/icon/ico/import_txt.png"),tr("Text/CSV Data Import"),this);
    m_actionTextImport->setObjectName( "actionTextImport" );
    m_actionTextImport->setWhatsThis(tr( "import text data and csv data"));
    connect(m_actionTextImport,&QAction::triggered
            ,this,&SATextDataImport::on_actionTextImport);
    m_actionCsvImport = new QAction(tr("CSV Data Import"),this);
    m_actionCsvImport->setObjectName( "actionCsvImport" );
    m_actionCsvImport->setWhatsThis(tr( "import csv data"));
    connect(m_actionCsvImport,&QAction::triggered
            ,this,&SATextDataImport::on_actionCsvImport);
    m_actionCFXCsvImport = new QAction(tr("CFX Export CSV Data Import"),this);
    m_actionCFXCsvImport->setObjectName( "actionCFXCsvImport" );
    m_actionCFXCsvImport->setWhatsThis(tr( "import ansys CFX Export csv data"));
    connect(m_actionCFXCsvImport,&QAction::triggered
            ,this,&SATextDataImport::on_cfxCsvImport);
    menu->addAction(m_actionTextImport);
    menu->addAction(m_actionCsvImport);
    menu->addAction(m_actionCFXCsvImport);

    m_ui->addDataImportPluginMenu(menu);
}



///
/// \brief isUseSAOpenFile返回false调用的打开文件接口函数
/// \return 成功返回true,将调用createResultPtr
///
bool SATextDataImport::openFile()
{
    QFileDialog dlg(m_ui->getMainWindowPtr());
    QStringList strNFilter;
    strNFilter.push_back(tr("text file(*.txt)"));
    strNFilter.push_back(tr("csv fila (*.csv)"));
    strNFilter.push_back(tr("all file (*.*)"));
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilters(strNFilter);
    if (QDialog::Accepted == dlg.exec())
    {
        QStringList strfileNames = dlg.selectedFiles();
        return openFile(strfileNames);
    }
    return false;
}
///
/// \brief isUseSAOpenFile返回true调用的打开文件接口函数
/// \return 成功返回true,将调用createResultPtr
///
bool SATextDataImport::openFile(const QStringList &filePaths)
{
    if(filePaths.size() != 1)
    {
        return false;
    }
    m_resPtr.clear();
    m_filePath = filePaths[0];
    QFileInfo temDir(m_filePath);
    QString suffix = temDir.suffix();
    suffix = suffix.toLower();
    bool isOpen = false;
    if(suffix == "csv")
    {
        isOpen = loadFile(filePaths,(int)TextFileImportDialog::Csv);
    }
    else
    {
        isOpen = loadFile(filePaths,(int)TextFileImportDialog::Txt);
    }
    return false;
}

///
/// \brief 是否打开了文件
/// \return
///
bool SATextDataImport::isOpenFile()
{
    return m_isOpen;
}

bool SATextDataImport::isUseSAOpenFile() const
{
    return true;
}
///
/// \brief 如果需要通过SA系统自带的OpenFile打开，需要提供支持的后缀信息给sa
/// isUseSAOpenFile返回true是必须实现此函数
/// \param nameFileter 文件描述
/// \param suffixKey 后缀描述
///
void SATextDataImport::getSAOpenFileInfo(QStringList &nameFileter, QStringList &suffixKey) const
{
    nameFileter.push_back(tr("文本文件 (*.txt)"));
    nameFileter.push_back(tr("逗号分隔文件 (*.csv)"));
    suffixKey.push_back("txt");
    suffixKey.push_back("csv");
}

///
/// \brief 创建导出的数据
/// \return
///
QList<SAAbstractDatas *> SATextDataImport::getResultPtr()
{
    return m_resPtr;
}
///
/// \brief 响应QAction的槽
///
void SATextDataImport::on_actionTextImport()
{
    QFileDialog dlg(m_ui->getMainWindowPtr());
    QStringList strNFilter;
    strNFilter.push_back(tr("text file(*.txt)"));
    strNFilter.push_back(tr("csv file (*.csv)"));
    strNFilter.push_back(tr("all file (*.*)"));
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilters(strNFilter);
    if (QDialog::Accepted == dlg.exec())
    {
        QStringList strfileNames = dlg.selectedFiles();
        loadFile(strfileNames,TextFileImportDialog::Txt);
    }
}

void SATextDataImport::on_actionCsvImport()
{
    QFileDialog dlg(m_ui->getMainWindowPtr());
    QStringList strNFilter;
    strNFilter.push_back(tr("csv file (*.csv)"));
    strNFilter.push_back(tr("all file (*.*)"));
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilters(strNFilter);
    if (QDialog::Accepted == dlg.exec())
    {
        QStringList strfileNames = dlg.selectedFiles();
        loadFile(strfileNames,TextFileImportDialog::Csv);
    }
}

void SATextDataImport::on_cfxCsvImport()
{
    CFX2DCsvImportDialog dlg(m_ui->getMainWindowPtr());
    dlg.exec();
}

bool SATextDataImport::loadFile(const QStringList &filePaths, int type)
{
    if(filePaths.size() != 1)
    {
        return false;
    }
    m_resPtr.clear();
    QString filePath = filePaths[0];
    TextFileImportDialog dlg(m_ui->getMainWindowPtr());
    dlg.setTextType(static_cast<TextFileImportDialog::TextType>(type));
    m_isOpen = dlg.openFile(filePath);
    if(!m_isOpen)
    {
        QMessageBox::warning(&dlg,tr("warning"),tr("can not open file"));
        return false;
    }
    if(QDialog::Accepted == dlg.exec())
    {
        m_resPtr = dlg.getResult();
        return true;
    }
    return false;
}


//====================================

//SA_PLUGIN_EXPORT SAAbstractPlugin *createPlugin()
//{
//    return new SATextDataImport();
//}

//SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin *obj)
//{
//    if(obj != nullptr)
//    {
//        delete obj;
//    }
//}

//SA_PLUGIN_EXPORT QString pluginName()
//{
//    return s_pluginName;
//}

//SA_PLUGIN_EXPORT QString pluginVersion()
//{
//    return s_pluginVersion;
//}

//SA_PLUGIN_EXPORT QString pluginDescription()
//{
//    return s_pluginDescription;
//}

//SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType()
//{
//    return s_pluginType;
//}


