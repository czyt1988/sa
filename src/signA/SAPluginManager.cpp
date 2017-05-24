#include "SAPluginManager.h"
#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDebug>
#include <algorithm>
SAPluginManager::SAPluginManager(SAUIInterface *ui, QObject *par):QObject(par)
  ,m_ui(ui)
{

}
///
/// \brief 加载插件
/// \return 返回加载插件的数量
///
int SAPluginManager::loadPlugins()
{
    int count = 0;
    count += loadNormalPlugin();
    return count;
}
///
/// \brief 判断是否加载了插件
/// \param pluginFileName 插件名字
/// \return
///
bool SAPluginManager::isNormalPluginLoaded(const QString &pluginFileName) const
{
    const int size = m_normalPluginList.size();
    for (int i=0;i<size;++i)
    {
        if(m_normalPluginList[i].pluginFileName == pluginFileName)
        {
            return true;
        }
    }
    return false;
}
///
/// \brief 获取导入插件的打开文件过滤表
/// \return
///
QStringList SAPluginManager::getOpenFileNameFilters() const
{
    return m_openFileNameFilters;
}
///
/// \brief 通过后缀获取打开文件对应的插件
/// \param suffix 后缀
/// \return 没有找到返回nullptr
///
SAAbstractDataImportPlugin *SAPluginManager::getDataImportPluginFromSuffix(const QString &suffix) const
{
    return m_importSuffix2PluginObj.value(suffix,nullptr);
}
///
/// \brief 卸载插件
/// \param plugin
/// \return
///
//bool SAPluginManager::unloadPlugin(SAAbstractPlugin *plugin)
//{

//}
///
/// \brief 加载正常插件- 目录位于plguin文件夹下
/// \return 加载的数量
///
int SAPluginManager::loadNormalPlugin()
{
    QDir pluginDir(qApp->applicationDirPath());
    //插件所在文件夹
    if(!pluginDir.cd("plugin"))
    {
        return 0;
    }
    QString suffix;
    int sufLen;
    int loadCount = 0;
#if defined(Q_OS_WIN)
    suffix = ".dll";
    sufLen = 4;
#elif defined(Q_OS_UNIX)
    suffix = ".so";
    sufLen = 3;
#endif
    foreach (const QString& file, pluginDir.entryList(QDir::Files))
    {
        if(suffix == file.right(sufLen))
        {
            if(loadOneNormalPlugin(pluginDir.absoluteFilePath(file)))
            {
                ++loadCount;
            }
        }
    }
    return loadCount;
}


///
/// \brief 加载一个插件
/// \param 插件的完整路径
/// \return 加载错误返回false
/// \note 错误信息会通过errorMessage发出
///
bool SAPluginManager::loadOneNormalPlugin(const QString &fullPath)
{
    QString baseName = QFileInfo(fullPath).baseName();
    QString errUnexp = tr("unexpect library!name:%1").arg(fullPath);
    if(isNormalPluginLoaded(baseName))
    {
        emit postInfoMessage( tr( "plugin %1 have been loaded!" )
                .arg( baseName),SA::WarningMessage);
        return false;
    }
    QLibrary lib(fullPath);
    bool isLoaded = lib.load();
    if ( !isLoaded )
    {
        emit postInfoMessage( tr( "Failed to load %1 (Reason: %2)" )
                .arg( lib.fileName(), lib.errorString() ),SA::WarningMessage);
        return false;
    }
    SA_GetPluginTypeFunPtr typeFp = (SA_GetPluginTypeFunPtr)lib.resolve("pluginType");
    if(nullptr == typeFp)
    {
        emit postInfoMessage(errUnexp,SA::ErrorMessage);
        return false;
    }
    switch(typeFp())
    {
    case SAAbstractPlugin::DataImport:
    {
        loadDataImportPlugin(lib,baseName,fullPath);
        break;
    }
    case SAAbstractPlugin::Function:
    {
        loadFunctionPlugin(lib,baseName,fullPath);
        break;
    }
    default:
        break;
    }
    return true;
}

bool SAPluginManager::loadDataImportPlugin(QLibrary& lib, const QString &baseName, const QString &fullPath)
{
    QString errUnexp = tr("unexpect library!name:%1").arg(fullPath);
    SA_CreatePluginFunPtr creFp = (SA_CreatePluginFunPtr)lib.resolve("createPlugin");
    if(nullptr == creFp)
    {
        emit postInfoMessage(errUnexp,SA::ErrorMessage);
        return false;
    }
    PluginInfo<SAAbstractPlugin*> pi;
    SAAbstractPlugin* plugin = creFp();
    if(nullptr == plugin)
    {
        emit postInfoMessage(errUnexp,SA::ErrorMessage);
        return false;
    }
    pi.plugin = plugin;
    pi.pluginFileName = baseName;
    pi.pluginPath = fullPath;
    pi.type = plugin->getType();
    SAAbstractDataImportPlugin* dataImportPlugin = static_cast<SAAbstractDataImportPlugin*>(plugin);
    if(nullptr == dataImportPlugin)
    {
        deleteTempPlugin(lib,plugin);
        return false;
    }

    m_normalPluginList.append(pi);
    dataImportPlugin->setupUI(m_ui);
    if(dataImportPlugin->isUseSAOpenFile())
    {
        //说明是可以通过sa的open打开的
        QStringList nameFilter,suffix;
        dataImportPlugin->getSAOpenFileInfo(nameFilter,suffix);
        m_openFileNameFilters.append(nameFilter);
        for(int i=0;i<suffix.size();++i)
        {
            m_importSuffix2PluginObj[suffix[i]] = dataImportPlugin;
        }
    }
    return true;
}

bool SAPluginManager::loadFunctionPlugin(QLibrary &lib, const QString &baseName, const QString &fullPath)
{
    QString errUnexp = tr("unexpect library!name:%1").arg(fullPath);
    SA_CreatePluginFunPtr creFp = (SA_CreatePluginFunPtr)lib.resolve("createPlugin");
    if(nullptr == creFp)
    {
        emit postInfoMessage(errUnexp,SA::ErrorMessage);
        return false;
    }
    PluginInfo<SAAbstractPlugin*> pi;
    SAAbstractPlugin* plugin = creFp();
    if(nullptr == plugin)
    {
        emit postInfoMessage(errUnexp,SA::ErrorMessage);
        return false;
    }
    pi.plugin = plugin;
    pi.pluginFileName = baseName;
    pi.pluginPath = fullPath;
    pi.type = plugin->getType();
    SAAbstractFunctionPackagePlugin* funPlugin = static_cast<SAAbstractFunctionPackagePlugin*>(plugin);
    if(nullptr == funPlugin)
    {
        deleteTempPlugin(lib,plugin);
        return false;
    }

    m_normalPluginList.append(pi);
    funPlugin->setupUI(m_ui);

    return true;
}

void SAPluginManager::deleteTempPlugin(QLibrary &lib, SAAbstractPlugin *plugin)
{
    SA_DeletePluginFunPtr delFp = (SA_DeletePluginFunPtr)lib.resolve("deletePlugin");
    if(nullptr == delFp)
    {
        QString str = tr("plugin:%1 was unexpect plugin,please remove").arg(lib.fileName());
        emit postInfoMessage(str,SA::ErrorMessage);
    }
    delFp(plugin);
}

