#ifndef TELNETPPC_H
#define TELNETPPC_H

#include "qttelnet.h"
#include "thread.h"
#include "header.h"

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
    
    //If the PPC is configured wrong, it will trigger this signal
    void connectPPCError(QAbstractSocket::SocketError error);


signals:
    void ipChanged(const bool fromUser , const QString &ipTail);
	void telnetResult(const QString &tResult);

	void connected();
	void busy();
	void connectTimeout();
	void receivedMsg(const QString&);
    

private:
    QString stripCR(const QString &msg);
    
    
private:
    QtTelnet *qttelnet;
    Thread *thread;

};

#endif

