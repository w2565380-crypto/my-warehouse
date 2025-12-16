#ifndef CHECKSERVER_H
#define CHECKSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"

class CheckServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CheckServer(QObject *parent=nullptr);

signals:
    void logMessage(const QString &msg);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    QVector<ServerWorker *>m_clients;

public slots:
    void stopServer();
};






#endif // CHECKSERVER_H
