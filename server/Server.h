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
#ifndef Server_H
#define Server_H
 
#include <QStringList>
#include <QTcpServer>
#include <QThread>
#include <QMutex>
#include "ClientThread.h"
#include "header.h"



class Server : public QTcpServer
{
    Q_OBJECT

    public:
        Server(QObject *parent = 0);
        ~Server();

    protected:
        void incomingConnection(int socketDescriptor);
        
    private slots:
        void threadFinished(int threadId);
        
    public slots:
        void updatePPCStatus(QList<IPStatus> &IPStatusList);
        void processNewClient();
		void processUpdateClientPPCStatus();
		void sendMsg(const QString &msg, const QString &msgContent = "");
        
    signals:
        void applyPPCMessage(const QString &clientName, const QString &PPCIP);
        void giveBackPPCMessage(const QString &clientName, const QString &PPCIP);
		void giveBackPPCMessageResult(const QString &clientName, const QString &PPCIP);
        
    private:
        QList<ClientThread*> clientList;
        
        void sendPPCStatus(Client *client);
        void sendSinglePPCStatus(const QString &ip, const QString &status, const QString &owner, Client *client);
        
        QList<IPStatus> IPStatusList;

};

#endif

