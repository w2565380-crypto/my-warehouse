#include "checkclient.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

CheckClient::CheckClient(QObject *parent)
    : QObject{parent}
{
    m_clientSocket=new QTcpSocket(this);
    connect(m_clientSocket,&QTcpSocket::connected,this,&CheckClient::connected);
    connect(m_clientSocket,&QTcpSocket::readyRead,this,&CheckClient::onReadyRead);
}

void CheckClient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);
    for(;;){
        socketStream.startTransaction();
        socketStream>>jsonData;
        if(socketStream.commitTransaction()){
            //emit messageReceived(QString::fromUtf8(jsonData));
            QJsonParseError parseError;
            const QJsonDocument jsonDoc=QJsonDocument::fromJson(jsonData,&parseError);
            if(parseError.error==QJsonParseError::NoError){
                if(jsonDoc.isObject()){
                    //emit logMessage(QJsonDocument(jsonDoc).toJson(QJsonDocument::Compact));
                    emit jsonReceived(jsonDoc.object());
                }
            }
        }else{
            break;
        }
    }
}

void CheckClient::sendMessage(const QString &text, const QString &type)
{
    if(m_clientSocket->state()!=QAbstractSocket::ConnectedState){
        return;
    }
    if(!text.isEmpty()){
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_5_12);

        QJsonObject message;
        message["type"]=type;
        message["text"]=text;

        serverStream<< QJsonDocument(message).toJson();
    }
}

void CheckClient::connectToServer(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(address,port);
}

void CheckClient::disconnectFromServer()
{
    m_clientSocket->disconnectFromHost();
}
