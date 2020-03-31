#ifndef SADATAPROCFUNCTIONS_H
#define SADATAPROCFUNCTIONS_H
#include <QFuture>
#include "SASession.h"
#include "SAProtocolHeader.h"
/**
  * @brief 所有异步调用函数
  *
 */


//处理二维点描述
SASession::XMLDataPtr reply2DPointsDescribe(const SAProtocolHeader &header, SASession::XMLDataPtr xml);

#endif // SADATAPROCFUNCTIONS_H
