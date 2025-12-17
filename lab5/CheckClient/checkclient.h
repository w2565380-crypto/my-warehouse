#ifndef CHECKCLIENT_H
#define CHECKCLIENT_H

#include <QObject>
#include <QtcpSocket>

class CheckClient : public QObject
{
    Q_OBJECT
public:
    explicit CheckClient(QObject *parent = nullptr);

signals:
    void connected();
    void messageReceived(const QString &text);

private:
    QTcpSocket *m_clientSocket;

public slots:
    void onReadyRead();
    void sendMessage(const QString &text,const QString &type="message");
    void connectToServer(const QHostAddress &address,quint16 port);
};

#endif // CHECKCLIENT_H
