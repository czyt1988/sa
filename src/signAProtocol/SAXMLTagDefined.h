#ifndef SAXMLTAGDEFINED_H
#define SAXMLTAGDEFINED_H
/*
<sa>
    <header></header>
    <header></header>
</sa>
*/
/// \def sa打头的标签，表示这是一个sa协议
#ifndef SA_XML_TAG_SA
#define SA_XML_TAG_SA "sa"
#endif
/// \def int val int型数据
#ifndef SA_XML_TAG_INTVAL
#define SA_XML_TAG_INTVAL "intVal"
#endif
/// \def uint val uint型数据
#ifndef SA_XML_TAG_UINTVAL
#define SA_XML_TAG_UINTVAL "uintVal"
#endif
/// \def quintptr型数据
#ifndef SA_XML_TAG_QUINTPTR
#define SA_XML_TAG_QUINTPTR "quintptrVal"
#endif

/// \def <sa>的属性类型名<sa type="xxx">
#ifndef SA_XML_ATT_NAME_SA_TYPE
#define SA_XML_ATT_NAME_SA_TYPE "type"
#endif
///
/// \def <sa>的属性类型名<sa type="xxx">中的xxx定义
/// 处理VectorPointF的返回
///
#ifndef ATT_SA_TYPE_VPFR
#define ATT_SA_TYPE_VPFR "vpfr"
#endif
/// \def header标签 用于标记sa的header
#ifndef SA_XML_TAG_HEADER_ROOT
#define SA_XML_TAG_HEADER_ROOT "header"
#endif
/// \def Content标签 用于标记sa的Content
#ifndef SA_XML_TAG_CONTENT_ROOT
#define SA_XML_TAG_CONTENT_ROOT "content"
#endif
/// \def dfi标签 用于标记一次VectorPointF的处理结果
#ifndef SA_XML_TAG_DFI_ROOT
#define SA_XML_TAG_DFI_ROOT "dfi"
#endif
/// \def group标签 用于标记一组项目
#ifndef SA_XML_TAG_GROUP
#define SA_XML_TAG_GROUP "group"
#endif
/// \def item标签 用于标记一个项目
#ifndef SA_XML_TAG_ITEM
#define SA_XML_TAG_ITEM "item"
#endif

/// \def type属性标签 用于描述属性"type"
#ifndef SA_XML_ATT_TYPE
#define SA_XML_ATT_TYPE "type"
#endif
/// \def type属性标签对应的值：value
#ifndef ATT_TYPE_VALUE
#define ATT_TYPE_VALUE "value"
#endif


/// \def name属性标签 用于描述属性"name"
#ifndef SA_XML_ATT_NAME
#define SA_XML_ATT_NAME "name"
#endif

/// \def varType属性标签 用于描述属性"varType"
#ifndef SA_XML_ATT_VALUE_TYPE
#define SA_XML_ATT_VALUE_TYPE "varType"
#endif

/// \def val属性标签 用于描述属性"val"
#ifndef SA_XML_ATT_VALUE
#define SA_XML_ATT_VALUE "val"
#endif




namespace SA_XML {
    enum ProtocolType
    {
        UnknowType
        ,TypeVectorPointFProcessResult ///< 线性数组处理结果返回
    };
}













#endif // SAXMLTAGDEFINED_H
