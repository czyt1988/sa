#ifndef SATREE_H
#define SATREE_H
#include "SALibGlobal.h"
#include <QVariant>
#include "SAItem.h"
class SATreePrivate;
class SATree;


/**
 * @brief 通用树形结构数据存储
 * 支持任意拷贝和赋值
 */
class SALIB_EXPORT SATree
{
    SA_IMPL(SATree)
    friend class SAItem;
public:
    SATree();
    ~SATree();
};

#endif // SATREE_H
