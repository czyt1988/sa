#include "SADataImportDelegate.h"
#include <memory>
#include <mainwindow.h>

#include <QInputDialog>

#include "SAData.h"

#include "SACsvFileTableModel.h"
#include "SAValueManager.h"
SADataImportDelegate::SADataImportDelegate(MainWindow* wnd)
    :SAMainWindowDelegate(wnd)
{
}

SADataImportDelegate::~SADataImportDelegate()
{

}
///
/// \brief SADataImportDelegate::textImport
/// \param path
/// \return
///
bool SADataImportDelegate::textImport(const QString& path)
{
//    std::unique_ptr<SATxtDataImport> txtImport;
//    txtImport.reset (new SATxtDataImport);
//    if(!txtImport->openFile(path))
//        return false;
//    TextFileImportDialog dlg(txtImport.get (),getMainWindow ());
//    if(QDialog::Accepted != dlg.exec ())
//    {
//        return false;
//    }
//    QList<SAAbstractDatas*> res = dlg.getResult ();
//    saValueManager->addDatas (res);
    return false;
}
///
/// \brief 导入csv文件
/// \param path
/// \return
///
bool SADataImportDelegate::csvImport(const QString &path)
{
//    std::unique_ptr<SACsvDataImport> importer;
//    importer.reset(new SACsvDataImport);
//    if(!importer->openFile(path))
//        return false;
//    TextFileImportDialog dlg(importer.get (),getMainWindow ());
//    if(QDialog::Accepted != dlg.exec ())
//    {
//        return false;
//    }
//    QList<SAAbstractDatas*> res = dlg.getResult ();
//    saValueManager->addDatas (res);
    return false;
}

bool SADataImportDelegate::csvImportAsTable(const QString &path)
{
    czy::QtApp::QczyHashTable<QVariant> table;
    SACsvFileTableModel::getCzyHashTable(path,table);
    if(table.rowCount () == 0 )
        return false;
    QString name = QInputDialog::getText (getMainWindow()
                                          ,QObject::tr("input variable name")
                                          ,QObject::tr("input name:")
                                          ,QLineEdit::Normal
                                          ,QObject::tr("table"));
    SATableVariant* data = new SATableVariant(name);
    data->setTable(table);
    saValueManager->addData(data);
    return true;
}

//bool SADataImportDelegate::cfxCsv2DImport()
//{
//    std::unique_ptr<SACfxCsvDataImport> import;
//    import.reset (new SACfxCsvDataImport);
//    Dialog_CFX2DCsvImport dlg(import.get (),getMainWindow ());
//    if(QDialog::Accepted != dlg.exec ())
//        return false;
//    QList<SAAbstractDatas*> res = dlg.getResult ();
//    saValueManager->addDatas (res);
//    return true;
//}


