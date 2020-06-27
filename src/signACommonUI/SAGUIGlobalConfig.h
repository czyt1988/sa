#ifndef SAGUICONFIG_H
#define SAGUICONFIG_H
#include "SACommonUIGlobal.h"
#include <QObject>
#include <memory>
class SAConfigPrivate;
///
/// \brief sa的总体设置内容
/// \note 此类是单例
/// \author czy
/// \date 2017-05-23
///
class SA_COMMON_UI_EXPORT SAGUIGlobalConfig
{
private:
    Q_DISABLE_COPY(SAGUIGlobalConfig)
public:
    explicit SAGUIGlobalConfig();
    ///
    /// \brief 属性表的样式
    ///
    enum PropertyBrowserType{
        TreeType ///< 树形样式
        ,GroupBoxType ///< group 样式
        ,ButtonType ///< 按钮样式 （默认）
    };
    ~SAGUIGlobalConfig();
public:
    //还原为默认设置
    static void setDefault();
    //默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
    static PropertyBrowserType getDefaultPropertySetDialogType();
    static void setDefaultPropertySetDialogType(PropertyBrowserType type);
    //
};

#endif // SACONFIG_H
