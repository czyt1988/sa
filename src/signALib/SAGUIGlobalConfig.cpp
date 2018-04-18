#include "SAGUIGlobalConfig.h"
#include <QDir>
#include <QApplication>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "SALog.h"
#include "SAGlobalConfig.h"
#include "SAGlobalConfigDefine.h"
SAGUIGlobalConfig::SAGUIGlobalConfig()
{

}

SAGUIGlobalConfig::~SAGUIGlobalConfig()
{

}

///
/// \brief 默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
/// \return
///
SAGUIGlobalConfig::PropertyBrowserType SAGUIGlobalConfig::getDefaultPropertySetDialogType()
{
    int def = (int)GroupBoxType;
    return (SAGUIGlobalConfig::PropertyBrowserType)(saConfig.getIntValue(CFG_CONTENT_GUI,CFG_GUI_DefaultPropertySetDialogType,def));
}
///
/// \brief 设置默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
/// \param type
/// \see SAConfig::PropertyBrowserType
///
void SAGUIGlobalConfig::setDefaultPropertySetDialogType(SAGUIGlobalConfig::PropertyBrowserType type)
{
    saConfig.setIntValue(CFG_CONTENT_GUI,CFG_GUI_DefaultPropertySetDialogType,(int)type);
}
///
/// \brief 所有参数设置为默认
///
void SAGUIGlobalConfig::setDefault()
{
    setDefaultPropertySetDialogType(GroupBoxType);
}


