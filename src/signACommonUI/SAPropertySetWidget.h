#ifndef SAPROPERTYSETWIDGET_H
#define SAPROPERTYSETWIDGET_H
#include <QWidget>
#include <memory>
#include <QVariant>
#include "SACommonUIGlobal.h"
class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtVariantProperty;
class QVBoxLayout;
class QLabel;
class QHBoxLayout;
class QSpacerItem;
class QtProperty;
#include <QtVariantProperty>
class SA_COMMON_UI_EXPORT SAPropertySetWidget : public QWidget
{
    Q_OBJECT
public:
    enum BrowserType{
        TreeType
        ,GroupBoxType
        ,ButtonType
    };
    SAPropertySetWidget(QWidget* par=nullptr,BrowserType type = TreeType);
    ~SAPropertySetWidget();
    typedef std::function<void(SAPropertySetWidget*,QtProperty*,const QVariant&)> PropertyChangEventPtr;
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
    //插入枚举类
    QtVariantProperty* appendEnumProperty(const QString& name
                                          ,const QStringList& enumNameList
                                          ,int defautIndex=0
                                          ,const QString& tooltip = QString());
    //插入浮点数属性
    QtVariantProperty* appendDoubleProperty(const QString& name,double defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendDoubleProperty(const QString& name,double min,double max,double defaultData=0,const QString& tooltip = QString());
    //插入int数属性
    QtVariantProperty* appendIntProperty(const QString& name,int defaultData=0,const QString& tooltip = QString());
    QtVariantProperty* appendIntProperty(const QString& name,int min,int max,int defaultData=0,const QString& tooltip = QString());
    //添加bool选项
    QtVariantProperty* appendBoolProperty(const QString& name,bool defaultData=true,const QString& tooltip = QString());
    //添加文字选项
    QtVariantProperty* appendStringProperty(const QString& name, const QString& defaultData=QString(), const QString& tooltip = QString());
    //添加颜色选项
    QtVariantProperty* appendColorProperty(const QString& name, const QColor& defaultData=QColor(255,255,255), const QString& tooltip = QString());
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
signals:
    void valueChanged(QtProperty* prop,const QVariant& var);
private:
    class UI;
    SAPropertySetWidget::UI* ui;
};

#endif // SAPROPERTYSETWIDGET_H
