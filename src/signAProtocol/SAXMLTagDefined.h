#ifndef SAXMLTAGDEFINED_H
#define SAXMLTAGDEFINED_H

/*
 * <sa>
 *  <values>
 *      <group name="">
 *          <item type='int'>5<item>
 *      </group>
 *  </values>
 * </sa>
 */

/// \def 定义特殊处理的数组类型 QVariantList
#ifndef SA_XML_VAR_ARR_LIST
#define SA_XML_VAR_ARR_LIST    "QVariantList"
#endif
/// \def 定义特殊处理的数组类型 QStringList
#ifndef SA_XML_VAR_ARR_STRLIST
#define SA_XML_VAR_ARR_STRLIST    "QStringList"
#endif
/// \def 定义特殊处理的数组类型 QVariantMap
#ifndef SA_XML_VAR_ARR_MAP
#define SA_XML_VAR_ARR_MAP    "QVariantMap"
#endif
/// \def 定义特殊处理的数组类型 QVariantHash
#ifndef SA_XML_VAR_ARR_HASH
#define SA_XML_VAR_ARR_HASH    "QVariantHash"
#endif
/// \def 定义特殊处理的数组类型 QVector<QPointF>
#ifndef SA_XML_VAR_ARR_VECTOR_POINTF
#define SA_XML_VAR_ARR_VECTOR_POINTF    "QVector<QPointF>"
#endif
/// \def sa打头的标签，表示这是一个sa协议
#ifndef SA_XML_TAG_SA
#define SA_XML_TAG_SA    "sa"
#endif
/// \def value标签，value下是group，group下是item
#ifndef SA_XML_TAG_VALUES
#define SA_XML_TAG_VALUES    "values"
#endif
/// \def prop标签，用于描述属性，下是item
#ifndef SA_XML_TAG_PROPERTY
#define SA_XML_TAG_PROPERTY    "prop"
#endif
/// \def default-group标签 用于标记一默认分组
#ifndef SA_XML_TAG_DEFAULTGROUP
#define SA_XML_TAG_DEFAULTGROUP    "default-group"
#endif
/// \def group标签 用于标记一组项目
#ifndef SA_XML_TAG_GROUP
#define SA_XML_TAG_GROUP    "group"
#endif
/// \def item标签 用于标记一个项目
#ifndef SA_XML_TAG_ITEM
#define SA_XML_TAG_ITEM    "item"
#endif
/// \def li标签 用于标记一个数组内容
#ifndef SA_XML_TAG_LI
#define SA_XML_TAG_LI    "li"
#endif
/// \def int val int型数据
#ifndef SA_XML_TAG_INTVAL
#define SA_XML_TAG_INTVAL    "intVal"
#endif
/// \def uint val uint型数据
#ifndef SA_XML_TAG_UINTVAL
#define SA_XML_TAG_UINTVAL    "uintVal"
#endif
/// \def quintptr型数据
#ifndef SA_XML_TAG_QUINTPTR
#define SA_XML_TAG_QUINTPTR    "quintptrVal"
#endif

/// \def <sa>的属性类号<sa classid="xxx">
#ifndef SA_XML_ATT_NAME_CLASSID
#define SA_XML_ATT_NAME_CLASSID    "classid"
#endif

/// \def <sa>的属性功能号<sa funid="xxx">
#ifndef SA_XML_ATT_NAME_FUNID
#define SA_XML_ATT_NAME_FUNID    "funid"
#endif

/// \def <sa>的属性类型名<sa type="xxx">
#ifndef SA_XML_ATT_NAME_SA_TYPE
#define SA_XML_ATT_NAME_SA_TYPE    "type"
#endif
///
/// \def <sa>的属性类型名<sa type="xxx">中的xxx定义
/// 处理VectorPointF的返回
///
#ifndef ATT_SA_TYPE_VPFR
#define ATT_SA_TYPE_VPFR    "vpfr"
#endif
/// \def dfi标签 用于标记一次VectorPointF的处理结果
#ifndef SA_XML_TAG_DFI_ROOT
#define SA_XML_TAG_DFI_ROOT    "dfi"
#endif


/// \def type属性标签 用于描述属性"type"
#ifndef SA_XML_ATT_TYPE
#define SA_XML_ATT_TYPE    "type"
#endif
/// \def type属性标签对应的值：string
#ifndef ATT_TYPE_STRING
#define ATT_TYPE_STRING    "string"
#endif
/// \def type属性标签对应的值：int
#ifndef ATT_TYPE_INT
#define ATT_TYPE_INT    "int"
#endif
/// \def type属性标签对应的值：variant
#ifndef ATT_TYPE_VARIANT
#define ATT_TYPE_VARIANT    "variant"
#endif
/// \def type属性标签对应的值：vectorpointf
#ifndef ATT_TYPE_VECTORPOINTF
#define ATT_TYPE_VECTORPOINTF    "vectorpointf-value"
#endif
/// \def type属性标签对应的值：vectorpointf
#ifndef ATT_TYPE_VECTORPOINT
#define ATT_TYPE_VECTORPOINT    "vectorpoint-value"
#endif
/// \def name属性标签 用于描述属性"name"
#ifndef SA_XML_ATT_NAME
#define SA_XML_ATT_NAME    "name"
#endif
/// \def id属性标签 用于描述属性"id"
#ifndef SA_XML_ATT_ID
#define SA_XML_ATT_ID    "id"
#endif
/// \def varType属性标签 用于描述属性"varType"
#ifndef SA_XML_ATT_VALUE_TYPE
#define SA_XML_ATT_VALUE_TYPE    "varType"
#endif

/// \def icon属性标签 用于描述属性"icon",内容是一个base64
#ifndef SA_XML_ATT_ICON
#define SA_XML_ATT_ICON    "icon"
#endif

/// \def varType属性标签值vectorpointf,标识点序列的数值
#ifndef SA_XML_ATT_VALUE_TYPE_VECTORPOINTF
#define SA_XML_ATT_VALUE_TYPE_VECTORPOINTF    "vectorpointf"
#endif

/// \def val属性标签 用于描述属性"val"
#ifndef SA_XML_ATT_VALUE
#define SA_XML_ATT_VALUE    "val"
#endif



#endif // SAXMLTAGDEFINED_H
