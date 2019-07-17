#ifndef SALOCALSERVESOCKETCLINEPARSE_H
#define SALOCALSERVESOCKETCLINEPARSE_H
#include "SALocalServeSocketOpt.h"

class SALocalServeSocketClineParse : public SALocalServeSocketOpt
{
    Q_OBJECT
public:
    SALocalServeSocketClineParse(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeSocketClineParse(QObject* parent = nullptr);
    //设置token - 仅用于客户端
    void setToken(uint token);
};

#endif // SALOCALSERVESOCKETCLINEPARSE_H
