#include "SAGUIConfig.h"
#include <QDir>
#include <QApplication>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "SALog.h"
#include "SAGlobalConfig.h"
#include "SAGlobalConfigDefine.h"
SAGUIConfig::SAGUIConfig()
{

}

SAGUIConfig::~SAGUIConfig()
{

}

///
/// \brief 默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
/// \return
///
SAGUIConfig::PropertyBrowserType SAGUIConfig::getDefaultPropertySetDialogType()
{
    int def = (int)GroupBoxType;
    return (SAGUIConfig::PropertyBrowserType)(saConfig->getIntConfig(CFG_CONTENT_GUI,CFG_GUI_DefaultPropertySetDialogType,def));
}
///
/// \brief 设置默认属性对话框的样式，用于记录SAPropertySetDialog的生成默认样式
/// \param type
/// \see SAConfig::PropertyBrowserType
///
void SAGUIConfig::setDefaultPropertySetDialogType(SAGUIConfig::PropertyBrowserType type)
{
    saConfig->setIntConfig(CFG_CONTENT_GUI,CFG_GUI_DefaultPropertySetDialogType,(int)type);
}
///
/// \brief 所有参数设置为默认
///
void SAGUIConfig::setDefault()
{
    setDefaultPropertySetDialogType(GroupBoxType);
}


