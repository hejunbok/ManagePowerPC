#ifndef MANAGEPPCSERVER_H
#define MANAGEPPCSERVER_H

#include <QTcpServer>
#include <QList>
#include "header.h"

class ManagePPCServer : public QTcpServer
{
	Q_OBJECT

public:
	ManagePPCServer(QObject *parent = 0);
	~ManagePPCServer();


public slots:
	void readClient();
	void sendPPCStatus(QList<IPStatus> *IPStatusList);

signals:
	void requestPPCStatus();
	

private:    
	void incomingConnection(int socketDescriptor);
	void ppcStatus(const QString &ip, const QString &status, const QString &owner);

	QTcpSocket *tcpsocket;
	quint16 nextBlockSize;
};

#endif // MANAGEPPCSERVER_H
