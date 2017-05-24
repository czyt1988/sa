#include "SAPropertySetDialog.h"
//#include "ui_SAPropertySetDialog.h"
#include <QApplication>
#include <QtTreePropertyBrowser>
#include <QtButtonPropertyBrowser>
#include <QtGroupBoxPropertyBrowser>
#include <QtVariantPropertyManager>
#include <QtVariantEditorFactory>
#include <QtVariantProperty>

#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

void SAPropertySetDialog::UI::setupUI(QDialog *par, BrowserType type)
{
    if (par->objectName().isEmpty())
        par->setObjectName(QString("SAPropertySetDialog"));
    par->resize(381, 360);
    verticalLayout = new QVBoxLayout(par);
    verticalLayout->setObjectName(QString("verticalLayout"));
    labelTitle = new QLabel(par);
    labelTitle->setObjectName(QString("labelTitle"));

    verticalLayout->addWidget(labelTitle);
    switch(type)
    {
    case SAPropertySetDialog::TreeType:
    {
        QtTreePropertyBrowser* pro = new QtTreePropertyBrowser(par);
        propertyBrowser = pro;
        pro->setResizeMode(QtTreePropertyBrowser::Interactive);
        break;
    }
    case SAPropertySetDialog::ButtonType:
        propertyBrowser = new QtButtonPropertyBrowser(par);
        break;
    case SAPropertySetDialog::GroupBoxType:
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

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    pushButton_ok = new QPushButton(par);
    pushButton_ok->setObjectName(QString("pushButton_ok"));

    horizontalLayout->addWidget(pushButton_ok);

    pushButton_cancle = new QPushButton(par);
    pushButton_cancle->setObjectName(QString("pushButton_cancle"));

    horizontalLayout->addWidget(pushButton_cancle);


    verticalLayout->addLayout(horizontalLayout);
    //connect
    QObject::connect(pushButton_ok,&QAbstractButton::clicked,[par](){par->accept();});
    QObject::connect(pushButton_cancle,&QAbstractButton::clicked,[par](){par->reject();});

    par->setWindowTitle(QApplication::translate("SAPropertySetDialog", "property set dialog", 0));
    labelTitle->setText(QString());
    pushButton_ok->setText(QApplication::translate("SAPropertySetDialog", "ok", 0));
    pushButton_cancle->setText(QApplication::translate("SAPropertySetDialog", "cancle", 0));
}

SAPropertySetDialog::SAPropertySetDialog(QWidget *parent,BrowserType type) :
    QDialog(parent),
    ui(new UI)
  ,m_currentGroup(nullptr)
{
    ui->setupUI(this,type);
    m_varPropMgr = new QtVariantPropertyManager(ui->propertyBrowser);
    m_varFac = new QtVariantEditorFactory(ui->propertyBrowser);
    ui->propertyBrowser->setFactoryForManager(m_varPropMgr,m_varFac);
    connect(m_varPropMgr,&QtVariantPropertyManager::valueChanged
            ,this,&SAPropertySetDialog::onPropertyValuechanged);
}

SAPropertySetDialog::~SAPropertySetDialog()
{
}
///
/// \brief 查看当前组
/// \return 如果没有组返回nullptr
///
QtVariantProperty *SAPropertySetDialog::getCurrentGroup()
{
    return m_currentGroup;
}
///
/// \brief 获取组
/// \param index 组的索引顺序，和添加顺序一致
/// \return 如果索引超范围返回nullptr
///
QtVariantProperty *SAPropertySetDialog::getGroup(int index)
{
    if(index < m_group.size())
    {
        return m_group[index];
    }
    return nullptr;
}
///
/// \brief 把当前组游标设置为对应的组
/// 这样使用appendXX属性将会添加到此组下面
/// \note 如果要设置顶层不分组，传入nullptr即可
/// \param p
///
void SAPropertySetDialog::setCurrentGroup(QtVariantProperty *p)
{
    m_currentGroup = p;
}
///
/// \brief 插入一个组,注意，组无法通过getData获取，也就是不计入索引
/// \param name 组名
/// \return 组属性指针
/// \note 此操作会把当前组设置为所新添加的组
///
QtVariantProperty *SAPropertySetDialog::appendGroup(const QString &name)
{
    QtVariantProperty* p = nullptr;
    p = m_varPropMgr->addProperty(QtVariantPropertyManager::groupTypeId(),name);
    m_group.append(p);
    setCurrentGroup(p);
    ui->propertyBrowser->addProperty (p);
    return p;
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
QtVariantProperty* SAPropertySetDialog::appendProperty(const QString &name, const QVariant &varDefaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    if(varDefaultData.isValid())
    {
        p = m_varPropMgr->addProperty ((int)varDefaultData.type(),name);
        p->setValue(varDefaultData);
    }
    else
    {
        p = m_varPropMgr->addProperty(QVariant::String,name);
    }
    p->setToolTip(tooltip);
    m_prop.append (p);
    if(nullptr != m_currentGroup)
    {
        m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}

QtVariantProperty *SAPropertySetDialog::appendProperty(const QString &id, const QString &name, const QVariant &varDefaultData, const QString &tooltip)
{
    QtVariantProperty * prop = appendProperty(name,varDefaultData,tooltip);
    recorder(id,prop);
    return prop;
}
///
/// \brief 插入枚举类属性
/// \param name 属性名
/// \param enumNameList 枚举类描述
/// \param defautIndex 默认的枚举索引
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetDialog::appendEnumProperty(const QString &name, const QStringList &enumNameList, int defautIndex, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = m_varPropMgr->addProperty (QtVariantPropertyManager::enumTypeId(),name);
    p->setAttribute(QLatin1String("enumNames"), enumNameList);
    p->setValue(defautIndex);
    p->setToolTip(tooltip);
    m_prop.append (p);
    if(nullptr != m_currentGroup)
    {
        m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}

QtVariantProperty *SAPropertySetDialog::appendEnumProperty(const QString &id, const QString &name, const QStringList &enumNameList, int defautIndex, const QString &tooltip)
{
    QtVariantProperty *prop = appendEnumProperty(name,enumNameList,defautIndex,tooltip);
    recorder(id,prop);
    return prop;
}
///
/// \brief 插入浮点数属性
/// \param name 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetDialog::appendDoubleProperty(const QString &name, double defaultData,const QString& tooltip)
{
    QtVariantProperty* p = nullptr;
    p = m_varPropMgr->addProperty (QVariant::Double,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    m_prop.append (p);
    if(nullptr != m_currentGroup)
    {
        m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }

    return p;
}

QtVariantProperty *SAPropertySetDialog::appendDoubleProperty(const QString &id, const QString &name, double defaultData, const QString &tooltip)
{
    QtVariantProperty * p = appendDoubleProperty(name,defaultData,tooltip);
    recorder(id,p);
    return p;
}
///
/// \brief 插入浮点数属性
/// \param name 属性名
/// \param min 属性名
/// \param max 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty* SAPropertySetDialog::appendDoubleProperty(const QString& name,double min,double max,double defaultData,const QString& tooltip)
{
    QtVariantProperty* p = appendDoubleProperty(name,defaultData,tooltip);
    p->setAttribute(QLatin1String("minimum"), min);
    p->setAttribute(QLatin1String("maximum"), max);
    return p;
}

QtVariantProperty *SAPropertySetDialog::appendDoubleProperty(const QString &id, const QString &name, double min, double max, double defaultData, const QString &tooltip)
{
    QtVariantProperty * p = appendDoubleProperty(name,min,max,defaultData,tooltip);
    recorder(id,p);
    return p;
}
///
/// \brief 插入int数属性
/// \param name 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetDialog::appendIntProperty(const QString &name, int defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = m_varPropMgr->addProperty (QVariant::Int,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    m_prop.append (p);
    if(nullptr != m_currentGroup)
    {
        m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}

QtVariantProperty *SAPropertySetDialog::appendIntProperty(const QString &id, const QString &name, int defaultData, const QString &tooltip)
{
    QtVariantProperty * p = appendIntProperty(name,defaultData,tooltip);
    recorder(id,p);
    return p;
}
///
/// \brief 插入int数属性
/// \param name 属性名
/// \param min 属性名
/// \param max 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty指针，若为空说明没创建成功
///
QtVariantProperty *SAPropertySetDialog::appendIntProperty(const QString &name, int min, int max, int defaultData, const QString &tooltip)
{
    QtVariantProperty* p = appendIntProperty(name,defaultData,tooltip);
    p->setAttribute(QLatin1String("minimum"), min);
    p->setAttribute(QLatin1String("maximum"), max);
    return p;
}

QtVariantProperty *SAPropertySetDialog::appendIntProperty(const QString &id, const QString &name, int min, int max, int defaultData, const QString &tooltip)
{
    QtVariantProperty* p = appendIntProperty(name,min,max,defaultData,tooltip);
    recorder(id,p);
    return p;
}
///
/// \brief 添加bool选项
/// \param name 属性名
/// \param defaultData 默认值
/// \param tooltip 描述提示
/// \return
///
QtVariantProperty *SAPropertySetDialog::appendBoolProperty(const QString &name, bool defaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    p = m_varPropMgr->addProperty (QVariant::Bool,name);
    p->setValue(defaultData);
    p->setToolTip(tooltip);
    m_prop.append (p);
    if(nullptr != m_currentGroup)
    {
        m_currentGroup->addSubProperty(p);
    }
    else
    {
        ui->propertyBrowser->addProperty (p);
    }
    return p;
}

QtVariantProperty *SAPropertySetDialog::appendBoolProperty(const QString &id, const QString &name, bool defaultData, const QString &tooltip)
{
    QtVariantProperty * p = appendBoolProperty(name,defaultData,tooltip);
    recorder(id,p);
    return p;
}

///
/// \brief 设置默认值
/// \param var
///
void SAPropertySetDialog::setDefaultData(const QVariantList& var)
{
    int count=0;
    for(auto i=m_prop.begin ();i!=m_prop.end () && count<var.size ();++i)
    {
        (*i)->setValue (var[count]);
        ++count;
    }
}


///
/// \brief 把所有设置的值转换为variant list发送
/// \return
///
QVariantList SAPropertySetDialog::getDatas() const
{
    QVariantList res;
    for(int i=0;i<m_prop.size ();++i)
    {
        if(m_prop[i])
            res.append (m_prop[i]->value ());
    }
    return res;
}
///
/// \brief 获取一个数值
/// \param index 索引
/// \return  如果超过索引返回QVariant()
///
QVariant SAPropertySetDialog::getData(int index) const
{
    if(index < m_prop.size())
    {
        return m_prop.at(index)->value();
    }
    return QVariant();
}
///
/// \brief 删除所有属性内容
///
void SAPropertySetDialog::deleteAll()
{
    //m_propID.deleteAllProperty();
    m_prop.clear ();
}
///
/// \brief 获取所有设置进去的属性
/// \return
///
const QList<QtVariantProperty *> &SAPropertySetDialog::getPropertys() const
{
    return m_prop;
}
///
/// \brief 可编辑属性
/// 此函数一般只在重载时调用，直接使用\sa getDatas 函数获取变量即可
/// \return
///
QList<QtVariantProperty *> &SAPropertySetDialog::getPropertys()
{
    return m_prop;
}
///
/// \brief 获取属性管理器
/// 此函数一般只在重载时调用，直接使用\sa getDatas 函数获取变量即可
/// \return
///
QtVariantPropertyManager *SAPropertySetDialog::getVariantPropertyManager() const
{
    return m_varPropMgr;
}
///
/// \brief 获取属性编辑器
/// \return
///
QtVariantEditorFactory *SAPropertySetDialog::getVariantEditorFactory() const
{
    return m_varFac;
}
///
/// \brief 设置属性改变的触发事件
/// \param prop 属性
/// \param funEvent 事件
///
void SAPropertySetDialog::setPropertyChangEvent(QtProperty *prop, SAPropertySetDialog::PropertyChangEventPtr funEvent)
{
    m_event[prop] = funEvent;
}
///
/// \brief 记录属性，可以通过getDataByID<>(id:QString)获取对应的value
/// \param id
/// \param pro
///
void SAPropertySetDialog::recorder(const QString &id, QtVariantProperty *pro)
{
    if(nullptr == m_recordStr2Pro)
    {
        m_recordStr2Pro.reset(new QHash<QString,QtVariantProperty*>());
    }
    m_recordStr2Pro->insert(id,pro);
}
///
/// \brief 通过id获取property
/// \note 需要调用recorder后，此函数才能有效
/// \param id
/// \return
/// \see recorder
///
QtVariantProperty *SAPropertySetDialog::getPropertyByID(const QString &id)
{
    if(nullptr == m_recordStr2Pro)
    {
        return nullptr;
    }
    return m_recordStr2Pro->value(id,nullptr);
}
///
/// \brief 属性改变触发的槽函数
/// \param prop
/// \param var
///
void SAPropertySetDialog::onPropertyValuechanged(QtProperty *prop, const QVariant &var)
{
    EVENT_MAP_ITE ite = m_event.find(prop);
    if(ite == m_event.end())
    {
        return;
    }
    PropertyChangEventPtr fun = ite.value();
    if(nullptr != fun)
    {
        fun(this,prop,var);
    }
}



