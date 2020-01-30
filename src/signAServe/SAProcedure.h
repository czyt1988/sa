#ifndef SAPROCEDURE_H
#define SAPROCEDURE_H
#include "SAServeGlobal.h"
#include <QString>
/**
 * @brief 调用过程封装
 */
class SASERVE_EXPORT SAProcedure
{
public:
    SAProcedure(const QString& name,const QString& nameSpace = "");
    virtual ~SAProcedure();
};

#endif // SAPROCEDURE_H
