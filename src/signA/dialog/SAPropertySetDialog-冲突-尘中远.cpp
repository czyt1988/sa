#include "SAPropertySetDialog.h"
//#include "ui_SAPropertySetDialog.h"
#include <QApplication>
#include <QtTreePropertyBrowser>
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

void SAPropertySetDialog::UI::setupUI(QDialog *par)
{
    if (par->objectName().isEmpty())
        par->setObjectName(QString("SAPropertySetDialog"));
    par->resize(381, 360);
    verticalLayout = new QVBoxLayout(par);
    verticalLayout->setObjectName(QString("verticalLayout"));
    labelTitle = new QLabel(par);
    labelTitle->setObjectName(QString("labelTitle"));

    verticalLayout->addWidget(labelTitle);

    propertyTree = new QtTreePropertyBrowser(par);
    propertyTree->setObjectName(QString("propertyTree"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(propertyTree->sizePolicy().hasHeightForWidth());
    propertyTree->setSizePolicy(sizePolicy);

    verticalLayout->addWidget(propertyTree);

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

SAPropertySetDialog::SAPropertySetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new UI)
{
    ui->setupUI(this);
    m_varProp = new QtVariantPropertyManager(ui->propertyTree);
    m_varFac = new QtVariantEditorFactory(ui->propertyTree);
    ui->propertyTree->setFactoryForManager(m_varProp,m_varFac);
}

SAPropertySetDialog::~SAPropertySetDialog()
{
}
///
/// \brief 插入一个属性内容
/// \param name 属性名
/// \param varDefaultData 默认值
/// \param tooltip 描述提示
/// \return 返回QtVariantProperty
///
QtVariantProperty* SAPropertySetDialog::appendProperty(const QString &name, const QVariant &varDefaultData, const QString &tooltip)
{
    QtVariantProperty* p = nullptr;
    if(varDefaultData.isValid())
    {
        p = m_varProp->addProperty ((int)varDefaultData.type(),name);
    }
    else
    {
        p = m_varProp->addProperty(QVariant::String,name);
    }
    m_prop.append (p);
    ui->propertyTree->addProperty (p);
    p->setToolTip(tooltip);
    return p;
}
///
/// \brief 设置参数描述
/// \param des
///
QList<QtVariantProperty*> SAPropertySetDialog::setDescribe(const QList<QPair<QString,int>>& des)
{
    deleteAll();
    QList<QtVariantProperty*>  props;
    for (auto i=des.begin ();i!=des.end ();++i)
    {

        QtVariantProperty* p = m_varProp->addProperty (i->second,i->first);
        m_prop.append (p);
        props.append (p);
        ui->propertyTree->addProperty (p);
    }
    return props;
}

///
/// \brief 设置默认值
/// 