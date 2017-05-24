#ifndef CFX2DCSVIMPORTDIALOG_H
#define CFX2DCSVIMPORTDIALOG_H

#include <QDialog>
#include <CfxCsvDataImportConfig.h>
#include <memory>
#include <QStringList>

class QStringListModel;
class SAAbstractDatas;
class SADataTableModel;

namespace Ui {
class CFX2DCsvImportDialog;
}

class CFX2DCsvImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFX2DCsvImportDialog(QWidget *parent = 0);
    ~CFX2DCsvImportDialog();
    QList<SAAbstractDatas*> getResult() const;
public slots:
    void addCsvFile();
    void removeFile();
    void parser();
private slots:
    void on_comboBox_codec_currentIndexChanged(const QString &arg1);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    void initUI();
    QStringListModel* getStringListModel();
    SADataTableModel* getDataTableModel();
    void clearAndDeleteDatas();
private:
    QString m_codec;
    Ui::CFX2DCsvImportDialog *ui;
    QList<SAAbstractDatas*> m_datas;
    std::unique_ptr<CfxCsvDataImportConfig> m_dataConfigs;
    QStringList m_pathList;
    bool m_isDeleteMakeValue;
};

#endif // DIALOG_CFX2DCSVIMPORT_H
