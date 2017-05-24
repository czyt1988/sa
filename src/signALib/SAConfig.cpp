#include "SAConfig.h"
#include <QDir>
#include <QApplication>
#define CONFIG_FOLDER_NAME "config"
SAConfig* SAConfig::s_instance = nullptr;

SAConfig::SAConfig():QObject(nullptr)
{
    QString configPath = getConfigFolderPath();
    QDir dir(configPath);
    if(!dir.exists(configPath))
    {
        dir.mkpath(configPath);
    }
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

}
