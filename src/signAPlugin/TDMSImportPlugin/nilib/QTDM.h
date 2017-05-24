#ifndef QTDM_H
#define QTDM_H

#include "qtdm_global.h"
#include <QString>

class QTDMSHARED_EXPORT QTDM
{
public:
    QTDM();
    ~QTDM();
    QString version() const;
};

#endif // QTDM_H
