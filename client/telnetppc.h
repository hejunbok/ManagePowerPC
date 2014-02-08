#ifndef TELNETPPC_H
#define TELNETPPC_H

#include "qttelnet.h"
#include "Thread.h"

class TelnetPPC: public QObject
{
    Q_OBJECT
public:
    TelnetPPC(QObject *par = 0);


	void disconnectPPC();

	//Send message to PPC
	void sendMessage(const QString &msg);

public slots:
    
	void connectPPC(const QString &ip);

    //Receive message from PPC
    void receiveMessage(const QString &msg);



signals:
    void ipChanged(const bool fromUser , const QString &ipTail);
    void connected();
    void disconnected();
	void busy();
	void connectTimeout();
	void receivedMsg(const QString&);
    
    //If the PPC is configured wrong, it will trigger this signal
    void connectPPCError(QAbstractSocket::SocketError error);

private:
    QString stripCR(const QString &msg);
    
    
private:
    QtTelnet *qttelnet;
    Thread *thread;

};

#endif
