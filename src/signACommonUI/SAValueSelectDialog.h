#ifndef SAVALUESELECTDIALOG_H
#define SAVALUESELECTDIALOG_H

#include <QDialog>
#include <QList>
#include <QSet>
#include "SACommonUIGlobal.h"

class SADataTableModel;
class SAValueManagerModel;
class SAAbstractDatas;


namespace Ui {
class SAValueSelectDialog;
}

class SA_COMMON_UI_EXPORT SAValueSelectDialog : public QDialog
{
    Q_OBJECT

public:
    enum SelectionMode
    {
        SingleSelection
        ,MultiSelection
    };

    explicit SAValueSelectDialog(QWidget *parent = 0);
    ~SAValueSelectDialog();
    //获取选择的数据
    QList<SAAbstractDatas *> getSelectDatas();
    //获取选择的数据
    static QList<SAAbstractDatas *> getSelectDatas(QWidget *parent
                                                   ,const SelectionMode selectMode = MultiSelection);
    //变量选择的树形控件的选择模式
    SAValueSelectDialog::SelectionMode getSelectMode() const;
    //变量选择的树形控件的选择模式设置，可设置为单选或多选
    void setSelectMode(const SAValueSelectDialog::SelectionMode selectMode);
    //设置默认选中的数据
    void setDefaultSelectDatas(const QList<SAAbstractDatas *>& defaultSelDatas);

    QColor getHightLightColor() const;
    void setHightLightColor(const QColor &hightLightColor);

private:
    void initUI();
private slots:

    void onTreeClicked(const QModelIndex& index);
    void onAccept();
private:
    Ui::SAValueSelectDialog *ui;
    SADataTableModel* m_dataModel;
    SAValueManagerModel* m_valueModel;
    SelectionMode m_selectMode;
    QSet<SAAbstractDatas *> m_hightLightData;
    QColor m_hightLightColor;
};

#endif // SAVALUESELECTDIALOG_H
