#ifndef SAMDISUBWINDOWSERIALIZEHEAD_H
#define SAMDISUBWINDOWSERIALIZEHEAD_H
#include "SACommonUIGlobal.h"
#ifndef SA_SUB_WINDOW_HEADER_LENGTH
#define SA_SUB_WINDOW_HEADER_LENGTH (256)
#endif
///
/// \brief 用于窗口文件保存用的文件头
///
struct SA_COMMON_UI_EXPORT SAMdiSubWindowSerializeHead
{

    union
    {
        struct
        {
            unsigned int magic1;///< 判断文件的魔数
            int type;
        } param;
        char raw[SA_SUB_WINDOW_HEADER_LENGTH];
    };
    bool isValid() const;
    void setValid();
};
SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAMdiSubWindowSerializeHead& p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAMdiSubWindowSerializeHead& p);
#endif // SAMDISUBWINDOWSERIALIZEHEAD_H
