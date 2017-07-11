#ifndef SAFUNCTIONPARAMINPUTDIALOG_H
#define SAFUNCTIONPARAMINPUTDIALOG_H

#include <QDialog>
#include <QList>
#include <QPair>
#include <QVariantList>
#include <memory>
#include <QStringList>
#include "SACommonUIGlobal.h"
#include <functional>
#include "SAPropertySetWidget.h"
class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtVariantProperty;
class QVBoxLayout;
class QLabel;
class QHBoxLayout;
class QSpacerItem;
class SAPropertySetWidget;
///
/// \brief 生成一个通用Property设置对话框
///
class SA_COMMON_UI_EXPORT SAPropertySetDialog : public QDialog
{
    Q_OBJECT
public:
    enum BrowserType{
        TreeType = SAPropertySetWidget::TreeType
        ,GroupBoxType = SAPropertySetWidget::GroupBoxType
        ,ButtonType = SAPropertySetWidget::ButtonType
    };
    typedef std::function<void(SAPropertySetDialog*,QtProperty*,const QVariant&)> PropertyChangEventPtr;
    explicit SAPropertySetDialog(QWidget *parent = 0,BrowserType type = TreeType);
    ~SAPropertySetDialog();
    //插入一个组,注意，组无法通过getData获取，也就是不计入索引
    QtVariantProperty* appendGroup(const QString &name);
    //查看当前组，如果没有组返回nullptr
    QtVariantProperty* getCurrentGroup();
    //获取组
    QtVariantProperty* getGroup(int index);
    //把当前组游标设置为对应的组
    void setCurrentGroup(QtVariantProperty* p);
    //插入一个属性内容
    QtVariantProperty* appendProperty(const QString& name,const QVariant& varDefaultData = QVariant(),const QString& tooltip = QString());
    QtVariantProperty* appendProperty(const QString& id,const QString& name,const QVariant& varDefaultData = QVariant(),const QString& tooltip = QString());
    //插入枚举类
    QtVariantProperty* appendEnumProperty(const QString& name
                                          ,const QStringList& enumNameList
                                          ,int defautIndex=0
                                          ,const QString& tooltip = QString());
    QtVariantProperty* appendEnumProperty(const QString& id
                                          ,const QString& name
                                          ,const QStringList& enumNameList
                                          ,int defautIndex=0
                                          ,const QString& tooltip = QString());
    //插入浮点数属性
    QtVariantProperty* appendDoubleProperty(const QString& name,double defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendDoubleProperty(const QString& id,const QString& name,double defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendDoubleProperty(const QString& name,double min,double max,double defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendDoubleProperty(const QString& id,const QString& name,double min,double max,double defaultData=0,const QString& tooltip = QString());
    //插入int数属性
    QtVariantProperty* appendIntProperty(const QString& name,int defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendIntProperty(const QString& id,const QString& name,int defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendIntProperty(const QString& name,int min,int max,int defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendIntProperty(const QString& id,const QString& name,int min,int max,int defaultData=0,const QString& tooltip = QString());
    //添加bool选项
    QtVariantProperty* appendBoolProperty(const QString& name,bool defaultData=true,const QString& tooltip = QString());
    QtVariantProperty* appendBoolProperty(const QString& id,const QString& name,bool defaultData=true,const QString& tooltip = QString());
    //设置默认值
    void setDefaultData(const QVariantList& var);
    //把所有设置的值转换为variant list发送
    QVariantList getDatas() const;
    QVariant getData(int index) const;
    //删除所有属性内容
    void deleteAll();

    //获取所有设置的属性
    const QList<QtVariantProperty*>& getPropertys() const;
    QList<QtVariantProperty*>& getPropertys();

    //获取ropertyManager
    QtVariantPropertyManager* getVariantPropertyManager() const;

    //获取属性编辑器
    QtVariantEditorFactory* getVariantEditorFactory() const;

    //设置属性改变的触发事件
    void setPropertyChangEvent(QtProperty *prop,PropertyChangEventPtr funEvent);
    //记录属性，可以通过get(id:QString)获取对应的QVariant
    void recorder(const QString& id,QtVariantProperty* pro);

    ///
    /// \brief 通过id获取对应的值
    /// \param id id
    /// \return 值 如果没有对应的值将返回默认构造
    /// \see recorder
    ///
    template<typename T>
    T getDataByID(const QString& id,const T& defaultData = T())
    {
        QtVariantProperty* pro = getPropertyByID(id);
        if(nullptr == pro)
        {
            return defaultData;
        }
        return pro->value().value<T>();
    }
    //通过id获取property
    QtVariantProperty* getPropertyByID(const QString& id);
    //获取内部的SAPropertySetWidget属性widget
    SAPropertySetWidget* getPropertyWidget() const;
private slots:
    void onPropertyValuechanged(QtProperty* prop,const QVariant& var);
private:
    class UI;
    UI* ui;
};

#endif // SAFUNCTIONPARAMINPUTDIALOG_H
