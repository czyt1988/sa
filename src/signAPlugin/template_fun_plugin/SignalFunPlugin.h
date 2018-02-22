#ifndef SATEXTDATAIMPORT_H
#define SATEXTDATAIMPORT_H
#include "SAPluginInterface.h"
#include <QObject>
#include <QtPlugin>
#include <QIcon>
class SAAbstractDatas;
class QStringList;
class QAction;
///
/// \brief 数据导入的接口
///

class SignalFunPlugin : public QObject
                        ,public SAAbstractDataImportPlugin
{
    Q_OBJECT
public:
    SignalFunPlugin();
    virtual ~SignalFunPlugin();
    //插件类型
    virtual PluginType getType() const;
    //插件的名称
    virtual QString getPluginName() const;
    //返回插件版本
    virtual QString getVersion() const;
    //获取插件的描述
    virtual QString getDescription() const;
    //获取菜单图标
    virtual void setupUI();
};

#endif // SAABSTRACTDATAIMPORT

