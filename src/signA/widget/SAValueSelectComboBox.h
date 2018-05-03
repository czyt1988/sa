#ifndef SAVALUESELECTCOMBOBOX_H
#define SAVALUESELECTCOMBOBOX_H

#include <QWidget>
#include <functional>
class SAAbstractDatas;
namespace Ui {
class SAValueSelectComboBox;
}
///
/// \brief 封装变量选择combobox
/// TODO 过滤功能还未开发
///
class SAValueSelectComboBox : public QWidget
{
    Q_OBJECT

public:
    typedef std::function<bool(const SAAbstractDatas*,int)> FunPtrFilter;
    explicit SAValueSelectComboBox(QWidget *parent = 0);
    ~SAValueSelectComboBox();
    //设置过滤器，过滤器会按照规则过滤不显示的条目
    void setFilter(FunPtrFilter filterFun);
    //设置描述
    void setText(const QString& text);
    QString text() const;
    //设置combobox内容
    void setComboBoxText(const QString& text);
    QString comboBoxText() const;
    //获取选中的值
    SAAbstractDatas* getSelectData() const;
private:
    void updateItems();
private:
    Ui::SAValueSelectComboBox *ui;
    FunPtrFilter m_fun;
};
// 只保留1维数据
bool filterSAValueSelectComboBox1Dim(const SAAbstractDatas*p,int index);

#endif // SAVALUESELECTCOMBOBOX_H
