#ifndef SADATAPROCCLIENT_H
#define SADATAPROCCLIENT_H
#include <QObject>
#include <QLocalSocket>
class SADataProcClient : public QObject
{
    Q_OBJECT
public:
    SADataProcClient(QObject *parent = nullptr);
    void shakeHand();
    uint getPid() const;
    void setPid(const uint &pid);

private:
    Q_SLOT void errorOccurred(QLocalSocket::LocalSocketError err);
    Q_SLOT void onReadyRead();
private:
    QLocalSocket m_socket;
    uint m_pid;
};

#endif // SADATAPROCCLIENT_H
