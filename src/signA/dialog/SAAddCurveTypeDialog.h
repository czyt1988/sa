#ifndef SAADDCURVETYPEDIALOG_H
#define SAADDCURVETYPEDIALOG_H

#include <QDialog>
class QButtonGroup;
namespace Ui {
class SAAddCurveTypeDialog;
}
///
/// \brief 用于选择添加曲线的方式，有添加到新图，添加到当前图，和添加到子图
///
class SAAddCurveTypeDialog : public QDialog
{
    Q_OBJECT

public:
    enum AddCurveType{
        AddInNewFig ///< 添加到新图
        ,AddInCurrentFig ///< 添加到当前图
        ,AddInCurrentFigWithSubplot ///< 添加到当前图，以子图形式
        ,Unknow ///< 未知
    };
    explicit SAAddCurveTypeDialog(QWidget *parent = 0);
    ~SAAddCurveTypeDialog();
    //获取当前选择的类型
    AddCurveType getSelectType() const;
    //设置当前选择的类型
    void setSelectType(AddCurveType type);
    //
    static AddCurveType getAddCurveType(QWidget* par,AddCurveType defaultType = AddInNewFig,const QString& title = QString());
private:
    Ui::SAAddCurveTypeDialog *ui;
    QButtonGroup* m_btnGroup;
};

#endif // SAADDCURVETYPEDIALOG_H
