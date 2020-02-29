#include "SAPropertySetWidget.h"
#include <QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <qtpropertybrowser.h>
#include <qttreepropertybrowser.h>
#include <qtbuttonpropertybrowser.h>
#include <qtgroupboxpropertybrowser.h>
#include <qtpropertymanager.h>
#include <qteditorfactory.h>
#include <qtvariantproperty.h>
#include <QtProperty>
//#include <QtTreePropertyBrowser>
//#include <QtButtonPropertyBrowser>
//#include <QtGroupBoxPropertyBrowser>
//#include <QtVariantPropertyManager>
//#include <QtVariantEditorFactory>
//#include <QtVariantProperty>
class SAPropertySetWidget::UI
{
public:
    QVBoxLayout *verticalLayout;
    QtAbstractPropertyBrowser *propertyBrowser;
    //
    QtVariantPropertyManager* m_varPropMgr;
    QtVariantEditorFactory* m_varFac;
    QList<QtVariantProperty*> m_prop;
    QList<QtVariantProperty*> m_group;
    //
    QtVariantProperty* m_currentGroup;///< 记录游标所处于的当前组，如果为nullptr，那么就在顶层

    void setupUI(QWidget *par,SAPropertySetWidget::BrowserType type)
    {
        if (par->objectName().isEmpty())
            par->setObjectName(QString("SAPropertySetDialog"));
        par->resize(381, 360);
        verticalLayout = new QVBoxLayout(par);
        verticalLayout->setObjectName(QString("verticalLayout"));
        switch(type)
        {
        case SAPropertySetWidget::TreeType:
        {
            QtTreePropertyBrowser* pro = new QtTreePropertyBrowser(par);
            propertyBrowser = pro;
            pro->setResizeMode(QtTreePropertyBrowser::Interactive);
            break;
        }
        case SAPropertySetWidget::ButtonType:
            propertyBrowser = new QtButtonPropertyBrowser(par);
            break;
        case SAPropertySetWidget::GroupBoxType:
            propertyBrowser = new QtGroupBoxPropertyBrowser(par);
            break;
        }
        propertyBrowser->setObjectName(QString("propertyBrowser"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(propertyBrowser->sizePolicy().hasHeightForWidth());
        propertyBrowser->setSizePolicy(sizePolicy);



        verticalLayout->addWidget(propertyBrowser);
        //
        m_varPropMgr = new QtVariantPropertyManager(propertyBrowser);
        m_varFac = new QtVariantEditorFactory(propertyBrowser);
        propertyBrowser->setFactoryForManager(m_varPropMgr,m_varFac);
        //connect
        par->setWindowTitle(QApplication::translate("SAPropertySetDialog", "property set dialog", 0));
    }
};


//


SAPropertySetWidget::SAPropertySetWidget(QWidget *par,BrowserType type)
    :QWidget(par)
    ,ui(new UI)
{
    ui->setupUI(this,type); 
//    connect(ui->m_varPropMgr,&QtVariantPropertyManager::valueChanged
//                    ,this,&SAPropertySetWidget::valueChanged);
    connect(ui->m_varPropMgr,SIGNAL(valueChanged(QtProperty*,QVariant))
            ,this,SIGNAL(valueChanged(QtProperty*,QVariant)));
}

SAPropertySetWidget::~SAPropertySetWidget()
{
    if(ui)
    {
        delete ui;
    }
}
///
/// \brief 插入一个组,注意，组无法通过getData获取，也就是不计入索引
/// \param name 组名
/// \return 组属性指针
/// \note 此操作会把当前组设置为所新添加的组
///
QtVariantProperty *SAPropertySetWidget::appendGroup(const QString &name)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty(QtVariantPropertyManager::groupTypeId(),name);
    ui->m_group.append(p);
    setCurrentGroup(p);
    ui->propertyBrowser->addProperty (p);
    return p;
}
///
/// \brief 查看当前组
/// \return 如果没有组返回nullptr
///
QtVariantProperty *SAPropertySetWidget::getCurrentGroup()
{
    return ui->m_currentGroup;
}
///
/// \brief 获取组
/// \param index 组的索引顺序，和添加顺序一致
/// \return 如果索引超范围返回nullptr
///
QtVariantProperty *SAPropertySetWidget::getGroup(int index)
{
    if(index < ui->m_group.size())
    {
        return ui->m_group[index];
    }
    return nullptr;
}
///
/// \brief 把当前组游标设置为对应的组
/// 这样使用appendXX属性将会添加到此组下面
/// \note 如果要设置顶层不分组，传入nullptr即可
/// \param p
///
void SAPropertySetWidget::setCurrentGroup(QtVariantProperty *p)
{
    ui->m_currentGroup = p;
}
///
/// \brief 插入一个属性内容
///
/// 属性类型根据默认值的属性定制，如果默认值为空，则为字符串属性
/// \param name 属性名
/// \param varDefaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetWidget::appendProperty(const QString &name, const QVariant &varDefaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    if(varDefaultData.isValid())
    {
        p = ui->m_varPropMgr->addProperty ((int)varDefaultData.type(),name);
        p->setValue(varDefaultData);
    }
    else
    {
        p = ui->m_varPropMgr->addProperty(QVariant::String,name);
    }
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}

///
/// \brief 插入枚举类属性
/// \param name 属性名
/// \param enumNameList 枚举类描述
/// \param defautIndex 默认的枚举索引
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetWidget::appendEnumProperty(const QString &name, const QStringList &enumNameList, int defautIndex, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty (QtVariantPropertyManager::enumTypeId(),name);
    p->setAttribute(QLatin1String("enumNames"), enumNameList);
    p->setValue(defautIndex);
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}



QtVariantProperty *SAPropertySetWidget::appendDoubleProperty(const QString &name, double defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty (QVariant::Double,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }

    return p;
}



QtVariantProperty *SAPropertySetWidget::appendDoubleProperty(const QString &name, double min, double max, double defaultData, const QString &tooltip)
{
    QtVariantProperty* p = appendDoubleProperty(name,defaultData,tooltip);
    p->setAttribute(QLatin1String("minimum"), min);
    p->setAttribute(QLatin1String("maximum"), max);
    return p;
}


///
/// \brief 插入int数属性
/// \param name 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetWidget::appendIntProperty(const QString &name, int defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty (QVariant::Int,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}



QtVariantProperty *SAPropertySetWidget::appendIntProperty(const QString &name, int min, int max, int defaultData, const QString &tooltip)
{
    QtVariantProperty* p = appendIntProperty(name,defaultData,tooltip);
    p->setAttribute(QLatin1String("minimum"), min);
    p->setAttribute(QLatin1String("maximum"), max);
    return p;
}


///
/// \brief 添加bool选项
/// \param name 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return
///
QtVariantProperty *SAPropertySetWidget::appendBoolProperty(const QString &name, bool defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty (QVariant::Bool,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}
///
/// \brief 添加文字属性
/// \param name 名称
/// \param defaultVal 默认值
/// \param tooltip 说明
/// \return
///
QtVariantProperty *SAPropertySetWidget::appendStringProperty(const QString &name, const QString &defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty (QVariant::String,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}
///
/// \brief 添加颜色选项
/// \param name 名字
/// \param defaultData 默认值
/// \param tooltip 说明
/// \return
///
QtVariantProperty *SAPropertySetWidget::appendColorProperty(const QString &name, const QColor &defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = ui->m_varPropMgr->addProperty (QVariant::Color,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    ui->m_prop.append (p);
    if(nullptr != getCurrentGroup())
    {
        ui->m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}

///
/// \brief 设置默认值
/// \param var
///
void SAPropertySetWidget::setDefaultData(const QVariantList &var)
{
    int count=0;
    for(auto i=ui->m_prop.begin ();i!=ui->m_prop.end () && count<var.size ();++i)
    {
        (*i)->setValue (var[count]);
        ++count;
    }
}
///
/// \brief 把所有设置的值转换为variant list发送
/// \return
///
QVariantList SAPropertySetWidget::getDatas() const
{
    QVariantList res;
    for(int i=0;i<ui->m_prop.size ();++i)
    {
        if(ui->m_prop[i])
            res.append (ui->m_prop[i]->value ());
    }
    return res;
}
///
/// \brief 获取一个数值
/// \param index 索引
/// \return  如果超过索引返回QVariant()
///
QVariant SAPropertySetWidget::getData(int index) const
{
    if(index < ui->m_prop.size())
    {
        return ui->m_prop.at(index)->value();
    }
    return QVariant();
}
///
/// \brief 删除所有属性内容
///
void SAPropertySetWidget::deleteAll()
{
    ui->m_prop.clear ();
}
///
/// \brief 获取所有设置进去的属性
/// \return
///
const QList<QtVariantProperty *> &SAPropertySetWidget::getPropertys() const
{
    return ui->m_prop;
}
///
/// \brief 可编辑属性
/// 此函数一般只在重载时调用，直接使用\sa getDatas 函数获取变量即可
/// \return
///

QList<QtVariantProperty *> &SAPropertySetWidget::getPropertys()
{
    return ui->m_prop;
}
///
/// \brief 获取属性管理器
/// 此函数一般只在重载时调用，直接使用\sa getDatas 函数获取变量即可
/// \return
///
QtVariantPropertyManager *SAPropertySetWidget::getVariantPropertyManager() const
{
    return ui->m_varPropMgr;
}
///
/// \brief 获取属性编辑器
/// \return
///
QtVariantEditorFactory *SAPropertySetWidget::getVariantEditorFactory() const
{
    return ui->m_varFac;
}


#include "moc_SAPropertySetWidget.cpp"
