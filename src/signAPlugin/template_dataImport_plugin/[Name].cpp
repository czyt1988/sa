#include "[Name].h"
#include "SA[Name]Factory.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include "SAUIReflection.h"

#define TR(str)\
    QApplication::translate("[Name]",str,0)
static const QString s_pluginName = TR("");
static const QString s_pluginDescription =  TR("");
static const QString s_pluginVersion = TR("");
static const SAAbstractPlugin::PluginType s_pluginType = SAAbstractPlugin::DataImport;

SA_PLUGIN_EXPORT SAAbstractPlugin *createPlugin()
{
    return new [Name]();
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
