
#include <QMessageBox>
#include <QHostAddress>
#include "client.h"
#include "QLogger.h"

ManagePPCClient::ManagePPCClient(const QString cName, QObject *parent)
	:QObject(parent), clientName(cName), ppcIP(""), nextBlockSize(0)
{
    tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
	connect(tcpSocket, SIGNAL(disconnected()),this, SLOT(disconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMsg()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(error(QAbstractSocket::SocketError)));

	QHostAddress serverAddress("172.16.20.156");
	tcpSocket->connectToHost(serverAddress, 5555);
}

void ManagePPCClient::connected()
{
	sendMsg(NEW_CLIENT);
    emit connectedToServer();
}

void ManagePPCClient::disconnected()
{
	emit connectionClosedByServer();
	closeConnection();
}

void ManagePPCClient::readMsg()
{
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_1);
	QString replyType;
	QString replyContent;

	forever 
	{
		if (nextBlockSize == 0) 
		{
			if (tcpSocket->bytesAvailable() < sizeof(quint16))
				break;
			in >> nextBlockSize;
			qDebug() << "nextBlockSize: " << nextBlockSize;
		}

		if (tcpSocket->bytesAvailable() < nextBlockSize)
			break;

		qDebug() << "tcpSocket->bytesAvailable(): " << tcpSocket->bytesAvailable();
		in >> replyType;
		qDebug() << "replyType:" << replyType;

		if (UPDATE_CLIENT_PPC_STATUS == replyType) 
		{
			updatePPCStatusList(in);
		}
		else if (APPLY_SUCCESS == replyType || APPLY_FAILED == replyType ||
				 GIVE_BACK_SUCCESS == replyType || GIVE_BACK_FAILED == replyType)
		{
			in >> replyContent;
			qDebug() << "replyContent:" << replyContent;
			emit resultFromServer(replyType, replyContent);
		}
		else
		{
		}
		nextBlockSize = 0;
	}
	
}

void ManagePPCClient::sendMsg(const QString &msgType, const QString &ppcIP)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);

	qDebug() << "clientName: " << clientName << "Sending msg:" << msgType << "ppcIP: " << ppcIP;

	out << quint16(0) << clientName << msgType;
	if (APPLY_PPC == msgType || GIVE_BACK_PPC == msgType || GIVE_BACK_PPC_RESULT == msgType)
	{
		out << ppcIP;
	}

	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	tcpSocket->write(block);
}

void ManagePPCClient::updatePPCStatusList(QDataStream &in)
{
	bool newIP = true;
	IPStatus *ipStatus = new IPStatus;
	in >> ipStatus->IP >> ipStatus->status >> ipStatus->owner;

	qDebug() << "updatePPCStatusList: " << ipStatus->IP << ipStatus->status << ipStatus->owner;
	QStringList sendMsg;
	sendMsg << ipStatus->IP << ipStatus->status << ipStatus->owner;

	if (IPStatusList.isEmpty())
	{
		IPStatusList << *ipStatus;
		newIP = false;
	}
	else
	{
		for (int i = 0; i < IPStatusList.size(); i++)
		{
			if (IPStatusList.at(i).IP == ipStatus->IP)
			{
				//ipStatus->status = ipStatus->status;
				//ipStatus->owner = ipStatus->owner;
				IPStatusList.replace(i, *ipStatus);
				newIP = false;
			}
		}
	}

	if(newIP)
		IPStatusList << *ipStatus;

	emit updateTableWidget(IPStatusList);
}

void ManagePPCClient::closeConnection()
{
	tcpSocket->close();
}

void ManagePPCClient::error(QAbstractSocket::SocketError socketError)
{
	QString errorMsg;
	switch (socketError) 
	{
	case QAbstractSocket::RemoteHostClosedError:
		errorMsg = "Connection is closed by server.\nPlease make sure the server is running";
		break;
	case QAbstractSocket::HostNotFoundError:
		errorMsg = "The host was not found. \nPlease check the host name and port settings.";
		break;
	case QAbstractSocket::ConnectionRefusedError:
		errorMsg = "Connecting to server failed. \nPlease make sure the server is running";
		break;
	default:
		errorMsg = QString("The following error occurred: %1.").arg(tcpSocket->errorString());
	}
	emit connectToServerError(errorMsg);
}

bool operator ==(const IPStatus &lIP, const IPStatus &rIP)
{
	if (lIP.IP == rIP.IP && lIP.status == rIP.status && lIP.owner == rIP.owner)
		return true;
	else
		return false;
}


bool operator <(const IPStatus &lIP, const IPStatus &rIP)
{
	if(lIP.IP < rIP.IP)
		return true;
	else
		return false;
}
