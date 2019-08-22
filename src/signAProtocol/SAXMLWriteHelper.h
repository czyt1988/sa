#ifndef SAXMLWRITEHELPER_H
#define SAXMLWRITEHELPER_H
#include "SAProtocolGlobal.h"
#include <QVariant>

class SAXMLWriteHelperPrivate;

/**
 * @brief SA XML协议的辅助写入类
 */
class SA_PROTOCOL_EXPORT SAXMLWriteHelper
{
    SA_IMPL(SAXMLWriteHelper)
public:
    SAXMLWriteHelper(const QString& type);
    ~SAXMLWriteHelper();
    //开始协议头编辑
    void startHeadWriteGroup(const QString& name);
    //结束协议头的编辑
    void endHeadWriteGroup();
    //写入协议头的数据必须和startHeadWriteGroup 配套
    void writeHeadValue(const QString& name, const QVariant &d);

    //开始内容区分组
    void startContentWriteGroup(const QString& name);
    //结束内容区分组
    void endContentWriteGroup();
    //在内容区写入数据
    void writeContentValue(const QString& name, const QVariant &d);
    //转换为文本
    QString toString(int indent = 1) const;
};

#endif // SAXMLWRITEHELPER_H
