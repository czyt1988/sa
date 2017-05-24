#ifndef SACONFIG_H
#define SACONFIG_H
#include "SALibGlobal.h"
#include <QObject>
class SAConfigPrivate;
///
/// \brief sa的总体设置内容
/// \note 此类是单例
/// \author czy
/// \date 2017-05-23
///
class SALIB_EXPORT SAConfig : public QObject
{
    Q_OBJECT
private:
    explicit SAConfig();
    Q_DISABLE_COPY(SAConfig)
public:
    enum PropertyBrowserType{
        TreeType
        ,GroupBoxType
        ,ButtonType
    };
    ~SAConfig();
    //获取对象
    static SAConfig* getInstance();
    //加载保存的配置文件
    void load();
    //获取配置文件的路径
    static QString getConfigFolderPath();
    //默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
    PropertyBrowserType getDefaultPropertySetDialogType() const;
private:
    static SAConfig* s_instance;
private:

};
#ifndef saConfig
#define saConfig SAConfig::getInstance()
#endif
#endif // SACONFIG_H
