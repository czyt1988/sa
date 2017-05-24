#ifndef SATEXTDATAIMPORT_H
#define SATEXTDATAIMPORT_H
#include "SAPluginInterface.h"
#include <QObject>
#include <QtPlugin>
#include <QIcon>
#include "SAUIInterface.h"
class SAAbstractDatas;
class QStringList;
class QAction;
///
/// \brief 数据导入的接口
///

class SATextDataImport : public QObject
                        ,public SAAbstractDataImportPlugin
{
    Q_OBJECT
#ifdef SA_QT_PLUGIN_MAKE
    Q_PLUGIN_METADATA(IID SAAbstractDataImportPlugin_iid FILE "textImport.json")
    Q_INTERFACES(SAAbstractDataImportPlugin)
#endif
public:
    SATextDataImport();
    virtual ~SATextDataImport();
    //插件类型
    virtual PluginType getType() const;
    //插件的名称
    virtual QString getPluginName() const;
    //返回插件版本
    virtual QString getVersion() const;
    //获取插件的描述
    virtual QString getDescription() const;
    //获取菜单图标
    virtual void setupUI(SAUIInterface* ui);
    //isUseSAOpenFile返回false调用的打开文件接口函数
    virtual bool openFile();
    //打开文件接口函数
    bool openFile(const QStringList &filePaths);
    //是否打开了文件
    virtual bool isOpenFile();
    //是通过SA系统自带的OpenFile还是使用自身openFile
    virtual bool isUseSAOpenFile() const;
    //如果需要通过SA系统自带的OpenFile打开，需要提供支持的后缀信息给sa
    virtual void getSAOpenFileInfo(QStringList& nameFileter,QStringList& suffixKey) const;
    //创建导出的数据
    virtual QList<SAAbstractDatas*> getResultPtr();
private slots:
    // action 的响应
    void on_actionTextImport();
    // csv import
    void on_actionCsvImport();
    //CFX csv Import
    void on_cfxCsvImport();
private:
    bool loadFile(const QStringList &filePaths,int type);
private:
    QString m_filePath;///< 保存文件路径
    QList<SAAbstractDatas *> m_resPtr;///< 保存生成的数据指针
    bool m_isOpen;///< 记录是否成功打开
    //
    QAction * m_actionTextImport;///< 构建ui用的action
    QAction * m_actionCsvImport;///< 构建ui用的action
    QAction * m_actionCFXCsvImport;///< 构建ui用的action
    //
    SAUIInterface* m_ui;
};

#endif // SAABSTRACTDATAIMPORT

