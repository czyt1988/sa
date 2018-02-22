#ifndef SA[NAME]_H
#define SA[NAME]_H
#include "SAPluginInterface.h"
#include <QObject>
#include <QtPlugin>
#include <QIcon>
#include "SAUIInterface.h"
class SAAbstractDatas;
class QStringList;
class QAction;
///
/// \brief 
///
class [Name] : public QObject
                        ,public SAAbstractDataImportPlugin
{
    Q_OBJECT

public:
    [Name]();
    virtual ~[Name]();
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
    //是否打开了文件
    virtual bool isOpenFile();
    //是通过SA系统自带的OpenFile还是使用自身openFile
    virtual bool isUseSAOpenFile() const;
    //如果需要通过SA系统自带的OpenFile打开，需要提供支持的后缀信息给sa
    virtual void getSAOpenFileInfo(QStringList& nameFileter,QStringList& suffixKey) const;
    //创建导出的数据
    virtual QList<SAAbstractDatas*> getResultPtr();
private:
    SAUIInterface* m_ui;
};

#endif // SAABSTRACTDATAIMPORT

