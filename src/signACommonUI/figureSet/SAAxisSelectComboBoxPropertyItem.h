#ifndef SAAXISSELECTCOMBOBOXPROPERTYITEM_H
#define SAAXISSELECTCOMBOBOXPROPERTYITEM_H
#include "SAComboBoxPropertyItem.h"
#include "SACommonUIGlobal.h"
class SA_COMMON_UI_EXPORT SAAxisSelectComboBoxPropertyItem : public SAComboBoxPropertyItem
{
    Q_OBJECT
public:
    enum SelectMode{
        XAxisOnly ///< 只可选x轴
        ,YAxisOnly///< 只可选y轴
        ,AllAxis///< 可选所有轴
    };
    SAAxisSelectComboBoxPropertyItem(SelectMode mode,QWidget* par = nullptr);
public slots:
    void setCurrentSelectAxis(int axisID);
private slots:
    void onCurrentIndexChanged(int index);
signals:
    void axisSelectChanged(int axisID);
private:
    SelectMode m_mode;
};

#endif // SAAXISSELECTCOMBOBOXPROPERTYITEM_H
