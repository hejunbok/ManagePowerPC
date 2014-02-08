/****************************************************************************
**
** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "ppcthread.h"

#include <QtNetwork>
#include <QString>

PPCThread::PPCThread(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), nextBlockSize(0)
{
}

void PPCThread::run()
{
	QTcpSocket *tcpSocket = new QTcpSocket;
	if (!tcpSocket->setSocketDescriptor(socketDescriptor)) 
	{
		emit error(tcpSocket->error());
		return;
	}
/*
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_1);

	//read block size
	if (nextBlockSize == 0) 
	{
		forever
		{
			qDebug() << tcpSocket->bytesAvailable();
			if (tcpSocket->bytesAvailable() < sizeof(quint16))
			{
				sleep(1);
				continue;
			}
		}
		in >> nextBlockSize;
	}

	forever
	{
		if (tcpSocket->bytesAvailable() < nextBlockSize)
		{
			msleep(10);
			continue;
		}
	}
	in >> nextBlockSize;
*/
	quint8 requestType;
	QString ip;
	QString status;
	QString owner;
/*
	in >> requestType;
	if (requestType == 'S')
		in >> ip;
	else
		return;
*/
	ppcStatus(ip, status, owner);

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << quint16(0) << ip << status << owner;
	out.device()->seek(0);
	out << quint16(block.size() - sizeof(quint16));
	out << quint16(0xFFFF);

	tcpSocket->write(block);
	tcpSocket->disconnectFromHost();
	tcpSocket->waitForDisconnected();

}

void PPCThread::ppcStatus(QString &ip, QString &status, QString &owner)
{
	if(ip.contains("20.2"))
	{
		status = "Idle";
		owner = "jingsz";
	}
}
