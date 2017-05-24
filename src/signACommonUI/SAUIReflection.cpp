#include "SAUIReflection.h"

#define CHECK_PTR(ptr,ret)\
do{\
    if(nullptr == ptr)\
    {\
        return ret;\
    }\
}while(0)

#define CHECK_PTR_RET_NULL(ptr)\
do{\
    if(nullptr == ptr)\
    {\
        return nullptr;\
    }\
}while(0)

#define CHECK_PTR_RET(ptr)\
do{\
    if(nullptr == ptr)\
    {\
        return;\
    }\
}while(0)

//SAUIReflection* SAUIReflection::s_instancePtr = nullptr;

SAUIReflection::SAUIReflection()
{

}

SAUIReflection::~SAUIReflection()
{
}

SAUIInterface *SAUIReflection::uiInterface()
{
    return m_uiInterface;
}

void SAUIReflection::setupUIInterface(SAUIInterface *ui)
{
    m_uiInterface = ui;
}


SAUIReflection *SAUIReflection::getInstance()
{
    //C++11 static 都是原子操作
    static SAUIReflection s_instance;
    return &s_instance;
}


