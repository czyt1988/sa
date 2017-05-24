#ifndef TEXTFILEIMPORTDIALOG_H
#define TEXTFILEIMPORTDIALOG_H

#include <QDialog>
#include <memory>
#include "SATextParser.h"
#include "TextImportConfig.h"
#include <QMutex>
namespace Ui {
class TextFileImportDialog;
}

class SATextDataImport;
class SATxtDataImport;
class SADataTableModel;
class OpenFileManager;
class SAAbstractDatas;
class SATextReadWriter;
class QThread;
///
/// \brief 文本文件导入设置，可以导入txt和csv
///
class TextFileImportDialog : public QDialog
{
    Q_OBJECT
public:
    enum TextType
    {
        Txt
        ,Csv
    };
    explicit TextFileImportDialog(const QString& filePath,TextType type = Txt,QWidget *parent = 0);
    explicit TextFileImportDialog(QWidget *parent = 0);
    ~TextFileImportDialog();

    QList<SAAbstractDatas*> getResult() const;

    bool openFile(const QString& filePath);
    void setTextType(TextType type);
private slots:
    void on_pushButton_Next_clicked();

    void on_spinBox_startReadLine_valueChanged(int arg1);

    void on_comboBox_Separator_currentIndexChanged(int index);


    void on_pushButton_Back_clicked();




    void on_radioButton_1D_toggled(bool checked);

    void on_spinBox_toHeader_valueChanged(int arg1);

    void on_stackedWidget_currentChanged(int arg1);

    void on_spinBox_endLine_valueChanged(int arg1);


    void on_comboBox_codec_currentIndexChanged(const QString &arg1);

    void on_comboBox_codec_currentTextChanged(const QString &arg1);
    //进行了重新解析触发的槽
    void onHaveParser();
    void on_radioButton_oneColumn_clicked(bool checked);

    void onTextReadComplete(const QString& text,bool isOK);
    void onTextThreadDestroyed(QObject* obj);

    void startReadTextThread(const QString &filePath);

    void onAppendTextTimeOut();
private:
    void init();
    void updateData(bool downLoad = true);
    void deal();
    SADataTableModel* getTableModel();

    Ui::TextFileImportDialog *ui;
    bool m_enableInput;///< 防止combo插入导致多次触发输入框
    TextImportConfig m_config;
    QString m_currentSplit;
    int m_maxShowLine;
    int m_realEndLine;
   //QList<SAAbstractDatas*> m_res;
    QList<std::shared_ptr<SAAbstractDatas> > m_res;
    QThread* m_textReaderThread;
    SATextReadWriter* m_textReader;
    QMutex m_mutex;
    QStringList m_readedText;
};

#endif // TEXTFILEIMPORTDIALOG_H
