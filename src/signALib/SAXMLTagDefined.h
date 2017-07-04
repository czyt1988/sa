#ifndef SAXMLTAGDEFINED_H
#define SAXMLTAGDEFINED_H
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
#ifndef SA_XML_ATT_SA_TYPE_VPFR
#define SA_XML_ATT_SA_TYPE_VPFR "vpfr"
#endif
#endif // SAXMLTAGDEFINED_H
