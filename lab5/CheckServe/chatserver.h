#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>

class ChatServer : public QTcpServer
{
public:
    ChatServer();
};

#endif // CHATSERVER_H
