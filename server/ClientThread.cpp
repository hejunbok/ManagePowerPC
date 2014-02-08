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
 
#include <QDateTime>
#include <QByteArray>
#include <QMessageBox>
#include "ClientThread.h"
#include "header.h"

ClientThread::ClientThread(int _id, QObject *parent) : QThread(parent), nextBlockSize(0)
{
    socketId = _id;
	quit = false;
}

void ClientThread::run()
{
    client = new Client(socketId);

	connect(client, SIGNAL(readyRead()), this, SLOT(readMsg()), Qt::DirectConnection);
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    //client->write("Welcome to the Server!");

	QLog_Trace("ClientThread", "New client from " + client->peerAddress().toString());

	while (!quit)
	{
		client->waitForReadyRead(1);
		for (int i = msgToWrite.size() - 1; i >= 0; i--)
		{
			client->write(msgToWrite.at(i).toLatin1());
			msgToWrite.removeAt(i);
		}
	}
}

void ClientThread::readMsg()
{
	QString requestType;
	QString clientName;
	QString PPCIP;
	
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_1);
    
	qDebug() << client->bytesAvailable();
    QLog_Trace("Server", "Received missage from client [" + (client->peerAddress().toString()) + "]");
	forever 
	{
		if (nextBlockSize == 0) 
		{
			if (client->bytesAvailable() < sizeof(quint16))
				return;
			in >> nextBlockSize;
		}	
		if (client->bytesAvailable() < nextBlockSize)
			return;

		in >> clientName >> requestType;
		QLog_Trace("ClientThread", QString("%1 client. Request: %2.").arg(clientName, requestType));
		qDebug() << "clientName" << clientName << "requestType: " << requestType;

		if (requestType == NEW_CLIENT) 
		{
			emit newClientMessage(clientName);
		}
		else if (requestType == UPDATE_PPC_STATUS) 
		{
			emit updateClientPPCStatusMessage(clientName);
		}
		else if (requestType == APPLY_PPC) 
		{
			in >> PPCIP;
			qDebug() << "PPCIP" << PPCIP;
			emit applyPPCMessage(clientName, PPCIP);
		}
		else if (requestType == GIVE_BACK_PPC) 
		{
			in >> PPCIP;
			qDebug() << "PPCIP" << PPCIP;
			emit giveBackPPCMessage(clientName, PPCIP);
		}
		else if (requestType == GIVE_BACK_PPC_RESULT) 
		{
			in >> PPCIP;
			qDebug() << "PPCIP" << PPCIP;
			emit giveBackPPCMessageResult(clientName, PPCIP);
		}

		else
		{
			QLog_Trace("ClientThread", QString("%1 is not defined.").arg(requestType));
		}
		nextBlockSize = 0;
	}
}

void ClientThread::disconnected()
{
    emit clientClosed(socketId);

    client->deleteLater();

	quit = true;
}

void ClientThread::sendMessage(const QString &msg)
{
	QLog_Trace("ClientThread", "ClientThread::sendMessage()");
	msgToWrite.prepend(msg);
}

