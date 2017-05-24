#include "QTDM.h"
#pragma comment(lib, "user32.lib")

QTDM::QTDM()
{
}

QTDM::~QTDM()
{

}

QString QTDM::version() const
{
    return QString("1.0");
}
