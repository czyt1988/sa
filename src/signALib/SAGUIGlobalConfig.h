#ifndef SAGUICONFIG_H
#define SAGUICONFIG_H
#include "SALibGlobal.h"
#include <QObject>
#include <memory>
class SAConfigPrivate;
///
/// \brief sa的总体设置内容
/// \note 此类是单例
/// \author czy
/// \date 2017-05-23
///
class SALIB_EXPORT SAGUIGlobalConfig
{
private:
    Q_DISABLE_COPY(SAGUIGlobalConfig)
public:
    explicit SAGUIGlobalConfig();
    enum PropertyBrowserType{
        TreeType
        ,GroupBoxType
        ,ButtonType
    };
    ~SAGUIGlobalConfig();
public:
    static void setDefault();
    //默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
    static PropertyBrowserType getDefaultPropertySetDialogType();
    static void setDefaultPropertySetDialogType(PropertyBrowserType type);
    //
};

#endif // SACONFIG_H
