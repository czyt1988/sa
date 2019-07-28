#include "SATree.h"
#include "SAItem.h"
class SATreePrivate
{
    SA_IMPL_PUBLIC(SATree)
public:
    SATreePrivate(SATree* c)
        :q_ptr(c)
    {

    }
    QList<SAItem*> rootNodes;///< 记录所有根节点
};


SATree::SATree():d_ptr(new SATreePrivate(this))
{

}

SATree::~SATree()
{

}


