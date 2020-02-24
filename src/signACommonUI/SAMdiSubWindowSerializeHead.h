#ifndef SAMDISUBWINDOWSERIALIZEHEAD_H
#define SAMDISUBWINDOWSERIALIZEHEAD_H
#include "SACommonUIGlobal.h"
#include <QString>
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
            int type;///< 类型
            char typeName[64];///< 类名
        } param;
        char raw[SA_SUB_WINDOW_HEADER_LENGTH];
    };
    void init();
    bool isValid() const;
};
SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAMdiSubWindowSerializeHead& p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAMdiSubWindowSerializeHead& p);
//把类名写入SAMdiSubWindowSerializeHead
SA_COMMON_UI_EXPORT void setTypeNameToSerializeHead(const QString& typeName,SAMdiSubWindowSerializeHead& p);
SA_COMMON_UI_EXPORT QString getTypeNameFromSerializeHead(const SAMdiSubWindowSerializeHead& p);

#endif // SAMDISUBWINDOWSERIALIZEHEAD_H
