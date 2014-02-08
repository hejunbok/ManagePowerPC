
#include <QtDebug>
#include <QString>
#include <QIntValidator>
#include "telnetppc.h"

TelnetPPC::TelnetPPC(QObject *par)
:QObject(par), qttelnet(new QtTelnet)
{    
	 
	thread = new Thread;
	moveToThread(thread);

    //Receive message from PPC
    connect(qttelnet, SIGNAL(message(const QString &)),
            this, SLOT(receiveMessage(const QString &)), Qt::DirectConnection);

    //If the PPC is configured wrong, it will trigger this slot
	connect(qttelnet, SIGNAL(connectionError(QAbstractSocket::SocketError)),
			this, SIGNAL(connectPPCError(QAbstractSocket::SocketError)), Qt::DirectConnection);

}

/*
** If this PPC is idle, it will return "->".
** When there's PPC has been connected, then stop connecting.
** If this PPC has been occupied, it will return "Sorry, session limit reached". 
*/
void TelnetPPC::receiveMessage(const QString &msg)
{
    QString stripMsg = stripCR(msg);
    
    if (!stripMsg.isEmpty())
	{
		emit receivedMsg(stripMsg);
		qDebug() << "stripMsg" << stripMsg ;
		if (stripMsg == "->")
		{
			emit telnetResult(IDLE);
		}
		else if (stripMsg == "Sorry,sessionlimitreached.")
		{
			emit telnetResult(BUSY);
		}
		else if (stripMsg == "Timeout")
		{
			emit telnetResult(TIMEOUT);
		}
		else
		{
		}
	}

}

void TelnetPPC::connectPPCError(QAbstractSocket::SocketError error)
{
	qDebug() << QString("Connection error: %1").arg(error);
    emit telnetResult(ERROR);
}

void TelnetPPC::sendMessage(const QString &msg)
{
    qttelnet->sendData(msg);
}

void TelnetPPC::connectPPC(const QString &ip)
{	
	qDebug() << "connectPPC: " << ip;
	qttelnet->connectToHost(ip);
 }

void TelnetPPC::disconnectPPC()
{
    qttelnet->close();
}

QString TelnetPPC::stripCR(const QString &msg)
{
    QString nmsg(msg);
    nmsg.remove('\r');
    nmsg.remove(QRegExp("\033\\[[0-9;]*[A-Za-z]")); // Also remove terminal control codes
	nmsg.remove(" ");
	nmsg.remove('\n');
    return nmsg;
}



