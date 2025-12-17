#include "checkserver.h"
#include "serverworker.h"

CheckServer::CheckServer(QObject *parent):
    QTcpServer(parent)
{

}

void CheckServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker=new ServerWorker(this);
    if(!worker->setSocketDescriptor(socketDescriptor)){
        worker->deleteLater();
        return;
    }
    connect(worker,&ServerWorker::logMessage,this,&CheckServer::logMessage);
    m_clients.append(worker);
    emit logMessage("新用户已连接");
}

void CheckServer::stopServer()
{
    close();
}
