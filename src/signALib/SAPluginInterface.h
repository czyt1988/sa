#ifndef SAPLUGININTERFACE_H
#define SAPLUGININTERFACE_H
///
/// \file 此文件定义了sa的所有对外接口，接口的编写都是从此文件下的各个接口类继承
///
#include <QtPlugin>
QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QIcon;
QT_END_NAMESPACE
#include <QList>
class SAAbstractDatas;
class SAUIInterface;
class SAAbstractPlugin
{
public:
    SAAbstractPlugin(){}
    virtual ~SAAbstractPlugin(){}

    enum PluginType
    {
        UnKnow = 0///<
        ,UI = 1 ///< 界面相关
        ,DataImport///< 数据导入插件
        ,Function ///< 函数包
    };

    virtual PluginType getType() const = 0;

    ///
    /// \brief 插件名
    /// \return
    ///
    virtual QString getPluginName() const = 0;

    ///
    /// \brief 插件名getter & setter
    /// \return
    ///
    virtual QString getVersion() const = 0;


    ///
    /// \brief 插件描述
    /// \return
    ///
    virtual QString getDescription() const = 0;

//    ///
//    /// \brief 卸载插件
//    /// 此函数仅仅用于手动提前卸载插件时调用，并不会delete插件的object
//    /// 此函数主要用于对ui等关联的解除，以及在插件中new处理的内容的delete
//    ///
//    virtual void unload() = 0;
};
///
/// \brief 数据导入的接口文件插件
/// 所有导入数据插件，都是先调用openFile() 然后调用createResultPtr()把数据生成
///
class SAAbstractUIPlugin : public SAAbstractPlugin
{
public:
    SAAbstractUIPlugin():SAAbstractPlugin(){}
    virtual ~SAAbstractUIPlugin(){}
    ///
    /// \brief 构建ui，插件加载之后，第一件事情就会调用此函数
    ///
    virtual void setupUI(SAUIInterface* ui) = 0;
};



///
/// \brief 数据导入的接口文件插件
/// 所有导入数据插件，都是先调用openFile() 然后调用createResultPtr()把数据生成
///
class SAAbstractDataImportPlugin : public SAAbstractUIPlugin
{
public:
    SAAbstractDataImportPlugin():SAAbstractUIPlugin(){}
    virtual ~SAAbstractDataImportPlugin(){}

    ///
    /// \brief 是通过SA系统自带的OpenFile还是使用自身openFile
    ///
    /// 如果返回true。这时sa将会调用bool openFile(const QStringList* filePaths),并传入用户选择的路径，
    /// 如果需要多选，需要isMultiselect返回true
    /// 如果返回false，sa将会调用bool openFile()
    /// \return
    ///
    virtual bool isUseSAOpenFile() const = 0;
    ///
    /// \brief isUseSAOpenFile为true时，需要重写getSAOpenFileInfo
    /// \param nameFileter 文件名过滤
    /// \param suffixKey 文件后缀索引，如果和已有的后缀重复，将会失效
    ///
    virtual void getSAOpenFileInfo(QStringList& nameFileter,QStringList& suffixKey) const = 0;
    ///
    /// \brief 打开文件接口函数
    /// \return 成功返回true,将调用createResultPtr
    ///
    virtual bool openFile() = 0;
    virtual bool openFile(const QStringList &filePaths) = 0;
    ///
    /// \brief 是否打开了文件
    /// \return
    ///
    virtual bool isOpenFile() = 0;
    ///
    /// \brief 创建导出的数据
    /// \return
    ///
    virtual QList<SAAbstractDatas*> getResultPtr() = 0;
};

/// \brief 函数功能包插件
///
class SAAbstractFunctionPackagePlugin : public SAAbstractUIPlugin
{
public:
    virtual ~SAAbstractFunctionPackagePlugin(){}
    //获取包名
    virtual QString getPackageName() const = 0;
    //获取包里的函数名
    virtual QStringList getFunctionNames() const = 0;
    //获取函数的描述
    virtual QString getFunctionDescribe(const QString& funName) const = 0;
};


typedef SAAbstractPlugin::PluginType (*SA_GetPluginTypeFunPtr)();
typedef QString (*SA_GetPluginNameFunPtr)();
typedef QString (*SA_GetPluginVersionFunPtr)();
typedef QString (*SA_GetPluginDescriptionFunPtr)();
typedef SAAbstractPlugin* (*SA_CreatePluginFunPtr)();
typedef void (*SA_DeletePluginFunPtr)(SAAbstractPlugin*);

#ifndef SA_PLUGIN_EXPORT
#define SA_PLUGIN_EXPORT Q_DECL_EXPORT
#endif

#ifdef SA_QT_PLUGIN_MAKE
    QT_BEGIN_NAMESPACE

    #define SAAbstractPlugin_iid "sa.plugin.interface.SAAbstractPlugin"
    Q_DECLARE_INTERFACE(SAAbstractPlugin,SAAbstractPlugin_iid)//这个宏用声明接口

    #define SAAbstractUIPlugin_iid "sa.plugin.interface.SAAbstractUIPlugin"
    Q_DECLARE_INTERFACE(SAAbstractUIPlugin,SAAbstractUIPlugin_iid)//这个宏用声明接口

    #define SAAbstractDataImportPlugin_iid "sa.plugin.interface.SAAbstractDataImportPlugin"
    Q_DECLARE_INTERFACE(SAAbstractDataImportPlugin,SAAbstractDataImportPlugin_iid)//这个宏用声明接口

    #define SAAbstractFunctionPackagePlugin_iid "sa.plugin.interface.SAAbstractFunctionPackagePlugin"
    Q_DECLARE_INTERFACE(SAAbstractFunctionPackagePlugin,SAAbstractFunctionPackagePlugin_iid)//这个宏用声明接口

    QT_END_NAMESPACE
#endif
#endif // SAABSTRACTPLUGIN

