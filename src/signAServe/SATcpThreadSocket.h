#ifndef SATCPTHREADSOCKET_H
#define SATCPTHREADSOCKET_H
#include <QThread>

/**
 * @brief 多线程socket
 */
class SATcpThreadSocket : public QThread
{
public:
    SATcpThreadSocket(QObject* par = nullptr);

};

#endif // SATCPTHREADSOCKET_H
