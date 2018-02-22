#ifndef SADATAIMPORTFACTORY_H
#define SADATAIMPORTFACTORY_H


#if defined(SA_COMMON_PLUGIN_LIB_MAKE)     // create a DLL library
    #ifndef SA_PLUGIN_EXPORT
    #define SA_PLUGIN_EXPORT Q_DECL_EXPORT
    #endif
#else                        // use a DLL library
    #ifndef SA_PLUGIN_EXPORT
    #define SA_PLUGIN_EXPORT Q_DECL_IMPORT
    #endif
#endif

#include  "SAPluginInterface.h"
//这是插件仅有的两个导出函数
extern "C" SA_PLUGIN_EXPORT SAAbstractPlugin* createPlugin();
extern "C" SA_PLUGIN_EXPORT void deletePlugin(SAAbstractPlugin* obj);
extern "C" SA_PLUGIN_EXPORT QString pluginName();
extern "C" SA_PLUGIN_EXPORT QString pluginVersion();
extern "C" SA_PLUGIN_EXPORT QString pluginDescription();
extern "C" SA_PLUGIN_EXPORT SAAbstractPlugin::PluginType pluginType();


#endif // SADATAIMPORTFACTORY_H
