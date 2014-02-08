/****************************************************************************************
 ** Server is an application to manage several clients inside a thread.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#include <QtCore>
#include <QMutexLocker>
#include <QDateTime>
#include "Server.h"
#include "Client.h"
#include "header.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

Server::~Server()
{
}

void Server::incomingConnection(int socketDescriptor)
{
	ClientThread *client = new ClientThread(socketDescriptor);
	connect(client, SIGNAL(newClientMessage(QString)), this, SLOT(processNewClient()));
	connect(client, SIGNAL(updateClientPPCStatusMessage(QString)), this, SLOT(processUpdateClientPPCStatus()));	
	connect(client, SIGNAL(applyPPCMessage(QString, QString)), this, SIGNAL(applyPPCMessage(QString, QString)));
	connect(client, SIGNAL(giveBackPPCMessage(QString, QString)), this, SIGNAL(giveBackPPCMessage(QString, QString)));
	connect(client, SIGNAL(giveBackPPCMessageResult(QString, QString)), this, SIGNAL(giveBackPPCMessageResult(QString, QString)));
	connect(client, SIGNAL(clientClosed(int)), this, SLOT(threadFinished(int)), Qt::DirectConnection);
	connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));

	clientList.append(client);

	client->start();
}


void Server::threadFinished(int threadId)
{
	QString userExited;

	for (int i = 0; i < clientList.size(); i++)
	{
		if (clientList.at(i)->getThreadId() == threadId)
		{
			userExited = clientList.at(i)->getClient()->getName();
			clientList.removeAt(i);
		}
	}

	QLog_Trace("Server", "Thread finished");
}


void Server::updatePPCStatus(QList<IPStatus> &IPStatusList)
{
	this->IPStatusList = IPStatusList;
	(this->IPStatusList).clear();
	for(int i = 0; i < IPStatusList.size(); i++)
	{
        this->IPStatusList << IPStatusList.at(i);
	}
    
	for (int i = 0; i < clientList.size(); i++)
	{
        //sendPPCStatus(clientList.at(i)->getClient());
	}
}


void Server::processNewClient()
{
    //ClientThread *clSender = (ClientThread*)sender();
    //sendPPCStatus(clSender->getClient());
}

void Server::processUpdateClientPPCStatus()
{
	ClientThread *clSender = (ClientThread*)sender();
	sendPPCStatus(clSender->getClient());
}


void Server::sendPPCStatus(Client *client)
{
	for (int i = 0; i < IPStatusList.size(); i++)
	{
		sendSinglePPCStatus(IPStatusList.at(i).IP, IPStatusList.at(i).status, IPStatusList.at(i).owner, client);
	}

}

void Server::sendSinglePPCStatus(const QString &ip, const QString &status, const QString &owner, Client *client)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);

	qDebug() << "sendSinglePPCStatus: " << ip << status << owner;
	QStringList sendMsg;
	sendMsg << ip << status << owner;
	QLog_Trace("Server", sendMsg.join(", "));

	out << quint16(0) << UPDATE_CLIENT_PPC_STATUS << ip << status << owner;

	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	qDebug() << "getName: " << client->getName();
	client->write(block);
}

void Server::sendMsg(const QString &msg, const QString &msgContent)
{
	ClientThread *clSender = (ClientThread*)sender();

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);

	qDebug() << "Server msg: " << msg;

	out << quint16(0) << msg << msgContent;

	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	(clSender->getClient())->write(block);

}




