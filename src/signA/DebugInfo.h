#ifndef DEBUGINFO_H
#define DEBUGINFO_H
//
/// 用于方便进行调试用的文件
///

#ifndef MY_DEBUG
#define MY_DEBUG
#endif

#include <QtDebug>

///< 空指针提示
#ifndef DEBUGINF_NullPtr
#define DEBUGINF_NullPtr {\
    qDebug()<<QObject::tr("--null ptr:")<<__FUNCTION__<<QObject::tr(";line:")<<__LINE__;\
    }
#endif

#endif // DEBUGINFO_H
