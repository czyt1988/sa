#ifndef SATCPSECTION_PRIVATE_H
#define SATCPSECTION_PRIVATE_H
#include <QTcpSocket>
#include <QSet>
#include <QMutex>
/**
 * @brief 这是一个私有类
 */
class SATCPSection : public QObject
{
    Q_OBJECT
public:
    SATCPSection(QObject* par);
    ~SATCPSection();
public slots:
    void appendSocket(QTcpSocket* socket);
signals:
private slots:

private:
    QSet<QTcpSocket*> m_socketSets;
    QMutex m_lockSocketSets;
};

#endif // SATCPSECTION_PRIVATE_H
