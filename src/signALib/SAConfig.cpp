#include "SAConfig.h"
#include <QDir>
#include <QApplication>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "SALog.h"

#define CONFIG_FOLDER_NAME "config"

SAConfig* SAConfig::s_instance = nullptr;
class SAConfigPrivate
{
public:
    SAConfigPrivate();
    init();
public:
    SAConfig::PropertyBrowserType propertyBrowserType;
};

SAConfigPrivate::SAConfigPrivate()
{
    init();
}

SAConfigPrivate::init()
{
    this->propertyBrowserType = SAConfig::GroupBoxType;
}

SAConfig::SAConfig():QObject(nullptr)
  ,m_d(new SAConfigPrivate)
{
    QString configPath = getConfigFolderPath();
    QDir dir(configPath);
    if(!dir.exists(configPath))
    {
        dir.mkpath(configPath);
    }
    load();
}

SAConfig::~SAConfig()
{

}

SAConfig *SAConfig::getInstance()
{
    if(nullptr == s_instance)
    {
        s_instance = new SAConfig();
    }
    return s_instance;
}
///
/// \brief 加载保存的配置文件
///
void SAConfig::load()
{

}
///
/// \brief 保存配置
/// \return 成功保存返回true
///
bool SAConfig::save()
{

}
///
/// \brief 获取配置文件的路径
/// \return
///
QString SAConfig::getConfigFolderPath()
{
    return QApplication::applicationDirPath() + QDir::separator() + CONFIG_FOLDER_NAME;
}
///
/// \brief 默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
/// \return
///
SAConfig::PropertyBrowserType SAConfig::getDefaultPropertySetDialogType() const
{
    return m_d->propertyBrowserType;
}
///
/// \brief 所有参数设置为默认
///
void SAConfig::setDefault()
{
    m_d->init();
}


