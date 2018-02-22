#include "SignalFunPlugin.h"
#include <QApplication>
#include "SAUIReflection.h"
#define TR(str)\
    QApplication::translate("SignalFunPlugin",str,0)
static const QString s_pluginName = TR("");
static const QString s_pluginDescription =  TR("");
static const QString s_pluginVersion = TR("");
static const SAAbstractPlugin::PluginType s_pluginType = SAAbstractPlugin::Function;
SA_PLUGIN_EXPORT SAAbstractPlugin *createPlugin()
{
    return new SignalFunPlugin();
}
SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin *obj)
{
    if(obj != nullptr)
    {
        delete obj;
    }
}
SA_PLUGIN_EXPORT QString pluginName()
{
    return s_pluginName;
}
SA_PLUGIN_EXPORT QString pluginVersion()
{
    return s_pluginVersion;
}
SA_PLUGIN_EXPORT QString pluginDescription()
{
    return s_pluginDescription;
}
SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType()
{
    return s_pluginType;
}

///
/// \brief 是通过SA系统自带的OpenFile还是使用自身openFile
///
/// 如果返回true。这时sa将会调用bool openFile(const QStringList* filePaths),并传入用户选择的路径，
/// 如果需要多选，需要isMultiselect返回true
/// 如果返回false，sa将会调用bool openFile()
/// \return
///
SignalFunPlugin::SignalFunPlugin()
{

}

SignalFunPlugin::~SignalFunPlugin()
{

}
///
/// \brief 返回插件类型
/// \return
///
SAAbstractPlugin::PluginType SignalFunPlugin::getType() const
{
    return s_pluginType;
}
///
/// \brief 获取插件名字
/// \return
///
QString SignalFunPlugin::getPluginName() const
{
    return s_pluginName;
}
///
/// \brief 获取插件版本
/// \return
///
QString SignalFunPlugin::getVersion() const
{
    return s_pluginVersion;
}
///
/// \brief 获取描述信息
/// \return
///
QString SignalFunPlugin::getDescription() const
{
    return s_pluginDescription;
}
///
/// \brief ui构建
///
void SignalFunPlugin::setupUI()
{
    
}
