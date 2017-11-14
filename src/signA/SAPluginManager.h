#ifndef SAPLUGINMANAGER_H
#define SAPLUGINMANAGER_H

#include <QObject>
#include <QLibrary>
#include "SAPluginInterface.h"
#include "../global/SAGlobals.h"
class SAUIInterface;
///
/// \brief SA的插件管理类，此类管理所有外加的插件
///
class SAPluginManager : public QObject
{
    Q_OBJECT
public:
    SAPluginManager(SAUIInterface* ui,QObject* par);
    //加载插件
    int loadPlugins();
    //判断是否加载了插件
    bool isNormalPluginLoaded(const QString& pluginFileName) const;
    //卸载插件
    //bool unloadPlugin(SAAbstractPlugin* plugin);
    //获取导入插件的打开文件过滤表
    QStringList getOpenFileNameFilters() const;
    //通过后缀获取打开文件对应的插件
    SAAbstractDataImportPlugin* getDataImportPluginFromSuffix(const QString& suffix) const;
private:
    //加载正常插件
    int loadNormalPlugin();
    //加载一个插件
    bool loadOneNormalPlugin(const QString& fullPath);
    //加载数据导入插件
    bool loadDataImportPlugin(QLibrary& lib, const QString& baseName, const QString& fullPath);
    //函数插件加载
    bool loadFunctionPlugin(QLibrary& lib, const QString& baseName, const QString& fullPath);
    //临时删除插件，此函数主要在插件加载成功后但后续的操作出问题时调用
    void deleteTempPlugin(QLibrary& lib,SAAbstractPlugin* plugin);
signals:
    ///
    /// \brief 错误信息
    /// \param 信息内容
    ///
    void postInfoMessage(const QString& info,SA::MeaasgeType messageType);

private:
    template<typename PLUGIN_PTR>
    class PluginInfo{
    public:
        PluginInfo():plugin(nullptr)
          ,pluginPath("")
          ,pluginFileName("")
          ,type(SAAbstractPlugin::UnKnow)
        {}
        PLUGIN_PTR plugin;///< 保存插件指针
        QString pluginPath;
        QString pluginFileName;
        SAAbstractPlugin::PluginType type;
    };
    QList<PluginInfo<QObject*> > m_pluginQt;///< 保存qt plugin loader加载的插件
    QList<PluginInfo<SAAbstractPlugin*> > m_normalPluginList;///< 保存加载的插件信息
    typedef QHash<QString,SAAbstractDataImportPlugin*> ImportSuffix2ImportPluginMap;
    ImportSuffix2ImportPluginMap m_importSuffix2PluginObj;///< 记录支持默认打开对话框的文件后缀关联的插件指针
    QStringList m_openFileNameFilters;///< 用于保存打开action对应的文件后缀列表，由于插件支持，因此变量作为成员变量保存
    SAUIInterface* m_ui;
};

#endif // SAPLUGINMANAGER_H
