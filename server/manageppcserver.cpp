#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include "manageppcserver.h"

ManagePPCServer::ManagePPCServer(QObject *parent)
	: QTcpServer(parent)
{
	tcpsocket = new QTcpSocket;
	connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(tcpsocket, SIGNAL(disconnected()), tcpsocket, SLOT(deleteLater()));
	nextBlockSize = 0;	
}


void ManagePPCServer::incomingConnection(int socketDescriptor)
{
	tcpsocket->setSocketDescriptor(socketDescriptor);
}


void ManagePPCServer::readClient()
{
	QDataStream in(tcpsocket);
	in.setVersion(QDataStream::Qt_4_1);
	qDebug() << tcpsocket->bytesAvailable();
	if (nextBlockSize == 0) {
		if (tcpsocket->bytesAvailable() < sizeof(quint16))
			return;
		in >> nextBlockSize;
	}
	qDebug() << "nextBlockSize: " << nextBlockSize;
	if (tcpsocket->bytesAvailable() < nextBlockSize)
		return;

	QString requestType;
	QString ip;
	QString status;
	QString owner;
	in >> requestType >> ip >> status >> owner;
	if (requestType == QString("PPCStatus")) 
	{
		emit requestPPCStatus();
	}
	else
	{

	}

}

void ManagePPCServer::sendPPCStatus(QList<IPStatus> *IPStatusList)
{
	for(int i = 0; i < IPStatusList->size(); i++)
	{
		qDebug() << IPStatusList->at(i).IP << IPStatusList->at(i).status << IPStatusList->at(i).owner;
		ppcStatus(IPStatusList->at(i).IP, IPStatusList->at(i).status, IPStatusList->at(i).owner);
	}

	QDataStream out(tcpsocket);
	out << quint16(0xFFFF);
	//close();
	qDebug() << "isListening: " << isListening();
}

void ManagePPCServer::ppcStatus(const QString &ip, const QString &status, const QString &owner)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);

	out << quint16(0) << ip << status << owner;

	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	tcpsocket->write(block);
}

ManagePPCServer::~ManagePPCServer()
{

}

